
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_BUTTON_C
#define __STM8S_BUTTON_C
#include "inc/stm8s_button.h"

void ButtonInit(button_t* button,uint8_t PressLongCycleMin, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN)
{
	GPIO_Init(GPIOx, GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
	button->PressLongCycleMin=PressLongCycleMin;
	button->LevelLast=GPIO_ReadInputPin(GPIOx,GPIO_PIN);;
	button->CyclePress=0;
}

buttoncode_t ButtonRead(button_t* button, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN)
{
	uint8_t code;
	button->Level=GPIO_ReadInputPin(GPIOx,GPIO_PIN);
	// определяем код сигнала
	code=(button->Level<<1)|(button->LevelLast); 
	switch(code)
	{
		case idle:
			button->ret=idle;
			break;
		case pressdown:
			button->ret=pressdown;
			break;
		case pressup:
			button->CyclePress=0;
			button->ret=pressup;
			break;
		case presslong:
			if (button->CyclePress==button->PressLongCycleMin)
			{
				button->ret=presslong;
			}
			else
			{
				button->CyclePress+=1;
				button->ret=idle;
			}
			break;
	}
	button->LevelLast=button->Level;
	return button->ret;
}
#endif