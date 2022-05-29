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
#ifndef __STM8S_BUTTON_H
#define __STM8S_BUTTON_H

#include "stm8s.h"
#include "inc/stm8s_gpio.h"
typedef enum
{
	idle=0b11, // ожидаение события
	pressdown=0b01, // кнопка нажата
	presslong=0b00, // кнопка долго нажата
	pressup=0b10 // кнопка отпущена
} buttoncode_t;

typedef struct
{
	uint8_t LevelLast:1;  // прошлое состояние линии
	uint8_t Level:1;  // прошлое состояние линии
	buttoncode_t ret:2; // возвращенное функцией значение
	uint8_t PressLongCycleMin; // минимальная задержка для кода удеражания клавиши
	uint8_t CyclePress; // счетчик кол-ва циклов удержания 
} button_t;
/**
  * @brief  Инициализация структуры кнопки
  * @param   Структура хранения состояние кнопки
  * @param   Минимальное кол-во тактов для события удерживание кнопки.
  * @param   Порт кнопки
  * @param   Пин кнопки.
  * @retval None
  */
void ButtonInit(button_t* button,uint8_t PressLongCycleMin, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN);
/**
  * @brief  Чтение результата кнопки
  * @param   Структура хранения состояние кнопки
   * @param   Порт кнопки
  * @param   Пин кнопки.
  * @retval None
*/
buttoncode_t ButtonRead(button_t* button, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN);
#endif