/**
  ******************************************************************************
  * @file    stm8s_cmdline.h
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the cmdline.
   ******************************************************************************
	 
	 

uint16_t cnt_on2=0;
// ���������� � ��������� ����������
int16_t led(uint8_t argc,char *argv[])
{
	if (argc==2)  
	{
		if (strcmp(argv[1],"on")==0) // �������� ���������
		{
			cnt_on2++;
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
			printf("��������� ������� %d\r\n",cnt_on2);
			return 0;
		}
		else 
			if (strcmp(argv[1],"off")==0)  // ��������� ���������
			{
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
				printf("��������� ��������\r\n");
				return 0;
			}
			else
			{
				return 2;
			}
	}
	else
		return 1;
}


uint8_t bl=1; // ���������� ��� ��������� � ���������� �������
// ������� ��� ��������� � ���������� �������
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc==2) 
	{
		if (strcmp(argv[1],"on")==0) // ��������� ���������
			bl=1;
		else 
			if (strcmp(argv[1],"off")==0) // �������� ���������
				bl=0;
		return 0;
	}
	return 1;
}

// ������� ������������ ��������-������� ��������� �� � ���,
// � �� FLESH, �.�. ����� �� ������������� �� ���-�� �������
// � ����� �����
/*
const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};*/
// ������� ����������� �������-����� ��� ������������ ��������
/*
const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};
*/	 
	 
	 //============================ ����� ������ �� UART
	 /*
#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif
	 */
	 
	// ������������� ��������� ������ � main
	/*
	cmdinit('\r', // ����-������
					'\n', // 2 ����-������
					testf, // ��������� �� ������� ��������-�������
					2,  	// ���-�� ������� � �������
					RetText, // ������� �����-������� ������
					2);		// ������ ������� �����-�����*/
	/*				
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// ��������� ������ �������� �� uart2 � ��������� ������
	//UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// ������������� ��� ������ printf
	stdio_InitPrintf(uart2_sendtobuffer);
	UART2_Cmd(ENABLE);
	
	enableInterrupts();
	//printf("Check cmdline\r\n");
	
	*/ 
	 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CMDLINE_H
#define __STM8S_CMDLINE_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_stdioinit.h"
#include <string.h>

// -3 - ������� ����� ����������
#define error_parametr "too many parametr\r\n"
// -2 - ������� ������� ������
#define error_long_string "too long string\r\n"
// -1 - ��� ������� �� ������� �������
#define error_command "command not found\r\n"
// ������ ��������� ������
#define cmdrxnum_max 20
// ������������ ���-�� ����������
#define cmdargc_max 3

// ��������� ������� �����������
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// ��������� ������� ���������� �������� - ������� 
typedef struct
{
	char name[20];	// �������� �������
	func_t func;		// ��������� �� �������
} tablefunc_t;
// ��������� ������� ������������ ����� �������� �������- ����� ������
typedef struct
{
	int16_t retfunc;	// ����� �������� 
	char name[20];		// ����� ��� ������  
} tableRet_t;


typedef enum
{
	receive=0, 	// ����� ������
	exec=0b01,			// ������ ����-����
	error_ovf=0b10, // ������������ ��������� ������
	//error_param=0b100, //������������ ���-�� ����������
	//exec_error_param=0b101, //������ ����-��� � ���� ������������ ����������
	//exec_error_ovf_param=0x111, // ������ ����-���, ������������ ���������� � ������
	exec_error_ovf=0b011 // ������� ����-��� � ���� ������������ ������
	
}cmdstate_t;

// ��� ��������� ��������� ������
typedef struct
{
	char rx[cmdrxnum_max]; // �����
	uint8_t rxnum; // ����� �������� �����
	char stopbit; // ������ ����-��� ������������
	char stopbit2; // ������ ����-��� ��������������
	tablefunc_t *tabfunc; // ��������� �� ������ �������
	uint8_t numfunc;			// ���-�� �������
	cmdstate_t state; // ��������� ��������� ������
	uint8_t argc; // ���-�� ����������
	char *argv[cmdargc_max]; // ������ ���������� �� ��������� 
	tableRet_t *TabRet; //���������-������� �� ������ �������� ��������� �������
	uint8_t NumTabRet; // ������ ������� �������-����� 
}cmdline_t;

// ������������ ��������� ������
void cmdinit(	char stopbit, // ������ ������ ����-���
							char stopbit2, // ������ ������ ����-���
							tablefunc_t tabfunc[], //��������� �� ������ �������
							uint8_t numfunc, // ���-�� �������
							tableRet_t *tabret, //���������-������� �� ������ �������� ��������� �������
							uint8_t NumTabRet// ������ ������� �������-�����
							) ;
// ������� ���� ��� ��������� ������� ������
void cmdmainloop(void);

#endif