/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
#include  <stdio.h>
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
//void cmdreset(void);
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
/*
SetLCD_t lcd1;

 char st[40]="Test";

bme280_t bm;
uint32_t pres;
uint32_t hum;
int32_t t;
uint8_t rez;
*/
/*
char st[50];
char st_tx[100]; //  ����� ��� ��������
static uint8_t num_st=0;
unsigned short kol_tx=0; // ������� ������ ��������
unsigned char tx_num=0; // ����� ���� ��� ��������
float tx_float=0; // ������ �������� ���������� �����
bool tx_start=0; // � ��������� ������ �������� ������ ���
char st_tx2[10];

void putchar_st(char c)
{
	st[num_st] = c;
	num_st = (num_st+1) % 50;
}
*/
// ���������� ���������� �������� ������
// ������ ������� ���������� � stm8s_it.c
/*
void handler_tx(void)
{
	UART2_SendData8(st_tx[tx_num]); // �������� ������ � ��������� �������
	tx_num++; // ����������� �������
	if (st_tx[tx_num]=='\0') { 
														UART2_ITConfig(UART2_IT_TXE, DISABLE); // ���������  ���������� ���� ��������� ���� ����� ������
														tx_num=0; // �������� ������� ��� ����������� ������
														tx_start=0; // ���������� ���� ��������,
														// ������ ������ � ������ �� �����
														}
}
*/


// ��������� ������� �����������
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// ��������� ������� ���������� �������� - ������� 
typedef struct
{
	char name[20];	// �������� �������
	func_t func;		// ��������� �� �������
} tablefunc_t;

// ������ ��������� ������
#define cmdrxnum_max 40
// ������������ ���-�� ����������
#define cmdargc_max 3
typedef enum
{
	receive=0, 	// ����� ������
	exec=0b01,			// ������ ����-����
	error_ovf=0b10, // ������������ ��������� ������
	exec_error_ovf=0b11 // ������� ����-��� � ���� ������������ ������
}cmdstate_t;

// ��� ��������� ��������� ������
typedef struct
{
	char rx[cmdrxnum_max]; // �����
	uint8_t rxnum; // ����� �������� �����
	char stopbit; // ����-���
	tablefunc_t *tabfunc; // ��������� �� ������ �������
	uint8_t numfunc;			// ���-�� �������
	cmdstate_t state; // ��������� ��������� ������
	uint8_t argc; // ���-�� ����������
	char *argv[cmdargc_max]; // ������ ���������� �� ��������� 
	//char stpstr[2];
}cmdline_t;

//====================��������� �����
// ���������� ������ 
#ifndef CRITICAL_SECTION_START
char cc_reg; // ������� ��� �������� �������� ���������
#define CRITICAL_SECTION_START _asm("push CC \n	pop _cc_reg \n"); disableInterrupts()
#define CRITICAL_SECTION_END	 _asm("push _cc_reg \n	pop CC \n")
#endif

//! cBuffer structure
typedef struct struct_cBuffer
{
	uint8_t  *dataptr;		/// ��������� �� �������� �����
	uint16_t size;				/// ������ ������ ������
	uint16_t datalength;	/// ������ �������� ������ ������
	uint16_t dataindex;		/// ������ ������ ������
} cBuffer_t;

// ������������� ������ 
void bufferInit(cBuffer_t* buffer, uint8_t *start, uint16_t size)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// set start pointer of the buffer
	buffer->dataptr = start; // ���������� ���������
	buffer->size = size;	// ���������� �������
	// initialize index and length
	buffer->dataindex = 0;
	buffer->datalength = 0;
	// end critical section
	CRITICAL_SECTION_END;
}

// �������� ������ � ����� 
uint8_t bufferAddToEnd(cBuffer_t* buffer, uint8_t data)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// make sure the buffer has room
	if(buffer->datalength < buffer->size)
	{
		// save data byte at end of buffer
		buffer->dataptr[(buffer->dataindex + buffer->datalength) % buffer->size] = data;
		// increment the length
		buffer->datalength++;
		// end critical section
		CRITICAL_SECTION_END;
		// return success
		return -1;
	}
	// end critical section
	CRITICAL_SECTION_END;
	// return failure
	return 0;
}


// �������� ������ �� ������
uint8_t  bufferGetFromFront(cBuffer_t* buffer)
{
	uint8_t data = 0;
	// begin critical section
	CRITICAL_SECTION_START;
	// check to see if there's data in the buffer
	if(buffer->datalength)
	{
		// get the first character from buffer
		data = buffer->dataptr[buffer->dataindex];
		// move index down and decrement length
		buffer->dataindex++;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength--;
	}
	// end critical section
	CRITICAL_SECTION_END;
	// return
	return data;
}

// �������� ������ � ������
void bufferFlush(cBuffer_t* buffer)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// flush contents of the buffer
	buffer->datalength = 0;
	// end critical section
	CRITICAL_SECTION_END;
}

// �������� ������� ����� ������ ��������
uint8_t bufferIsNotFull(cBuffer_t* buffer)
{
	uint16_t bytesleft;
	// begin critical section
	CRITICAL_SECTION_START;
	// check to see if the buffer has room
	// return true if there is room
	bytesleft = (buffer->size - buffer->datalength);
	// end critical section
	CRITICAL_SECTION_END;
	return bytesleft;
}

// ���������� ��������� numbytes ����� � ������ �� ������
void bufferDumpFromFront(cBuffer_t* buffer, uint16_t numbytes)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// dump numbytes from the front of the buffer
	// are we dumping less than the entire buffer?
	if(numbytes < buffer->datalength)
	{
		// move index down by numbytes and decrement length by numbytes
		buffer->dataindex += numbytes;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength -= numbytes;
	}
	else
	{
		// flush the whole buffer
		buffer->datalength = 0;
	}
	// end critical section
	CRITICAL_SECTION_END;
}

//==================��������� ������===================
cmdline_t cmd; // ��������� ��������� ������
// ������������ ��������� ������
void cmdinit(	char stopbit, // ������ ������ ����-���
							tablefunc_t tabfunc[], //��������� �� ������ �������
							uint8_t numfunc) // ���-�� �������
{
	//uint8_t i=0;
	cmd.stopbit=stopbit; // ����-���
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
	//while(*st && i<2)
	//cmd.stpstr[i++]=*st++;
}

void cmdinputchar(char c)
{
	switch(cmd.state)
	{
		// ����� ������
		case receive:
				
				// ��������� ����� � ������
				if (cmd.rxnum<cmdrxnum_max) 
					cmd.rx[cmd.rxnum++]=c; // ��������� ����� 
				else 	
					{
					cmd.state=error_ovf; // ��������� ������ ������������ ������
					}
				//break;
		// ���� ��������� ������������ ������� ����-����
		case error_ovf:
				if (c==cmd.stopbit) // ���� ������� ����-���, 
						cmd.state|=exec; // ������ ��� �� ���������
				break;
		default:
					nop();
				break;
	}
}
// ������ �� �����, ������ �������� ��������� ����� ������
// � ������ ������� ��������� �� ������ ����
void cmdparcer(void)
{
	// �������� ��� ����� �������� �� ����� ������
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // ���� ������ ������
			cmd.rx[i]='\0'; // �� �������� ��� �� ����� ������
	cmd.rx[cmd.rxnum-1]='\0';
	// ���������� ������ �����
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx;
		}
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0')&&(cmd.argc<cmdargc_max))
			cmd.argv[cmd.argc++]=cmd.rx+i;
	nop();
}

// ����� ��������� �������� ��������� ������
void cmdreset(void)
{
	cmd.rxnum=0;
	cmd.state=receive;
	cmd.argc=0;
}

// ����� ������� ��� ����������
void cmdexec(void)
{
	uint8_t i;
	char *funcfind; // ��������� �� ������
	func_t funcexec;// ��������� �� �������
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // ��������� �� ������ ������
		funcexec=cmd.tabfunc[i].func; // ��������� �� ��������������� ������
		if (strcmp(cmd.argv[0],funcfind)==0) 
		{
			nop();
			funcexec(cmd.argc,cmd.argv);
			break; // ���� ������� �������, �� ������� �� ����� ������ 
		}
	}	
		cmdreset(); // ����� �������� ��� ����� ��������
}



// ������� ���� ��� ��������� ������� ������
void cmdmainloop(void)
{
	switch(cmd.state) // ��������� ��������� ������
		{
			case exec: // 
					cmdparcer(); 
					cmdexec();
				break;
			case 	exec_error_ovf:
					cmdreset();
				break;
		}
}

char st2[40]="test string"; // ������ ��� �������� ������

// ���������� � ��������� ����������
int16_t led(uint8_t argc,char *argv[])
{
	if (argc=2)  
	{
		if (strcmp(argv[1],"on")==0) // �������� ���������
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
		else 
			if (strcmp(argv[1],"off")==0)  // ��������� ���������
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
		return 0;
	}
	else
		return 1;
}


uint8_t bl=1; // ���������� ��� ��������� � ���������� �������
// ������� ��� ��������� � ���������� �������
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc=2) 
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
const tablefunc_t testf[2]={"led", led,
														"blink", blink	
														};
char simvol;														
char putchar(char c)
{
	nop();
	simvol=c;
	nop();
	return 0;
}


//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
	}
}
#endif

cBuffer_t buf;
uint8_t mas[30];

void main(void)
{
 #ifdef  __OSA__
 // ����������� ������� ������������ �� 16���
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // ������������� RTOS OSA
	TIM4_TimerOSA(1000); //����������� ���������� 500���
	OS_Task_Create(7, Task); // ������� ������
	OS_EI();   // ��������� ��� ����������
	OS_Run(); // ������ ���� RTOS OSA
#else
	/* Infinite loop */
//	cf_u=&cf;
	uint8_t i;
	Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  //testf={.name="nvvjhggvn",.func=yui};
	cmdinit('\r', // ����-������
					testf, // ��������� �� ������� ��������-�������
					2 ); 	// ���-�� ������� � �������
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
		/*
		#pragma asm
			push CC
			pop _cf
		#pragma endasm
		*/
	enableInterrupts();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	bufferInit(&buf, mas, 30);
	/*
	#pragma asm
		push _cf
		pop CC
	#pragma endasm
		*/
	while (1)
  {
		cmdmainloop(); // ��������� ��������� ������
		printf("qwetr %d",0x54);
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(1000);
	}
#endif
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
