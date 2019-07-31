/**
  ******************************************************************************
  * @file    stm8s_delay.h
  * @author  Salnikov Aleksey
  * @version V1.0.0
  * @date    31-07-2019
  * @brief   This file contains all functions prototype and macros for the program Delay.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_DELAY_C
#define __STM8S_DELAY_C

/* Includes ------------------------------------------------------------------*/
#include "inc/stm8s_delay.h"


uint8_t delay_us_pred=0,delay_us_post=0; // предделитель таймера

void Init_Delay(void)
{
	uint32_t f_cpu=0;
	uint8_t koef;
	delay_us_pred=0;
	delay_us_post=0;
	f_cpu=CLK_GetCPUClk();
	koef=f_cpu/4000000UL; 
	while(koef>1)
	{
		koef/=2;
		delay_us_pred++;
	}
	if (koef==0)
	{
	koef=4000000UL/f_cpu; 
	while(koef>1)
	{
		koef/=2;
		delay_us_post++;
	}
	}
}
//--------------
void delay_us( uint16_t ticks )
{
	uint16_t t1000=0x3FF,k2;
	uint32_t k=0;
k=ticks;
k<<=delay_us_pred;
k>>=delay_us_post;
k2=k;
if (k2>30) 
	{
	k2-=25;
	_asm("$N:\n decw X\n nop \n jrne $L\n ", k2);
	}
//---------------------------------------
	/*
	t1000<<=pred;
	t1000>>=post;
	k=ticks>>9;
	for(i=0;i<k;i++)
	_asm("$N:\n decw X\n nop \n jrne $L\n ", t1000);
*/
}
//---------------------------
void delay_ms(uint16_t ticks )
{
	while(ticks--)
	delay_us(1000);
}
//-----------------------
#endif 