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

//
/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
uint8_t a[]={0x00,0x45,0x36,0x19,0x02};

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
//char simvol;		
//============================ ����� ������ �� UART
#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif





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


uint16_t nmb=0;
//-----

i2cStatus_t res;

typedef struct
{
	uint8_t* Map;
	uint8_t sizeMap;
	uint8_t SlaveCnt;
} I2CSlave_t;
I2CSlave_t I2CSlave;

void I2C_InitSlaveIT(uint8_t address,uint8_t genCall, void* Map, uint8_t sizeMap, void(*HandlerSlaveI2C)(void))
{
	I2CSlave.Map=(uint8_t*)Map;
	I2CSlave.sizeMap=sizeMap;
	I2CSlave.SlaveCnt=0;
	I2C->gencall=genCall;
	I2C->add71=address;
	I2C->ack=1;
	I2C->iterren=1;
	I2C->itevten=1;
	I2C->itbufen=1;
	I2C->pe=1;
}
//uint8_t SlaveCnt=0;
void I2C_SlaveIT(void)
{
	i2cStatusReg_t SR;
	I2CSlave.SlaveCnt%=I2CSlave.sizeMap;
	SR.SR1=I2C->SR1;
	SR.SR3=I2C->SR3;
	SR.SR2=I2C->SR2;
	if (SR.SR2)
		{
			return;
		}
	if (SR.msl)
		{
			// master
		}
		else
		{
			// slave
			if (SR.addr)
			{
				return;
			}
			if (SR.rxne)
			{
				I2CSlave.Map[I2CSlave.SlaveCnt++]=I2C->DR;
				return;
			}
			if (SR.rxne)
			{
				I2CSlave.Map[I2CSlave.SlaveCnt++]=I2C->DR;
				return;
			}
			
		}
}


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

	// ������������� ��������� ������.
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
	*/
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	//printf("Check cmdline\r\n");
	
	//I2C_Init_7bit(100000);
	//I2C_MasterSend(0x57, a, 1);
	
	
	while (1)
  {
		//cmdmainloop(); // ��������� ��������� ������
	//	printf("qwe5675089tr %d\r\n",nmb);
	//res=I2C_MasterSend(0x68, a, 2);
	//res=I2C_MasterReceive(0x68, &a[1],1);
	//res=I2C_MasterSend(0x68, a, 2);
	//	res=I2C_MasterSendPtrSendData(0x68, a, 1, &a[1],0);
	//res=I2C_MasterSendPtrReceiveData(0x68, a, 1, &a[1],0);
	//res=I2C_MasterSendPtrReceiveData(0x68, a, 1, &a[1],1);
	//res=I2C_MasterSendPtrReceiveData(0x68, a, 1, &a[1],2);
	res=I2C_MasterSendPtrReceiveData(0x68, a, 1, &a[1],3);
	nmb++;
	//if (bl) 
	GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(50);
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
