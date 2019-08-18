#ifndef __STM8S_ENCODER_C
#define __STM8S_ENCODER_C
#include "inc/stm8s_encoder.h"

void EncoderInit(encoder_t* encod, 
										GPIO_TypeDef* GPIOx_A, GPIO_Pin_TypeDef GPIO_PIN_A,
										GPIO_TypeDef* GPIOx_B, GPIO_Pin_TypeDef GPIO_PIN_B,
										int32_t cnt,int32_t boost_max,uint16_t period_max )
{
	/*
	encod->p1_gpio=p1_Gpio;
	encod->p1_pin=p1_Pin;
	encod->p2_gpio=p2_Gpio;
	encod->p2_pin=p2_Pin;
	*/
	encod->cnt=cnt;
	encod->boost_max=boost_max;
	encod->period=1;
	if (period_max>0)
	{
		encod->period_max=period_max;
		encod->period=period_max;
	}
	else
	{
		encod->period_max=1;
	}
	GPIO_Init(GPIOx_A, GPIO_PIN_A, GPIO_MODE_IN_FL_NO_IT);
	GPIO_Init(GPIOx_B, GPIO_PIN_B, GPIO_MODE_IN_FL_NO_IT);
  encod->p1_last=GPIO_ReadInputPin(GPIOx_A, GPIO_PIN_B);
	encod->p2_last=GPIO_ReadInputPin(GPIOx_B, GPIO_PIN_B);
}


int32_t EncoderRead(encoder_t* encod,
										GPIO_TypeDef* GPIOx_A, GPIO_Pin_TypeDef GPIO_PIN_A,
										GPIO_TypeDef* GPIOx_B, GPIO_Pin_TypeDef GPIO_PIN_B)
{
	int8_t dir; // направление изменения
	int32_t dc; //изменение счетчика
	if (encod->period<encod->period_max) 
	{
		encod->period++;
	}
	// Считываем значения с пинов
	encod->p1_now=GPIO_ReadInputPin(GPIOx_A, GPIO_PIN_A);
	encod->p2_now=GPIO_ReadInputPin(GPIOx_B, GPIO_PIN_B);
	
	// Вычисляем фронт для 1 канала
	if (encod->p1_last!=encod->p1_now) 
		{
			encod->p1_last=encod->p1_now;
			encod->p1_front=1;
			dir=1;
		}
		
	// Вычисляем фронт для 1 канала
	if (encod->p2_last!=encod->p2_now) 
		{
			encod->p2_last=encod->p2_now;
			encod->p2_front=1;
			dir=-1;
		}
		// Отслеживаем щелчок энкодера 
	if ((encod->p1_now==encod->p2_now)&&(encod->p1_front)&&(encod->p2_front) )
		{
			
			encod->p1_front=0;
			encod->p2_front=0;
			dc=dir*encod->boost_max/encod->period;
			encod->period=0;
			if ((dc>0) && (encod->cnt>0))
			{
				encod->cnt+=dc;
				if (encod->cnt<0) { encod->cnt=S32_MAX; }
				return encod->cnt;
			}
			
			if ((dc<0) && (encod->cnt<0))
			{
				encod->cnt+=dc;
				if (encod->cnt>0) { encod->cnt=S32_MIN; }
				return encod->cnt;
			}
		}
		else 
		{
		dc=0;
		}
	encod->cnt+=dc;	
	return encod->cnt;
}
void EncoderSetCount(encoder_t* encod,int32_t cnt)
{
	encod->cnt=cnt;
}

#endif