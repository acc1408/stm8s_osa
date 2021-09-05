/**
  ******************************************************************************
  * @file    stm8s_stdioinit.c
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the init printf.
   ******************************************************************************/
	
#ifndef __STM8S_STDIOINIT_C
#define __STM8S_STDIOINIT_C
#include "inc/stm8s_stdioinit.h"
inputchar_t sendchar=0;

// инициализация функции для передачи байт для функции printf
void stdio_InitPrintf(void (*putcharFunc)(char))
{
	sendchar=putcharFunc;
}

// функция для передачи байт
char putchar(char c)
{
	if (sendchar) 
		{ 
			sendchar(c);
			return 0;
		}
	else
		return -1;
	//nop();
	
}


#endif
