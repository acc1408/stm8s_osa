/**
  ******************************************************************************
  * @file    stm8s_cmdline.c
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the cmdline.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CMDLINE_C
#define __STM8S_CMDLINE_C

/* Includes ------------------------------------------------------------------*/
#include "inc/stm8s_cmdline.h"
// -3 - слишком много параметров
// -2 - слишком длинная строка
// -1 - код функции не найдена команда

cmdline_t cmd; // структура командной строки
// инициализаци командной строки
void cmdinit(	char stopbit, // вводим символ стоп-бит
							char stopbit2, // вводим символ стоп-бит
							tablefunc_t tabfunc[], //указатель на массив таблицы
							uint8_t numfunc, // кол-во функций
							tableRet_t *tabret, //указатель-таблица на массив возврата результат функции
							uint8_t NumTabRet// размер массива возврат-текст
							) 
{
	cmd.stopbit=stopbit; //первый стоп-бит
	cmd.stopbit2=stopbit2; //второй  стоп-бит
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
	cmd.TabRet=tabret;
	cmd.NumTabRet=NumTabRet;
	
}


void cmdinputchar(char c)
{
	switch(cmd.state)
	{
		// прием данных
		case receive:
				// проверяем место в буфере
				if (cmd.rxnum<cmdrxnum_max) 
					cmd.rx[cmd.rxnum++]=c; // заполняем буфер 
				else 	
					{
					cmd.state=error_ovf; // фиксируем ошибку переполнения буфера
					}
				//break;
		// если произошло переполнение ожидаем стоп-бита
		case error_ovf:
				if (c==cmd.stopbit) // если получен стоп-бит, 
						cmd.state|=exec; // ставим стоп-бит на обработку
				break;
		//default:
			//		nop();
			//	break;
	}
}
// разбор на слова, вместо пробелов вставляем конец строки
// в массив заносим указатели на начала слов
// Если в результате разбора функции больше параметров, чем 
// допустимо, то результат будет 0, если меньше, то результат 1
uint8_t cmdparcer(void)
{
	// заменяем все знаки пробелов на конец строки
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // если символ пробел
			cmd.rx[i]='\0'; // то заменяем его на конец строки
	cmd.rx[cmd.rxnum-1]='\0';
	//фильтруем последний стоп-бит если он станет первым
	if (cmd.rx[0]==cmd.stopbit2) 
		cmd.rx[0]='\0'; // заменяем второй стоп-бит на пробел
	// определяем начало строк
	// Для первого символа в строке определяем что это начало функции
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx; // заменяем все пробелы на знак конец строки
		}
	// Для следующих символов определяем начало строк.
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0'))
			if (cmd.argc<cmdargc_max)
				cmd.argv[cmd.argc++]=cmd.rx+i;
			else
				return -1; //ошибка кол-во параметров больше, чем допустимо
	
	return 0; // кол-во параметров допустимо
	
			
}

// сброс состояния автомата командной строки
void cmdreset(void)
{
	cmd.rxnum=0;
	cmd.state=receive;
	cmd.argc=0;
}

// поиск фукнции для выполнения
void cmdexec(void)
{
	uint8_t i;
	int16_t ret_func; // номер возврата функции
	char *funcfind; // указатель на строку
	func_t funcexec;// указатель на функцию
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // указатель на строку поиска
		funcexec=cmd.tabfunc[i].func; // указатель на соответствующую строку
		if (strcmp(cmd.argv[0],funcfind)==0) // если нашли соответствующую строчку
		{
			// выполняем функцию 
			ret_func=funcexec(cmd.argc,cmd.argv);
			// выполняем поиск соответвия возврат-текст
			for(i=0;i<cmd.NumTabRet;i++)
			{
				if (ret_func==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name);
					cmdreset(); // сброс автомата для сбора символов
					return;		// выход из функции
				}
			}
		}
	}	
		/*
		// если не нашли функцию, то производим поиск ошибки на неверную команду
		for(i=0;i<cmd.NumTabRet;i++)
			{
				// -1 - код функции не найдена команда
				if (-1==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name); // если найдено совпадение, то печатаем ошибку
					break; // выходим из цикла
				}
			}
			*/
			printf(error_command);
		cmdreset(); // сброс автомата для сбора символов
		return;		// выход из функции
		
}

// главный цикл для обработки кондной строки
void cmdmainloop(void)
{
	uint8_t i;
	switch(cmd.state) // проверяем состояние строки
		{
			case exec: // 
					if (!cmdparcer()) 	
							cmdexec();
						else
						{
							/*
							for(i=0;i<cmd.NumTabRet;i++)
							{
								// -3 - слишком много параметров
								if (-3==cmd.TabRet[i].retfunc) 
								printf("%s",cmd.TabRet[i].name);
							}
							*/
							printf(error_parametr);
							cmdreset();
						}
				break;
			case 	exec_error_ovf:
					/*
					for(i=0;i<cmd.NumTabRet;i++)
					{
						// -2 - слишком длинная строка
						if (-2==cmd.TabRet[i].retfunc) 
							printf("%s",cmd.TabRet[i].name);
					}
					*/
					printf(error_long_string);
					cmdreset();
				break;
		}
}


#endif