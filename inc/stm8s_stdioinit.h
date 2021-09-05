/**
  ******************************************************************************
  * @file    stm8s_stdioinit.h
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the init printf.
   ******************************************************************************/
/*
Пример настройки проект для использования функции printf

void main(void)
{
	
	stdio_InitPrintf(uart2_sendtobuffer);
	
	while(1)
	{
		
	}
}


*/
	 
	 
#ifndef __STM8S_STDIOINIT_H
#define __STM8S_STDIOINIT_H
#include "stm8s.h"
#include <stdio.h>

#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif

// инициализация функции для передачи байт для функции printf
void stdio_InitPrintf(void (*putcharFunc)(char));


#endif

	