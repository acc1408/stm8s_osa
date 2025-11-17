
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_SOFTTIMER_C
#define __STM8S_SOFTTIMER_C
#include "inc/stm8s_softtimer.h"



void SoftTimer_Init(softTimer_t* 	softTimer,// указатель на работу таймера
							uint16_t 			autoreload,			// значение автообновления
							softTimer_Handler_t handler		// обработчик переполнения						
							)
{
	FunctionalState temp = softTimer->state;
	softTimer->state=DISABLE;
	
	softTimer->timer=0;
	softTimer->autoreload=autoreload;
	softTimer->handler=handler;
	softTimer->state=temp;
}

void SoftTimer_CMD(softTimer_t* 	softTimer,
									FunctionalState state)
{
	softTimer->state=state;
}


uint16_t SoftTimer_getTimer(softTimer_t* 	softTimer)
{
	return softTimer->timer;
}


// вызов функции вызывает такт таймера
// возврат значение счетчика
uint16_t SoftTimer_ClockISR(softTimer_t* 	softTimer)
{
	if (softTimer->state == ENABLE )
	{
		if (softTimer->timer >=	softTimer->autoreload)
		{
			if ( softTimer->handler )
			{
				softTimer->handler();
			}
			softTimer->timer = 0;
		}
		else
		{
			softTimer->timer++;
		}
	}
	return softTimer->timer;
}

FunctionalState SoftTimer_getState (softTimer_t* 	softTimer)
{
	return softTimer->state;
}

//-----------------------------------------------------
//==========================================

pwmFlag_t SoftPWM2_compare(	softPWM2_t* softPWM,
										uint16_t timer)
{
	if (timer <softPWM->cmp)
	{
		softPWM->equ = softPWM2_noEqu;
		if (softPWM->pwmFlagMode == pwmFlagLowHigh)
		{
			softPWM->flag=pwmFlagLow;
		}
		else
		{
			softPWM->flag=pwmFlagHigh;
		}
	}
	else
	if (timer >softPWM->cmp)
	{
		softPWM->equ = softPWM2_noEqu;
		LBL_SoftPWM_equ:
		if (softPWM->pwmFlagMode == pwmFlagLowHigh)
		{
			softPWM->flag=pwmFlagHigh;
		}
		else
		{
			softPWM->flag=pwmFlagLow;
		}
	}
	else
	{
		// значения равны
		
		if (softPWM->equ == softPWM2_noEqu)
		{
			if (softPWM->handler)
			{
				softPWM->handler();
			}
		}
	
		softPWM->equ=softPWM2_Equ;
		goto LBL_SoftPWM_equ;

	}
	
	return softPWM -> flag;
}


pwmFlag_t SoftPWM2_Init(	softPWM2_t* softPWM,
										uint16_t timer,
										uint16_t cmp,	
										pwmFlagMode_t pwmFlagMode,
										softPWM_TimEquCmp_Handler_t handler
										)
{
	softPWM->equ=softPWM2_noEqu;
	softPWM->handler=handler;
	softPWM->pwmFlagMode=pwmFlagMode;
	softPWM->cmp=cmp;
	
	if (timer <softPWM->cmp)
	{
		if (softPWM->pwmFlagMode == pwmFlagLowHigh)
		{
			softPWM->flag=pwmFlagLow;
		}
		else
		{
			softPWM->flag=pwmFlagHigh;
		}
	}
	else
	{
		if (softPWM->pwmFlagMode == pwmFlagLowHigh)
		{
			softPWM->flag=pwmFlagHigh;
		}
		else
		{
			softPWM->flag=pwmFlagLow;
		}
	}
	
	return softPWM -> flag;
}


#endif