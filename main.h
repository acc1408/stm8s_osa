#ifndef __main_H
#define __main_H

#define RotEnGpio 			GPIOD, GPIO_PIN_7 // Включение ШД варщения
#define RotDirGpio			GPIOD, GPIO_PIN_5 // Выбор направления вращения
// #define RotPulseGpio GPIOD, GPIO_PIN_3 // Тактирование вращения
#define LiftEnGpio			GPIOD, GPIO_PIN_0 // Включение ШД подъема
#define LiftDirGpio			GPIOC, GPIO_PIN_6 // Выбор направление подъема
#define LiftPulseGpio 	GPIOC, GPIO_PIN_4 // Тактирование подъема


#define RotLedGpio    	GPIOB, GPIO_PIN_4 // Светодиод вращения
#define LiftLedGpio   	GPIOB, GPIO_PIN_0 // Светодиод подъема

#define RotStartGpio		GPIOB, GPIO_PIN_5	// Кнопка старта вращения
#define RotStopGpio 		GPIOC, GPIO_PIN_2 // Кнопка стоп вращения
#define LiftStopGpio 		GPIOC, GPIO_PIN_1 // Кнопка Стоп подъема
#define LiftStartGpio		GPIOF, GPIO_PIN_4	// Кнопка Старт подъема

#define EncoderGpio 		GPIOB,GPIO_PIN_3,GPIOB,GPIO_PIN_2 // Энкодер
#define RotPauseGpio 		GPIOB, GPIO_PIN_1 // Кнопка экнодера

#endif