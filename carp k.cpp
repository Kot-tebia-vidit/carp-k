//����������� ���������
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>

using namespace std;
using namespace System;
using namespace System::IO;

//����������� ������
#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

char dan[9][55]= //������ �������� ��� ����
{
					"������ �����������                                    ", 
					"���������� � ������� �������� ���� 100���. �          ", 
					"��������� � ���������� ������� ��������               ", 
					"��������� ��������                                    ", 
					"���� �� ���������� ��������� �������� ��. � ��.���    ",
					"��� ���������� ��������� �������� ��. � ��.���.       ",
					"���������                                             ", 
					"�����                                                 ",
					"                                                      "
};
char BlankLine[ ]=  "                                                        "; //������ ������


struct z 
{
	char name[30]; //��� ����������
	char vid[20];  //��� ����������(������������/��������������)
	long summa;    //����� �������� � ������
	long plata;    //�������� ����������
	char data[11]; //���� ��������
};
struct sp //������ �������������� ������
{
	char fio[20];		//��� ����������
	char vid[40];		//��� ����������(������������/��������������)
	long summa;			//����� �������� � ������
	long plata;			//�������� ����������
	struct sp* sled;	//��������� �� ��������� �������
	struct sp* pred;	//��������� �� ���������� �������
 } *spisok;
struct sp* spisko; //�������� ������

//���������� �������
int menu(int);
void max(struct z*,int);
void last(struct z*,int);
void text_data(char *,char *);
void sovpad(struct z *,int);
void vsesovpad(struct z *,int);
void alfalist(struct z*,int);
void vstavka(struct z*,char*,int);
void vstavkao(struct z*,char*,int);
void listing(struct z*,int);
void diagram(struct z*,int);
void raar(char *s);

int main(array<System::String ^> ^args)//����� �������, �������� �����
{
	int i,n,NEN; //NEN - ����� �����������
	FILE *in;	//����������
	struct z *postavs;	//������ ��������� �� ������ ������
	setlocale(LC_CTYPE,".1251");	//��������� �������� �����
	Console::CursorVisible::set(false);	// ������ ������ ���������
	Console::BufferHeight=Console::WindowHeight;	//����� ������ ����
	Console::BufferWidth=Console::WindowWidth;	//����� ������ ����

	//������������� �����������
	if((in=fopen("BD.txt","r"))==NULL)
	{
		printf("\n���� BD.txt �� ������!");	//������� ���������, ���� ���� �� ������
		getch(); exit(1);
	}
	Console::ForegroundColor=ConsoleColor::Red;
		Console::BackgroundColor=ConsoleColor::Red;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Red;
	//���� ������ �� ����� � ������ 
	fscanf(in,"%d",&NEN);
	postavs=(struct z*)malloc(NEN*sizeof(struct z));
	for(i=0;i<NEN;i++)
	{
		fscanf(in,"%s%s%ld%ld%s",            &postavs[i].name, &postavs[i].vid, &postavs[i].summa, &postavs[i].plata, &postavs[i].data);
		raar(postavs[i].name);
	}
	for(i=0;i<NEN;i++)
		printf("\n%-20s %-20s %7ld %7ld %s", postavs[i].name, postavs[i].vid, postavs[i].summa, postavs[i].plata, postavs[i].data);
	getch();

	while(1)	//������� ������� ��� �������� � ���������� �� � �����
	{
		Console::ForegroundColor=ConsoleColor::Cyan;
		Console::BackgroundColor=ConsoleColor::Blue;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//���������� ����� ������ ����� ����������� ������� ����
		Console::CursorTop=4;
		printf(BlankLine);
		for(i=0;i<9;i++)	//���������� ��������
		{
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ",dan[i]);
		}

		Console::CursorLeft=10;
		Console::CursorTop=12;
		printf(BlankLine);

		n=menu(8);	//��������� ������ � ���� � ��������
		switch(n) 
		{
			case 1: alfalist(postavs,NEN); break;	//��������� ������
			case 2: listing(postavs,NEN); break;	//�������� ����� 100���.�
			case 3: max(postavs,NEN); break;		//������������� ��������
			case 4: last(postavs,NEN); break;		//��������� ��������
			case 5: sovpad(postavs,NEN); break;		//������ ����������
			case 6: vsesovpad(postavs,NEN); break;	//��� ����������
			case 7: diagram(postavs,NEN); break;	//���������
			case 8: exit(0);					//����� �� ���������
		}
	}	 // ����� while(1)...
return 0;
}		 // ����� main()
void raar(char *s)
{
	//�������� ������� ������������� � ��� �����������
	char *t;
	t=strchr(s,'_');
    *t=' ';
}

int menu(int n)
{
	int y1=0,y2=n-1;
	char c=1;
	while (c!=ESC)
	{
		switch(c) //���������� ��������� � ������� �����
		{
			case DOWN: y2=y1; y1++; break;
			case UP: y2=y1; y1--; break;
			case ENTER: return y1+1;
		}
		if(y1>n-1) //����������� � ������ ������ ���� ����� ���� ���������
		{
			y2=n-1;
			y1=0;
		}
		if(y1<0)  //����������� � ��������� ������ ���� ���� ������
		{
			y2=0;
			y1=n-1;
		}
		//���� ��������� ������� ������
		Console::ForegroundColor=ConsoleColor::White;
		Console::BackgroundColor=ConsoleColor::Red;
		Console::CursorLeft=11;		//����� ���������
		Console::CursorTop=y1+5;
		printf("%s",dan[y1]);	//������� ����� ������� ��� ���

		Console::ForegroundColor=ConsoleColor::Black;	//������� ���������� � ���������� ������
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=11;//����� ������� ���������
		Console::CursorTop=y2+5;
		printf("%s",dan[y2]);
		c=getch();
	} // ����� while(c!=ESC)...
exit(0);
}

void max(struct z* postav,int NC) //������� ��� ����������� ����� ������� ��������
{
	int i=0; struct z best;
	strcpy(best.name,postav[0].name);
	best.summa=postav[0].summa;
	for(i=1;i<NC;i++)
		if (postav[i].summa>best.summa)
		{
			strcpy(best.name,postav[i].name);
			best.summa=postav[i].summa;
		}
	Console::ForegroundColor=ConsoleColor::White; //���� ������
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;//���������� ������
	Console::CursorTop=15;
	printf("��������� %s �������� ������ �� %ld�",best.name,best.summa);
	Console::CursorLeft=10;
	Console::CursorTop=16;
	getch();
}


void text_data(char *s,char *sd) //������� �������� ���� �� ������ � �������
{
	char s0[3],month[12][9]={
	"������","�������","�����","������","���","����",
	"����","�������","��������","�������","������","�������"};
	strcpy(s,sd+8);	//�������� ���� ������
	strcat(s," ");	//������ ������
	strncpy(s0,sd+5,2); s0[2]=0;	//�������� ����� ������ � ������ s0
	strcat(s,month[ atoi(s0)-1]);	//��������� ����� ������ � ����� � ��������� � ������
	strcat(s," ");	//������
	strncat(s,sd,4);	//���
	return;
}

void last(struct z* postav,int NC)	//����������� ��������� ��������
{
	int i;
	char s[17];//���� ��������
	struct z* best=postav;	//������� ���������
	for(i=1;i<NC;i++)	//���� ��������� �����
		if (strcmp(postav[i].data,best->data)>0)
			best=&postav[i];
	text_data(s,best->data);	//��������� ����������� ���� � �����

	Console::ForegroundColor=ConsoleColor::White;	//���� ������
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;	//���������� ������
	Console::CursorTop=15;
	Console::CursorLeft=10;
	Console::CursorTop=16;
	//����� ������
	printf("��������� �������� �������� %s �� %ld�  %s",best->name,best->summa,s);	
	getch();
}

void sovpad(struct z* postav,int NC)	
//������� ������, ����� ���������� ������ ������ ����� 2 ������������ � ������� ������ ��������
{
	int i,j,sum,t=0;
	struct z* best=postav;
	Console::ForegroundColor=ConsoleColor::White;	//���� ������
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;	//���������� ������
	Console::CursorTop=15;
	Console::CursorLeft=10;
	Console::CursorTop=16;

	for(i=0;i<NC;i++)	//������� ���� ���������
	{
		best=&postav[i];	//������ ��������� ��� ���������
		sum=best->summa;	//����� ��������
		for(j=(i+1);j<NC;j++)	//���������� ����
		{	//��������� 
			if ((sum==postav[j].summa) && (*best->vid != *postav[j].vid))	
			{
				t=1;	//�������� ���������� ��� ����������
				//����� ����������
				printf("\n����������: %s � %s ��������� ������� �� %d�",postav[j].name,best->name,sum);
				break;	//����� �� ����������� �����
			}
		}
		if(t==1)	//��� ����������
		{
			break;	//����� �� ����������� �����
		}
	}
	if (t==0)	//���� ��� ����������
		printf("\n ���������� ���");
	getch();
}


void vsesovpad(struct z* postav,int NC)	//����� ���� ����������
{
	int i,j,sum,k=0,t,*num;
	struct z* best=postav;
	char str[100],zp[100];
	num = new int [NC];
	Console::ForegroundColor=ConsoleColor::White;
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	
	for(j=i;j<NC;j++)	//��������� ����������� �����������
		num[j]=0;

	for(i=0;i<NC;i++)	//������� ����
	{
		best=&postav[i];
		t=1;
		sum=best->summa;
		strcpy(str,best->name);
		for(j=(i+1);j<NC;j++)	//���������� ����
		{	//�������� �� �������
			if (sum==postav[j].summa && num[j]==0)
			{
				num[j]=1;	//������������� ������������ �����������
				if (*best->vid != *postav[j].vid)
				{
					t=t+1;
				}
				strcat(str,", ");	//���������� � ������ ��� ������ ����������
				strcat(str,postav[j].name);
			}
		}
		if (t>1)
		{	//�����
			printf("\n%s ��������� ������� �� %d�",&str,sum);
			k++;
		}
	}	//����� ����������
	printf("\n����� ����������: %d",k);
	getch();
}

void alfalist(struct z* postav,int NC)	//������� ������ ����������� ������ � ��������� ����������� ������
{
	int i;
	struct sp* nt;	//��������� �� �������� �������� ������
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	if(!spisok)	 //���� ���������� ������ ��� �� ������, �� ������
		for(i=0;i<NC;i++)
			vstavka(postav,postav[i].name,NC);	//�������� ������� ��� ������� ��������
	if(!spisko)	//���� �������� ������ ��� �� ������, �� ������
		for(i=0;i<NC;i++)
			vstavkao(postav,postav[i].name,NC);	//�������� ������� ��� ������� ��������
	Console::Clear();
	printf("\n ���������� ������ �����������              �������� ������ ");
	printf("\n =============================              ================\n");
	printf("\n ���:                 �� �����:             ���:                 �� �����:\n");
	//������� 2 ������
	for(nt=spisok; nt!=0; nt=nt->sled)
		printf("\n\r %-20s %ld�",nt->fio,nt->summa);
	Console::CursorTop = 6;
	for(nt=spisko; nt!=0; nt=nt->sled) {
		Console::CursorLeft = 44;
		printf("%-20s %ld� \n",nt->fio,nt->summa);
	}
	getch();
}

void vstavka(struct z* postav,char* fio,int NC)	//������� ���������� �������� � ���������� ������
{
	int i;
	struct sp *nov,*nt,*z=0;	//��������� �� ����� ������� � ��������
	//���� �����, ���� �������� ����� �������
	for(nt=spisok; nt!=0 && strcmp(nt->fio,fio)<0; z=nt, nt=nt->sled);	
		if(nt && strcmp(nt->fio,fio)==0) //���� ����� ������� ��� ���� �� �� ���������
			return;
	nov=(struct sp *) malloc(sizeof(struct sp));	//�������� ������ ��� ����� �������
	strcpy(nov->fio,fio);	//�������� ������ � ����� �������
	nov->sled=nt;	 //����� ������ �� �������� ��������
	nov->summa=0;
	for(i=0;i<NC;i++)
		if(strcmp(postav[i].name,fio)==0)
			nov->summa+=postav[i].summa;
	if(!z) 
		spisok=nov;
	else z->sled=nov;
	return;
}

void vstavkao(struct z* postav,char* fio,int NC)	//������� ���������� �������� � �������� ������
{
	int i;
	struct sp *nov,*nt,*z=0;	//��������� �� ����� ������� � ��������
	//���� �����, ���� �������� ����� �������
	for(nt=spisko; nt!=0 && strcmp(nt->fio,fio)>0; z=nt, nt=nt->sled);
		if(nt && strcmp(nt->fio,fio)==0)  //���� ����� ������� ��� ���� �� �� ���������
			return;
	nov=(struct sp *) malloc(sizeof(struct sp));	//�������� ������ ��� ����� �������
	strcpy(nov->fio,fio);	//�������� ������ � ����� �������
	nov->sled=nt;	 //����� ������ �� �������� ��������
	nov->summa=0;
	//nov->vid="";
	for(i=0;i<NC;i++)
		if(strcmp(postav[i].name,fio)==0)
			nov->summa+=postav[i].summa;
			//nov->vid=postav[i].vid;
	if(!z) 
		spisko=nov;
	else z->sled=nov;
return;
}

void listing(struct z* postav,int NC)	//����� ������ �������� ����� 100���.�
{
	int i,f;
	struct z* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	printf("\n\r ������ ����������� � ������� ������� ���� 100���.������");
	printf("\n\r=====================================================\n\r");
	printf("\n ���:                 ��� ��������         ����������� �� �����:\n");
	for(i=0,nt=postav;i<NC;nt++,i++)	//��������� ����������� ��������
		if (nt->summa>100000)
			printf("\n\r %-20s %-20s %ld�",nt->name,nt->vid,nt->summa);
	getch();
}

void diagram(struct z *postav,int NC)	//��������� �� ���� �������� �� ������ ������
{
	struct sp *nt;
	int len,i,NColor;
	long sum = 0 ;
	char str1[20];
	char str2[20];
	System::ConsoleColor Color;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::White;
	Console::Clear();
	for(i=0;i<NC;i++) sum = sum+postav[i].summa ;
		if(!spisok)
			for(i=0;i<NC;i++)
				vstavka(postav,postav[i].name,NC);
	Color=ConsoleColor::Black; NColor=0;

	for(nt=spisok,i=0; nt!=0; nt=nt->sled,i++)
	{
		sprintf(str1,"%s",nt->fio);
		sprintf(str2,"%3.1f%%",(nt->summa*100./sum));
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor= ConsoleColor::White;
		Console::CursorLeft=5; Console::CursorTop=i+1;
		printf(str1);
		Console::CursorLeft=20;
		printf("%s",str2);
		Console::BackgroundColor=++Color; NColor++;
		Console::CursorLeft=30;
		for(len=0; len<nt->summa*100/sum; len++) printf(" ");
			if(NColor==14)
				Color=ConsoleColor::Black; NColor=0; 
	}
	getch();
	return ;
}
