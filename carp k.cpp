//подключение библиотек
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

//обозначения кнопок
#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

char dan[9][55]= //тексты вопросов для меню
{
					"Список поставщиков                                    ", 
					"Поставщики с объёмом поставки выше 100тыс. р          ", 
					"Поставщик с наибольшим обьёмом поставок               ", 
					"Последняя поставка                                    ", 
					"Есть ли совпадение стоимости поставок об. и ст.мат    ",
					"Все совпадение стоимости поставки об. и ст.мат.       ",
					"Диаграмма                                             ", 
					"Выход                                                 ",
					"                                                      "
};
char BlankLine[ ]=  "                                                        "; //чёрная строка


struct z 
{
	char name[30]; //фио поставщика
	char vid[20];  //что поставляет(оборудование/стройматериалы)
	long summa;    //объём поставки в рублях
	long plata;    //зарплата поставщику
	char data[11]; //дата поставки
};
struct sp //шаблон двухстороннего списка
{
	char fio[20];		//фио поставщика
	char vid[40];		//что поставляет(оборудование/стройматериалы)
	long summa;			//объём поставки в рублях
	long plata;			//зарплата поставщику
	struct sp* sled;	//указатель на следующий элемент
	struct sp* pred;	//указатель на предыдущий элемент
 } *spisok;
struct sp* spisko; //обратный список

//объявление функций
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

int main(array<System::String ^> ^args)//Вывод менюшки, отурытие файла
{
	int i,n,NEN; //NEN - число поставщиков
	FILE *in;	//Дискриптер
	struct z *postavs;	//создаём структуру на основе шаблон
	setlocale(LC_CTYPE,".1251");	//поддержка русского языка
	Console::CursorVisible::set(false);	// делаем курсор невидимым
	Console::BufferHeight=Console::WindowHeight;	//задаём высоту окна
	Console::BufferWidth=Console::WindowWidth;	//задаём ширину окна

	//использование дескриптера
	if((in=fopen("BD.txt","r"))==NULL)
	{
		printf("\nФайл BD.txt не найден!");	//выводим сообщение, если файл не открыт
		getch(); exit(1);
	}
	Console::ForegroundColor=ConsoleColor::Red;
		Console::BackgroundColor=ConsoleColor::Red;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Red;
	//ввод данных из файла и печать 
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

	while(1)	//создаем область для вопросов и окрашиваем ее в цвета
	{
		Console::ForegroundColor=ConsoleColor::Cyan;
		Console::BackgroundColor=ConsoleColor::Blue;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10;	//координаты точки откуда будем закрашивать область меню
		Console::CursorTop=4;
		printf(BlankLine);
		for(i=0;i<9;i++)	//размещение вопросов
		{
			Console::CursorLeft=10;
			Console::CursorTop=i+5;
			printf(" %s ",dan[i]);
		}

		Console::CursorLeft=10;
		Console::CursorTop=12;
		printf(BlankLine);

		n=menu(8);	//связываем вопрос в меню с функцией
		switch(n) 
		{
			case 1: alfalist(postavs,NEN); break;	//афавитный список
			case 2: listing(postavs,NEN); break;	//поставки свыше 100тыс.р
			case 3: max(postavs,NEN); break;		//максимальнвая поставка
			case 4: last(postavs,NEN); break;		//последняя поставка
			case 5: sovpad(postavs,NEN); break;		//первое совпадение
			case 6: vsesovpad(postavs,NEN); break;	//все совпадения
			case 7: diagram(postavs,NEN); break;	//диаграмма
			case 8: exit(0);					//выход из программы
		}
	}	 // конец while(1)...
return 0;
}		 // конец main()
void raar(char *s)
{
	//Удаление нижнего подчёркивания в фио поставщиков
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
		switch(c) //управление стрелками и кнопкой ввода
		{
			case DOWN: y2=y1; y1++; break;
			case UP: y2=y1; y1--; break;
			case ENTER: return y1+1;
		}
		if(y1>n-1) //возвращение к первой кнопке если зашли ниже последней
		{
			y2=n-1;
			y1=0;
		}
		if(y1<0)  //возвращение к последней кнопке если выше первой
		{
			y2=0;
			y1=n-1;
		}
		//цвет подсветки текущей кнопки
		Console::ForegroundColor=ConsoleColor::White;
		Console::BackgroundColor=ConsoleColor::Red;
		Console::CursorLeft=11;		//место подсветки
		Console::CursorTop=y1+5;
		printf("%s",dan[y1]);	//выводим текст вопроса еще раз

		Console::ForegroundColor=ConsoleColor::Black;	//убираем подстветку с предыдущей кнопки
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=11;//место пропажи подсветки
		Console::CursorTop=y2+5;
		printf("%s",dan[y2]);
		c=getch();
	} // конец while(c!=ESC)...
exit(0);
}

void max(struct z* postav,int NC) //функция для определения самой большой поставки
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
	Console::ForegroundColor=ConsoleColor::White; //цвет ответа
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;//координаты ответа
	Console::CursorTop=15;
	printf("Поставщик %s поставил товара на %ldр",best.name,best.summa);
	Console::CursorLeft=10;
	Console::CursorTop=16;
	getch();
}


void text_data(char *s,char *sd) //функция перевода даты из текста в символы
{
	char s0[3],month[12][9]={
	"января","февраля","марта","апреля","мая","июня",
	"июля","августа","сентября","октября","ноября","декабря"};
	strcpy(s,sd+8);	//копируем день месяца
	strcat(s," ");	//ставим пробел
	strncpy(s0,sd+5,2); s0[2]=0;	//копируем номер месяца в строку s0
	strcat(s,month[ atoi(s0)-1]);	//переводим номер месяца в текст и добавляем в строку
	strcat(s," ");	//пробел
	strncat(s,sd,4);	//год
	return;
}

void last(struct z* postav,int NC)	//определение последней поставки
{
	int i;
	char s[17];//дата поставки
	struct z* best=postav;	//частный поставщик
	for(i=1;i<NC;i++)	//ищем последнее число
		if (strcmp(postav[i].data,best->data)>0)
			best=&postav[i];
	text_data(s,best->data);	//переводим необходимую дату в текст

	Console::ForegroundColor=ConsoleColor::White;	//цвет ответа
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;	//координаты ответа
	Console::CursorTop=15;
	Console::CursorLeft=10;
	Console::CursorTop=16;
	//вывод ответа
	printf("Последнюю поставку совершил %s на %ldр  %s",best->name,best->summa,s);	
	getch();
}

void sovpad(struct z* postav,int NC)	
//сложный вопрос, вывод совпадения объёма рублей между 2 поставщиками с разными видами поставки
{
	int i,j,sum,t=0;
	struct z* best=postav;
	Console::ForegroundColor=ConsoleColor::White;	//цвет ответа
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;	//координаты ответа
	Console::CursorTop=15;
	Console::CursorLeft=10;
	Console::CursorTop=16;

	for(i=0;i<NC;i++)	//внешний цикл сравнения
	{
		best=&postav[i];	//первый поставщик для сравнения
		sum=best->summa;	//объём поставки
		for(j=(i+1);j<NC;j++)	//внутренний цикл
		{	//сравнение 
			if ((sum==postav[j].summa) && (*best->vid != *postav[j].vid))	
			{
				t=1;	//флаговая перепенная при совпадении
				//вывод совпадения
				printf("\nСовпадение: %s и %s поставили товаров на %dр",postav[j].name,best->name,sum);
				break;	//выход из внутреннего цикла
			}
		}
		if(t==1)	//при совпадении
		{
			break;	//выход из внутреннего цикла
		}
	}
	if (t==0)	//если нет совпадений
		printf("\n Совпадений нет");
	getch();
}


void vsesovpad(struct z* postav,int NC)	//вывод всех совпадений
{
	int i,j,sum,k=0,t,*num;
	struct z* best=postav;
	char str[100],zp[100];
	num = new int [NC];
	Console::ForegroundColor=ConsoleColor::White;
	Console::BackgroundColor=ConsoleColor::Blue;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	
	for(j=i;j<NC;j++)	//обнуление проверенных поставщиков
		num[j]=0;

	for(i=0;i<NC;i++)	//внешний цикл
	{
		best=&postav[i];
		t=1;
		sum=best->summa;
		strcpy(str,best->name);
		for(j=(i+1);j<NC;j++)	//внутренний цикл
		{	//проверка на условия
			if (sum==postav[j].summa && num[j]==0)
			{
				num[j]=1;	//фиксированние проверенного поставщиков
				if (*best->vid != *postav[j].vid)
				{
					t=t+1;
				}
				strcat(str,", ");	//вписывание в строку имён нужных постащиков
				strcat(str,postav[j].name);
			}
		}
		if (t>1)
		{	//вывод
			printf("\n%s поставили товаров на %dр",&str,sum);
			k++;
		}
	}	//всего совпадений
	printf("\nВсего совпадений: %d",k);
	getch();
}

void alfalist(struct z* postav,int NC)	//Функция вывода алфавитного списка и обратного алфавитного списка
{
	int i;
	struct sp* nt;	//указатели на соседние элементы списка
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	if(!spisok)	 //если алфавитный список ещё не создан, то создаём
		for(i=0;i<NC;i++)
			vstavka(postav,postav[i].name,NC);	//вызываем функцию для каждого элемента
	if(!spisko)	//если обратный список ещё не создан, то создаём
		for(i=0;i<NC;i++)
			vstavkao(postav,postav[i].name,NC);	//вызываем функцию для каждого элемента
	Console::Clear();
	printf("\n Алфавитный список поставщиков              Обратный список ");
	printf("\n =============================              ================\n");
	printf("\n ФИО:                 На сумму:             ФИО:                 На сумму:\n");
	//выводим 2 списка
	for(nt=spisok; nt!=0; nt=nt->sled)
		printf("\n\r %-20s %ldр",nt->fio,nt->summa);
	Console::CursorTop = 6;
	for(nt=spisko; nt!=0; nt=nt->sled) {
		Console::CursorLeft = 44;
		printf("%-20s %ldр \n",nt->fio,nt->summa);
	}
	getch();
}

void vstavka(struct z* postav,char* fio,int NC)	//фукнция добавления элемента в алфавитный список
{
	int i;
	struct sp *nov,*nt,*z=0;	//указатель на новый элемент и соседние
	//ищем место, куда вставить новый элемент
	for(nt=spisok; nt!=0 && strcmp(nt->fio,fio)<0; z=nt, nt=nt->sled);	
		if(nt && strcmp(nt->fio,fio)==0) //если такой элемент уже есть то не добавляем
			return;
	nov=(struct sp *) malloc(sizeof(struct sp));	//выделяем память под новый элемент
	strcpy(nov->fio,fio);	//копируем данные в новый элемент
	nov->sled=nt;	 //задаём ссылки на соседние элементы
	nov->summa=0;
	for(i=0;i<NC;i++)
		if(strcmp(postav[i].name,fio)==0)
			nov->summa+=postav[i].summa;
	if(!z) 
		spisok=nov;
	else z->sled=nov;
	return;
}

void vstavkao(struct z* postav,char* fio,int NC)	//фукнция добавления элемента в обратный список
{
	int i;
	struct sp *nov,*nt,*z=0;	//указатель на новый элемент и соседние
	//ищем место, куда вставить новый элемент
	for(nt=spisko; nt!=0 && strcmp(nt->fio,fio)>0; z=nt, nt=nt->sled);
		if(nt && strcmp(nt->fio,fio)==0)  //если такой элемент уже есть то не добавляем
			return;
	nov=(struct sp *) malloc(sizeof(struct sp));	//выделяем память под новый элемент
	strcpy(nov->fio,fio);	//копируем данные в новый элемент
	nov->sled=nt;	 //задаём ссылки на соседние элементы
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

void listing(struct z* postav,int NC)	//вывод списка поставок свыше 100тыс.р
{
	int i,f;
	struct z* nt;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::Clear();
	printf("\n\r Список поставщиков с объёмом товаров выше 100тыс.рублей");
	printf("\n\r=====================================================\n\r");
	printf("\n ФИО:                 Вид поставки         Поставленно на сумму:\n");
	for(i=0,nt=postav;i<NC;nt++,i++)	//подбираем необходимые поставки
		if (nt->summa>100000)
			printf("\n\r %-20s %-20s %ldр",nt->name,nt->vid,nt->summa);
	getch();
}

void diagram(struct z *postav,int NC)	//диаграмма по доле поставки от общего объёма
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
