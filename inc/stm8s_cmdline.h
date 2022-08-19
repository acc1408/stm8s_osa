/**
  ******************************************************************************
  * @file    stm8s_cmdline.h
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the cmdline.
   ******************************************************************************
	 
	 

uint16_t cnt_on2=0;
// отключения и включение светодиода
int16_t led(uint8_t argc,char *argv[])
{
	if (argc==2)  
	{
		if (strcmp(argv[1],"on")==0) // включаем светодиод
		{
			cnt_on2++;
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
			printf("Светодиод включен %d\r\n",cnt_on2);
			return 0;
		}
		else 
			if (strcmp(argv[1],"off")==0)  // отключаем светодиод
			{
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
				printf("Светодиод отключен\r\n");
				return 0;
			}
			else
			{
				return 2;
			}
	}
	else
		return 1;
}


uint8_t bl=1; // переменная для включения и отключения мигания
// функция для включения и отключения мигания
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc==2) 
	{
		if (strcmp(argv[1],"on")==0) // отключаем светодиод
			bl=1;
		else 
			if (strcmp(argv[1],"off")==0) // включаем светодиод
				bl=0;
		return 0;
	}
	return 1;
}

// таблица соответствия название-функция храниться не в ОЗУ,
// а во FLESH, т.е. можно не беспокопиться за кол-во функций
// и длину строк
/*
const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};*/
// таблица соответсвия возврат-текст для резултататов возврата
/*
const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};
*/	 
	 
	 //============================ вывод данных из UART
	 /*
#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif
	 */
	 
	// Инициализация командной строки в main
	/*
	cmdinit('\r', // стоп-символ
					'\n', // 2 стоп-символ
					testf, // указатель на таблицу название-функция
					2,  	// кол-во функций в таблице
					RetText, // таблица номер-возврат текста
					2);		// размер массива номер-текст*/
	/*				
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// настройка приема символов из uart2 в командную строку
	//UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// Инициализация для вывода printf
	stdio_InitPrintf(uart2_sendtobuffer);
	UART2_Cmd(ENABLE);
	
	enableInterrupts();
	//printf("Check cmdline\r\n");
	
	*/ 
	 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CMDLINE_H
#define __STM8S_CMDLINE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_stdioinit.h"
#include <string.h>

// -3 - слишком много параметров
#define error_parametr "too many parametr\r\n"
// -2 - слишком длинная строка
#define error_long_string "too long string\r\n"
// -1 - код функции не найдена команда
#define error_command "command not found\r\n"
// размер входящего буфера
#define cmdrxnum_max 20
// максимальное кол-во параметров
#define cmdargc_max 3

// структура функции обработчика
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// структура таблицы соответсия название - функции 
typedef struct
{
	char name[20];	// название функции
	func_t func;		// указатель на функции
} tablefunc_t;
// структура таблицы соответствия номер возврата функции- текст вывода
typedef struct
{
	int16_t retfunc;	// номер возврата 
	char name[20];		// текст для вывода  
} tableRet_t;


typedef enum
{
	receive=0, 	// прием данных
	exec=0b01,			// принят стоп-байт
	error_ovf=0b10, // переполнение входящего буфера
	//error_param=0b100, //переполнение кол-ва параметров
	//exec_error_param=0b101, //принят стоп-бит и было переполнение параметров
	//exec_error_ovf_param=0x111, // принят стоп-бит, переполнение параметров и буфера
	exec_error_ovf=0b011 // приннят стоп-бит и было переполнение буфера
	
}cmdstate_t;

// тип структуры командной строки
typedef struct
{
	char rx[cmdrxnum_max]; // длина
	uint8_t rxnum; // номер текущего байта
	char stopbit; // первый стоп-бит обязательный
	char stopbit2; // второй стоп-бит дополнительный
	tablefunc_t *tabfunc; // указатель на массив функций
	uint8_t numfunc;			// кол-во функций
	cmdstate_t state; // состояние командной строки
	uint8_t argc; // кол-во аргументов
	char *argv[cmdargc_max]; // массив указателей на параметры 
	tableRet_t *TabRet; //указатель-таблица на массив возврата результат функции
	uint8_t NumTabRet; // размер массива возврат-текст 
}cmdline_t;

// инициализаци командной строки
void cmdinit(	char stopbit, // вводим символ стоп-бит
							char stopbit2, // вводим символ стоп-бит
							tablefunc_t tabfunc[], //указатель на массив таблицы
							uint8_t numfunc, // кол-во функций
							tableRet_t *tabret, //указатель-таблица на массив возврата результат функции
							uint8_t NumTabRet// размер массива возврат-текст
							) ;
// главный цикл для обработки кондной строки
void cmdmainloop(void);

#endif