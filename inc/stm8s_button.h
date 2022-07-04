/*
������ ������ � �������
//������������� ��������� ������
button_t btn;
// ������������ ���������� ������� ������
buttoncode_t ret;
// ����� ������������� ������
ButtonInit(&btn,100,	GPIOB, GPIO_PIN_0);
//������ ���������� ������
ret=ButtonRead(&btn,GPIOB, GPIO_PIN_0);
*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_BUTTON_H
#define __STM8S_BUTTON_H

#include "stm8s.h"
#include "inc/stm8s_gpio.h"
typedef enum
{
	idle=0b11, // ��������� �������
	pressdown=0b01, // ������ ������
	presslong=0b00, // ������ ����� ������
	pressup=0b10 // ������ ��������
} buttoncode_t;

typedef struct
{
	uint8_t LevelLast:1;  // ������� ��������� �����
	uint8_t Level:1;  // ������� ��������� �����
	buttoncode_t ret:2; // ������������ �������� ��������
	uint8_t PressLongCycleMin; // ����������� �������� ��� ���� ���������� �������
	uint8_t CyclePress; // ������� ���-�� ������ ��������� 
} button_t;
/**
  * @brief  ������������� ��������� ������
  * @param   ��������� �������� ��������� ������
  * @param   ����������� ���-�� ������ ��� ������� ����������� ������.
  * @param   ���� ������
  * @param   ��� ������.
  * @retval None
  */
void ButtonInit(button_t* button,uint8_t PressLongCycleMin, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN);
/**
  * @brief  ������ ���������� ������
  * @param   ��������� �������� ��������� ������
   * @param   ���� ������
  * @param   ��� ������.
  * @retval None
*/
buttoncode_t ButtonRead(button_t* button, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN);
#endif