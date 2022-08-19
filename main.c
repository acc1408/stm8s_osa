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
#define enStep  GPIOD, GPIO_PIN_5
#define clkStep GPIOD, GPIO_PIN_0
#define dirStep GPIOC, GPIO_PIN_5
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
uint8_t a[]={0x01,0x04,0x02,0x03,0x02};

//char simvol;		






//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
	}
}
#endif


uint16_t nmb=0,adc0;
uint32_t clks;
//-----

i2cStatus_t res;

FlagStatus adcStatus;


void clockStep(void)
{
	GPIO_WriteReverse(clkStep);
	//TIM2_SetAutoreload(10+190*adc0/510);
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(1000); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Task); // создаем задачу
	OS_EI();   // Разрешить все прерывания
	OS_Run(); // Запуск ядра RTOS OSA
#else
	/* Infinite loop */
//	cf_u=&cf;
	uint8_t i;
	Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	GPIO_Init(enStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(dirStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
               ADC1_CHANNEL_0,
               ADC1_PRESSEL_FCPU_D2, 
               ADC1_EXTTRIG_TIM, 
               DISABLE, // Trig
							 ADC1_ALIGN_RIGHT, 
               ADC1_SCHMITTTRIG_CHANNEL0, 
               DISABLE);
	ADC1_StartConversion();
	TIM2_TimeBaseInit(TIM2_PRESCALER_128, 10);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	//TIM2_Cmd(ENABLE);
	enableInterrupts()
	while (1)
  {
		
	nmb++;
	//if (bl) 
	//GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	//delay_ms(100);
	// Анализ конца преобразования
	adcStatus=ADC1_GetFlagStatus(ADC1_FLAG_EOC);
	if (adcStatus)
	{
		adc0=ADC1_GetConversionValue();
		ADC1_StartConversion();
	}
	/*
	else
	{
		nop();
	}
	*/
	if (adc0<490)
	{
		GPIO_WriteLow(dirStep);
		GPIO_WriteLow(enStep);
		//TIM2_TimeBaseInit(TIM2_PRESCALER_128, 10+190*adc0/510);
		clks=9+300*(uint32_t)adc0/510;
		TIM2_SetAutoreload((uint16_t)clks);
		//TIM2_SetAutoreload(10);
		TIM2_Cmd(ENABLE);
	}
	/*
	if ((250<=adc0) &&(adc0<490 ))
	{
		GPIO_WriteLow(dirStep);
		GPIO_WriteLow(enStep);
		//TIM2_TimeBaseInit(TIM2_PRESCALER_128, 10+190*adc0/510);
		//TIM2_SetAutoreload(10);
		TIM2_SetAutoreload(200);
		TIM2_Cmd(ENABLE);
	}
	*/
	
	
	
	//else
		if (adc0>520)
		{
			GPIO_WriteHigh(dirStep);
			GPIO_WriteLow(enStep);
			
			//TIM2_TimeBaseInit(TIM2_PRESCALER_128, 200-190*(adc0-510)/510);
			clks=309-((uint32_t)300*(adc0-510))/510;
			TIM2_SetAutoreload((uint16_t)clks);
			//TIM2_SetAutoreload(9);
			TIM2_Cmd(ENABLE);
		}
		if ((490<=adc0) &&(adc0<=520))
		{
			GPIO_WriteHigh(enStep);
			TIM2_Cmd(DISABLE);
			TIM2_SetCounter(0);
		}
	
	
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
