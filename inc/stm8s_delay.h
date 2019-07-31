/**
  ******************************************************************************
  * @file    stm8s_delay.h
  * @author  Salnikov Aleksey
  * @version V1.0.0
  * @date    31-07-2019
  * @brief   This file contains all functions prototype and macros for the program Delay.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_DELAY_H
#define __STM8S_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

void Init_Delay(void);
void delay_us(uint16_t ticks);
void delay_ms(uint16_t ticks);
#endif 