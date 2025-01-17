// подключение библиотек
#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <malloc.h>
#include <clocale>
#include <windows.h>

using namespace std;
using namespace System;
using namespace System::IO;

#define ENTER 13 // обозначение кнопок
#define ESC 27
#define UP 72
#define DOWN 80
#define HOME 71
#define END 79

//int NC; //количество позиций задано глобально
struct z {
	char kod[20]; // артикул товара
	char tip[20]; // тип товара
	char firma[20]; // фирма товара
	char model[20]; // модель товара
	long cena; // цена товара
	long kolvo; // количество товара в наличии
}; //шаблон структуры для исходных данных
struct sp {
	char kod[30];
	long kolvo;
	struct sp* sled;
	struct sp* pred;
}; // шаблон, глобальное описание структуры двустроннего списка

int menu(int); //указание шаблонов используемых функций
void nout(struct z*, int);
void stoimost(struct z*, int);
void dorogo(struct z*, int);
void spis(struct z*, int, struct sp*);
void firm(struct z*, int);
void diagram(struct z*, int, struct sp*);
void vstavka(struct z*,char*, int, struct sp*);

// Ноутбуки дороже 30000
void nout(struct z* baza, int NC){
	int i;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::CursorTop=15;
	for(i=0;i<NC;i++)
	{
		if ( strcmp(baza[i].tip,"Ноутбук")==0 && baza[i].cena>30000)
		{
			Console::CursorLeft=10;
			printf(" Артикул: ");
			Console::ForegroundColor=ConsoleColor::Magenta;
			printf("%-45s \n",baza[i].kod);
			Console::ForegroundColor=ConsoleColor::Black;

		}
	}
	getch();
}

// Есть ли одинаковая стоимоть товаров одинаковой фирмы(сложный вопрос - поиск соответсвия)
void stoimost(struct z* baza, int NC) {
	int i, k, flag=0;
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	for (k=0; k<NC; k++)
	{
		for (i=k+1; i<NC; i++)
		{
			if(strcmp(baza[i].firma, baza[k].firma)==0 && (baza[i].cena == baza[k].cena) && (flag == 0))
			{
				flag++;
				printf(" Фирма: ");
				Console::ForegroundColor=ConsoleColor::Magenta;
				printf("%47s \n",baza[i].firma);
				Console::ForegroundColor=ConsoleColor::Black;
				Console::CursorLeft=10;
				printf(" Артикулы: ");
				Console::ForegroundColor=ConsoleColor::Magenta;
				printf("%37s",baza[i].kod);
				Console::ForegroundColor=ConsoleColor::Black;
				printf(" и ");
				Console::ForegroundColor=ConsoleColor::Magenta;
				printf("%s \n", baza[k].kod);
				Console::ForegroundColor=ConsoleColor::Black;
				Console::CursorLeft=10;
				printf(" Цена: ");
				Console::ForegroundColor=ConsoleColor::Magenta;
				printf("%48ld ",baza[i].cena);
				Console::ForegroundColor=ConsoleColor::Black;
				break;
			}

		}
	}
	if (flag==0)
	{
		printf("Совпадений не найдено!");
	}
	getch();
}

// Самый дорогой товар (функция поиска максимума)
void dorogo(struct z* baza, int NC) {
	int i=0; struct z best;
	strcpy(best.kod,baza[0].kod); //в переменную структуры записываем нулевой элемент
	best.cena=baza[0].cena;
	for(i=1;i<NC;i++)
		if (baza[i].cena>best.cena) //условие поиска максимальной стоимости
			{
			strcpy(best.kod,baza[i].kod);
			best.cena=baza[i].cena;
			}
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::CursorLeft=10;
	Console::CursorTop=15;
	printf(" Максимальная цена:"); // Вывод самого дорого товара
	Console::ForegroundColor=ConsoleColor::Magenta;
	printf("%29ld", best.cena);
	Console::ForegroundColor=ConsoleColor::Black;
	printf(" рублей ");
	Console::CursorLeft=10;
	Console::CursorTop=16;
	printf(" Артикул: ");
	Console::ForegroundColor=ConsoleColor::Magenta;
	printf("%45s ",best.kod);
	getch();
}

// Алфавитный список артикулов товаров и их кол-во
void vstavka(struct z* baza,char* kod, int NC, struct sp** spisok) //Вставка в список
{ 
	int i;
	struct sp *nov,*nt,*z=0;
	for(nt=*spisok; nt!=0 && strcmp(nt->kod,kod)<0; z=nt, nt=nt->sled);
		if(nt && strcmp(nt->kod,kod)==0) return;
			nov=(struct sp*) malloc(sizeof(struct sp));
			strcpy(nov->kod,kod);
			nov->sled=nt;
			nov->pred=z;
			nov->kolvo=0;
			for(i=0;i<NC;i++)
				if(strcmp(baza[i].kod,kod)==0)
					nov->kolvo+=baza[i].kolvo;
					if (!z) *spisok=nov;
					else z->sled=nov;
					if (nt) nt->pred=nov;
					return;
}


void spis(struct z* baza, int NC, struct sp** spisok)//Формирование списка
{
	int i;
	struct sp *nt,*z;
	Console::BufferHeight=720;
	Console::BufferWidth=1280;
	Console::ForegroundColor=ConsoleColor::Cyan;
	Console::BackgroundColor=ConsoleColor::DarkCyan;
	Console::Clear();
	
	printf("\n\t Алфавитный список");
	printf("\t\t\t\tОбратный список");
	printf("\n\n================================================================================\n");
	if(!*spisok)
		for(i=0;i<NC;i++)
			vstavka(baza,baza[i].kod, NC, spisok);
	for (nt=*spisok; nt; nt = nt->sled)
		printf("\n %-10s %ld", nt->kod, nt->kolvo);
	
	for(nt=*spisok, z=0; nt; z=nt, nt=nt->sled)
		Console::CursorTop=5;
		for (nt=z, i=0; nt; i++, nt=nt->pred)
		{
		Console::CursorLeft = 56;
		printf("%-10s %ld", nt->kod, nt->kolvo);
		Console::CursorTop+=1;
		}
		//printf("\n %-10s %ld",nt->kod,nt->kolvo);
	printf("\n================================================================================");
	getch();
}



// Введите фирму, чтобы увидеть их товары (Ввод названия фирмы с клавиатуры)
void firm(struct z* baza, int NC) {
	Console::ForegroundColor=ConsoleColor::Cyan;
	Console::BackgroundColor=ConsoleColor::DarkCyan;
	Console::Clear();
	int i, kolvo=0;
	char search[15];
	printf ("Список товаров конкретной фирмы \n");
	printf ("Введите название фирмы:");
	Console::CursorLeft++;
	Console::CursorVisible::set(true); //включаем курсор
	SetConsoleCP(1251); //для считывания русских символов
	scanf("%s",search); //вписываем нужный текст
	SetConsoleCP(866); //кодировка консоли, чтобы могли выводить русские символы
	Console::CursorVisible::set(false);
	printf("\nАртикул    Тип товара     Модель     Цена\n");
	printf("\n================================================================================");
	for (i=0;i<NC;i++)
		{
		if(strcmp(baza[i].firma,search)==0)
			{
			printf("\n %-10s %-15s %-7s %-15d \n", baza[i].kod,baza[i].tip,baza[i].model,baza[i].cena);
			kolvo++;
			}
		}
	printf("\n================================================================================");
	if (kolvo==0) printf("\nТоваров данной фирмы не найдено");
	if (kolvo) printf("\nКол-во товаров данной фирмы: %-7d",kolvo);
	getch();
	return ;

}

//Диаграмма. Процентное соотношение кол-ва товаров
void diagram(struct z* baza, int NC,struct sp** spisok) { //создаем диаграмму
	struct sp* nt; //перемеенная для работы со списком
	int len,i,NColor;  //i-для номера строки
	long sum = 0 ; //число-100%
	char str1[20];
	char str2[20];
	System::ConsoleColor Color; //Задаѐт константы, которые определяют основной цвет и цвет фона консоли
	Console::ForegroundColor=ConsoleColor::Cyan; //цвет символов
	Console::BackgroundColor=ConsoleColor::DarkCyan; //цвет заднего фона
	Console::Clear();
	for(i=0;i<NC;i++) sum = sum+baza[i].kolvo ;
	if(!*spisok)
		for(i=0;i<NC;i++)
			vstavka(baza,baza[i].kod, NC, spisok); //вставляем из списка названия имен и красим символы в серый цвет
	Color=ConsoleColor::Gray;
	NColor=8;

	printf("  Артикул       Процент кол-ва в наличии");
	for(nt=*spisok,i=0; nt!=0; nt=nt->sled,i++)
	{
		sprintf(str1,"%s",nt->kod);
		sprintf(str2,"%3.1f%%",(nt->kolvo*100./sum));
		Console::ForegroundColor=ConsoleColor::Cyan;
		Console::BackgroundColor=ConsoleColor::DarkCyan;;
		Console::CursorLeft=3; Console::CursorTop=i+2;
		printf(str1);
		Console::CursorLeft=20;
		printf("%s",str2);
		Console::BackgroundColor=++Color; NColor++;
		Console::CursorLeft=30;
		for(len=0; len<nt->kolvo*100/sum; len++) printf(" ");
		if(NColor==14) //выделяем на диаграмму 14 цветов
			{Color=ConsoleColor::Gray; NColor=8;}
	}
	getch();
	return ;
	}

int main(array<System::String ^> ^args) {
	char dan[7][55]={
		"Ноутбуки дороже 30000                                 ",
		"Есть ли одинаковая стоимоть товаров одинаковой фирмы  ",
		"Самый дорогой товар                                   ",
		"Алфавитный список артикулов товаров и их кол-во       ",
		"Введите фирму, чтобы увидеть их товары                ",
		"Диаграмма. Процентное соотношение кол-ва товаров      ",
		"Выход                                                 "
		}; // задание списка вопросов - пунктов меню
	char BlankLine[ ]= "                                                        ";
	//на локальном уровне осуществляем открытие файла,
	//считываем данные из файла в выделенную память
	int i,n;
	int NC;
	struct sp *spisok=0;
	FILE *in;
	struct z *baza;
	setlocale(LC_ALL,"Russian"); // поддержка русского языка
	Console::CursorVisible::set(false); // отключаем выдимость курсора
	Console::BufferHeight=Console::WindowHeight; // установка размера буфера по ширине
	Console::BufferWidth=Console::WindowWidth; // установка размера буфера по высоте
	if((in=fopen("ishodn.dat","r"))==NULL) { //попытка открыть файл
		printf("\nФайл ishodn.dat не открыт !"); //при неудачном открытии выводим соообщение “Файл не открыт!”
		getch(); exit(1);}
	printf("Список товаров в магазине:\n");
	printf("                          \n");
	printf("Артикул      Тип           Фирма         Модель      Цена    Кол-во в наличии\n");
	printf("\n================================================================================");
	fscanf(in,"%d",&NC);
	baza=(struct z*)malloc(NC*sizeof(struct z)); // выделяем память
	for(i=0;i<NC;i++) // считываем данные из списка
		fscanf(in,"%s%s%s%s%ld%ld", baza[i].kod, baza[i].tip, baza[i].firma, baza[i].model, &baza[i].cena, &baza[i].kolvo);
	for(i=0;i<NC;i++){ // выводим данные из списка на экран
		printf("\n%-10s %-15s %-15s %-10s %5ld %12ld", baza[i].kod, baza[i].tip, baza[i].firma, baza[i].model, baza[i].cena,	baza[i].kolvo);}
	printf("\n\n================================================================================");
	getch();
	while(1){ //в цикле создаем область для вопросов и окрашиваем еѐ в цвета; устанавливаем для букв цвет и подсветку для выбранного вопроса
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::DarkCyan;
		Console::Clear();
		Console::ForegroundColor=ConsoleColor::Black;
		Console::BackgroundColor=ConsoleColor::Cyan;
		Console::CursorLeft=10; //первая точка, откуда будем закрашивать область меню
		Console::CursorTop=4;
		printf(BlankLine);
		for(i=0;i<7;i++){ //размещение вопросов
			Console::CursorLeft=10; //последняя точка,где будет заканчиваться выделенная область под меню
			Console::CursorTop=i+5;
			printf(" %s ",dan[i]);}
		Console::CursorLeft=10;
		Console::CursorTop=12;
		printf(BlankLine);
		n=menu(7); //выбор вопроса в меню
		switch(n) {
			case 1: nout(baza, NC); break;
			case 2: stoimost(baza, NC); break;
			case 3: dorogo(baza, NC); break;
			case 4: spis(baza, NC, &spisok); break;
			case 5: firm(baza, NC); break;
			case 6: diagram(baza, NC, &spisok); break;
			case 7: exit(0);}
	} // конец while(1)...zx
	return 0;
} // конец main()
int menu(int n){ //n=8
	int NC;
	char dan[7][55]={
		"Ноутбуки дороже 30000                                 ",
		"Есть ли одинаковая стоимоть товаров одинаковой фирмы  ",
		"Самый дорогой товар                                   ",
		"Алфавитный список артикулов товаров и их кол-во       ",
		"Введите фирму, чтобы увидеть их товары                ",
		"Диаграмма. Процентное соотношение кол-ва товаров      ",
		"Выход                                                 "
		}; // задание списка вопросов - пунктов меню
	char BlankLine[ ]= "                                                        ";
	int y1=0,y2=n-1; // устанавливаем выделение для строчки
	char c=1;
	while (c!=ESC){ //при нажатии кнопки ESC, меню закроется
		switch(c) {
		case DOWN: y2=y1; y1++; break; //программирование кнопки DOWN, которая позволяет перемещаться вниз по меню
		case UP: y2=y1; y1--; break; //программирование кнопки UP, которая позволяет перещаться вверх по меню
		case ENTER: return y1+1; //программирование кнопки ENTER, которая позволяет выбрать вопрос
		case HOME: y2=y1; y1=0;break; //программирование кнопки HOME, которая премещает выбор на первый вопрос
		case END: y2=y1; y1=n-1; break; //программирование кнопки END, которая премещает выбор на последний вопрос
		}
	if(y1>n-1){y2=n-1;y1=0;} // условие,когда срабатывает кнопка DOWN
	if(y1<0) {y2=0;y1=n-1;} // условие,когда срабатывает кнопка UP
	Console::ForegroundColor=ConsoleColor::Magenta; // при выборе вопроса буквы окрашиваются в розовый цвет
	Console::BackgroundColor=ConsoleColor::DarkCyan; // при выборе вопроса план за буквами подсвечивается темно синим
	Console::CursorLeft=11;
	Console::CursorTop=y1+5;
	printf("%s",dan[y1]);
	Console::ForegroundColor=ConsoleColor::Black;
	Console::BackgroundColor=ConsoleColor::Cyan;
	Console::CursorLeft=11;
	Console::CursorTop=y2+5;
	printf("%s",dan[y2]);
	c=getch();
	} // конец while(c!=ESC)...
exit(0);
}