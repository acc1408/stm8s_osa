/**
  ******************************************************************************
  * @file    stm8s_delay.h
  * @author  Salnikov Aleksey
  * @version V1.0.0
  * @date    31-07-2019
  * @brief   This file contains all functions prototype and macros for the program Delay.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_I2CLCD1602_H
#define __STM8S_I2CLCD1602_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_delay.h"
#include "inc/stm8s_i2c.h"

typedef struct
{
		uint8_t rs:1;
		uint8_t rw:1;
		uint8_t e:1;
		uint8_t bl:1; //backlight
		uint8_t db4:4;
}LcdData_t;

typedef enum
{
	LcdFont5x8=0,
	LcdFont5x1=1
}LcdFont_t;

typedef enum
{
	LcdLine1=0,
	LcdLine2=1
}LcdLine_t;

typedef enum
{
	LcdSubOne=0,
	LcdAddOne=1
} LcdInc_t;

typedef enum
{
	LcdCom=0,
	LcdData=1
} LcdDataCom_t;


typedef struct														//	*
{															//	*
// ��������� ���������� �������� ������� ��� ������
LcdInc_t	ID_cursorShiftRightLeft:1;// ���������� �������/���������� ��������
FunctionalState S_ScreenShiftOnOff:1;// ����� ������ ��� ������ ��������=1/ ��������=0*
//--------------------------------------------------------------*
//��������� ����� � �������
FunctionalState B_BlinkOnOff:	 1; 	// ������� ������� ���=1/����=0	*
FunctionalState C_CursorOnOff: 1; 	// ������-������������� �����=1/ �������=0*
FunctionalState D_DisplayOnOff:1; 	// ������� �������=1/��������=0 *
//--------------------------------------------------------------*
// ��������� ������� ����� � ����� ��� ��������� DDRAM 
// uint8_t SC_shiftScreenCursor:1;
// uint8_t RL_RightLeft:1;
//--------------------
LcdFont_t F_font:		1;	// ������: 5*7px=0/5*10px=1					*
LcdLine_t N_lines:	1;	// ���-�� �����: ������1=0/�����2=1		*
//uint8_t bit:	1;	// �������� �������� 4���=0 8���=1
FunctionalState Backlight:1;
uint8_t Address:	7;  // ����� ����
union{
	uint8_t data[4];
	LcdData_t bit[4];
};
}SetLCD_t;				

void Lcdi2cInit(SetLCD_t *lcd, uint8_t Address);
void LcdSendByte(SetLCD_t *lcd, LcdDataCom_t data1_com0,uint8_t data);
void Lcdi2cPrint(SetLCD_t *lcd, char *st);
void LcdWriteUserChar(SetLCD_t *lcd, uint8_t kod, uint8_t *st);
// ���������� ����������� ����� ������ �� 1 ������ � ����� ������� 1 ������
// ������� ������

#endif 