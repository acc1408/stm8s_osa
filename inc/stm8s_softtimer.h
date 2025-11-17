/*
Пример работы с кнопкой
//Инициализация структуры кнопки
button_t btn;
// Инциализация результата функции чтения
buttoncode_t ret;
// Вызов инициализации кнопки
ButtonInit(&btn,100,	GPIOB, GPIO_PIN_0);
//Чтение результата кнопки
ret=ButtonRead(&btn,GPIOB, GPIO_PIN_0);
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_SOFTTIMER_H
#define __STM8S_SOFTTIMER_H

#include "stm8s.h"


//----------------------------------------
// Струтура программного таймера
typedef void(*softTimer_Handler_t)(void);


typedef struct
{
	uint16_t timer;
	uint16_t autoreload;
	softTimer_Handler_t handler; // обработчик переполнения
	FunctionalState state;			// включение таймера
} softTimer_t;





//------------------

void SoftTimer_Init(softTimer_t* 	softTimer,// указатель на работу таймера
							uint16_t 			autoreload,			// значение автообновления
							softTimer_Handler_t handler		// обработчик переполнения
							);
// включение таймера							
void SoftTimer_CMD(softTimer_t* 	softTimer,
									FunctionalState state);
// получение значения таймера
uint16_t SoftTimer_getTimer(softTimer_t* 	softTimer);
// вызов функции для нового тика
uint16_t SoftTimer_ClockISR(softTimer_t* 	softTimer);

FunctionalState SoftTimer_getState (softTimer_t* 	softTimer);

//-------------------------------
//--------------------------------------------
//=================================================
//=================================================
typedef void(*softPWM_TimEquCmp_Handler_t)(void);


typedef enum
{
	softPWM2_noEqu,
	softPWM2_Equ
}softPWM2_equ_t;



typedef enum
{
	pwmFlagLowHigh=0,
	pwmFlagHighLow
} pwmFlagMode_t;

typedef enum
{
		pwmFlagLow=0,
		pwmFlagHigh=!pwmFlagLow
} pwmFlag_t;

typedef struct
{
	uint16_t cmp;	// регистра сравнения
	pwmFlagMode_t pwmFlagMode:1;
	pwmFlag_t flag:1; // флаг выхода
	softPWM2_equ_t equ:1; // флаг равенства 
	
	softPWM_TimEquCmp_Handler_t handler; // вызов функции при равенсте таймера и 
	
} softPWM2_t;




pwmFlag_t SoftPWM2_Init(	softPWM2_t* softPWM,
										uint16_t timer,
										uint16_t cmp,	
										pwmFlagMode_t pwmFlagMode,
										softPWM_TimEquCmp_Handler_t handler
										);

pwmFlag_t SoftPWM2_compare(	softPWM2_t* softPWM,
														uint16_t timer);



#endif