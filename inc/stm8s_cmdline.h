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