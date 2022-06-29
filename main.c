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
  * You may obtain a copy of the License at:
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

#define lineEnGpio 	GPIOD, GPIO_PIN_2
#define lineClkGpio GPIOD, GPIO_PIN_3
#define lineDirGpio GPIOD, GPIO_PIN_4
#define btUpGpio 		GPIOB, GPIO_PIN_0 //������
#define btDownGpio 	GPIOB, GPIO_PIN_1 // �����

#define rotEnGpio		GPIOC, GPIO_PIN_3
#define rotClkGpio 	GPIOC, GPIO_PIN_4
#define rotDirGpio 	GPIOC, GPIO_PIN_5
#define btIncGpio 	GPIOB, GPIO_PIN_2 // �����
#define btDecGpio 	GPIOB, GPIO_PIN_3 // �������

#define btCangaGpio GPIOD, GPIO_PIN_7 // �������
#define ledCangaGpio GPIOC, GPIO_PIN_2 // �������


#define lineStepMax 6400
int32_t lineStep=0;
//uint32_t *lineStepPt=&lineStep;
#define rotStepMax 3200
uint16_t rotStep=0;
int16_t length=0;
button_t btInc,btDec,btUp,btDown,btCanga;
uint16_t cell=0; // ����� ���������, ���� 0 - ������
uint16_t nmb=0,i;
uint8_t cim=0;
SetLCD_t lcd1;
char st[40];

uint8_t rotDir=0, lineDir=0; // ����������� ��������
uint8_t rotEn=0,lineEn=0; // ���� ������ ���������

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************


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

const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};
// ������� ����������� �������-����� ��� ������������ ��������

const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};

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





//====================================

void privodlineTim2(void)
{
	
	GPIO_WriteReverse(lineClkGpio);
	
	if (lineDir)
	{
		if (lineStep<lineStepMax)
		{
			
			lineStep++;
			
		}
		else
		{
			TIM2_Cmd(DISABLE);
			GPIO_WriteLow(lineEnGpio);
		}
		
	}
	else
	{
		if (lineStep>0)
		{
			lineStep--;
		}
		else
		{
			TIM2_Cmd(DISABLE);
			GPIO_WriteLow(lineEnGpio);
	//		lineEn=0;
		}
	}
	
	TIM2_ClearFlag(TIM2_FLAG_UPDATE);
}
	
//====================================
// ������� ��� �������� ���������� ����������
/*
#define kolShag2 3200 //4076
	uint8_t shag2=0;
	uint16_t oborot2=kolShag2; //
	uint8_t dir2;
	uint8_t en2;	
	*/
void baraban_tim3 (void)
{
	GPIO_WriteReverse(rotClkGpio);
	
	if (rotStep<rotStepMax) 
		{
			rotStep++;
		}
	else
		{
			GPIO_WriteLow(rotEnGpio);
			TIM3_Cmd(DISABLE);
			rotStep=0;
		}
	
	
	TIM3_ClearFlag(TIM3_FLAG_UPDATE);
	
}
//====================================

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
	
	cmdinit('\r', // ����-������
					'\n', // 2 ����-������
					testf, // ��������� �� ������� ��������-�������
					2,  	// ���-�� ������� � �������
					RetText, // ������� �����-������� ������
					2);		// ������ ������� �����-�����
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// ��������� ������ �������� �� uart2 � ��������� ������
	UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// ������������� ��� ������ printf
	stdio_InitPrintf(uart2_sendtobuffer);
	//UART2_Cmd(ENABLE);
	I2C_Init_7bit(100000);
	enableInterrupts();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	printf("Check cmdline\r\n");
	//--------------
	
	Lcdi2cInit(&lcd1, 0b0111111, 
								ENABLE, // Backlight
								ENABLE, // Blink cursor
								ENABLE); // On/off cursor

	//delay_ms(5);							
	//LcdClearDisplay(&lcd1);
	//delay_ms(5);	
	
//----------
	CursorGoTo(&lcd1, 0, 0);
	sprintf(st," +  up");
	Lcdi2cPrint(&lcd1, st);
	//-----------
	CursorGoTo(&lcd1, 1, 0);
	sprintf(st,"<%d>select    ",cell);
	Lcdi2cPrint(&lcd1, st);
	//-----------
	CursorGoTo(&lcd1, 2, 0);
	sprintf(st," - down",cell);
	Lcdi2cPrint(&lcd1, st);
	
	//-------------------

	
	ButtonInit(&btInc,10, btIncGpio);
	ButtonInit(&btDec,10, btDecGpio);
	ButtonInit(&btUp,10, btUpGpio);
	ButtonInit(&btDown,10, btDownGpio);
	ButtonInit(&btCanga,10, btCangaGpio);
	
	//--------------
	
	
	GPIO_Init(lineClkGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(lineEnGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(lineDirGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(rotClkGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(rotEnGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(rotDirGpio, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(btCangaGpio, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(ledCangaGpio, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	

	
	//TIM4_TimeBaseInit(TIM4_PRESCALER_16, 125);
	//TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM3_TimeBaseInit(TIM3_PRESCALER_16, 200);
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_16, 200);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	//TIM3_Cmd(ENABLE);
	//TIM4_Cmd(ENABLE);
	
	while (1)
  {
		cmdmainloop(); // ��������� ��������� ������
	//	printf("qwe5675089tr %d\r\n",nmb);
		//nmb=65;
		//cim=65;
		//sprintf(st,"<>+-");
		//Lcdi2cPrint(&lcd1, st);
	
	// �������� ��������
	if (ButtonRead(&btInc, btIncGpio)==pressup )
	{
		// �������� �������
		GPIO_WriteHigh(rotEnGpio);
		// ����������� ��������
		GPIO_WriteHigh(rotDirGpio);
		
		if (rotStep )
		{
			// ��������� ������
			if (rotDir==0) // ��������� ��� �� ����� � ������ �������
			{
				rotDir=1;
				rotStep=rotStepMax-rotStep;
			}	
		}
		else
		{
			//��������� �����
			rotDir=1;
			if (cell==3)
			{
				cell=0;
			}
			else
			{
				cell++;
			}
				CursorGoTo(&lcd1, 1, 0);
				sprintf(st,"<%d>wait    ",cell);
				Lcdi2cPrint(&lcd1, st);
			//	dir=0;
			//	en=1;
			TIM3_Cmd(ENABLE);
		}
		
	}
	if (ButtonRead(&btDec, btDecGpio)==pressup )
	{
		// �������� �������
		GPIO_WriteHigh(rotEnGpio);
		// ����������� ��������
		GPIO_WriteLow(rotDirGpio);
		
		if (rotStep )
		{
			// ��������� ������
			if (rotDir==1) // ��������� ��� �� ����� � ������ �������
			{
				rotDir=0;
				rotStep=rotStepMax-rotStep;
			}	
		}
		else
		{
			//��������� �����
			rotDir=0;
			if (cell==0)
			{
				cell=3;
			}
			else
			{
				cell--;
			}
				CursorGoTo(&lcd1, 1, 0);
				sprintf(st,"<%d>wait    ",cell);
				Lcdi2cPrint(&lcd1, st);
			//		dir=0;
			//		en=1;
			TIM3_Cmd(ENABLE);
		}
	}
	
	//==============
	// �������� ������
	if (ButtonRead(&btUp, btUpGpio)==pressup )
	{
		
		
			if (lineStep<lineStepMax)
		{
			// �������� �������
			GPIO_WriteHigh(lineEnGpio);
			// ����������� ��������
			GPIO_WriteHigh(lineDirGpio);
			lineDir=1;
			TIM2_Cmd(ENABLE);
		}
	}
	if (ButtonRead(&btDown, btDownGpio)==pressup )
	{
		
		
		if (lineStep>0)
		{
		// �������� �������
			GPIO_WriteHigh(lineEnGpio);
			// ����������� ��������
			GPIO_WriteLow(lineDirGpio);
			lineDir=0;
			TIM2_Cmd(ENABLE);
		}
	}
	if (lineStep<0) lineStep=0;
	length=lineStep/320;
				CursorGoTo(&lcd1, 3, 0);
				sprintf(st,"H=%d mm    ",length);
				Lcdi2cPrint(&lcd1, st);
	
	if (rotStep==0)
		{
				CursorGoTo(&lcd1, 1, 0);
				sprintf(st,"<%d>select    ",cell);
				Lcdi2cPrint(&lcd1, st);
		}
	
	if (GPIO_ReadInputPin(btCangaGpio))
	{
		GPIO_WriteLow(ledCangaGpio);
	}
	else
	{
		GPIO_WriteHigh(ledCangaGpio);
	}
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(100);
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
