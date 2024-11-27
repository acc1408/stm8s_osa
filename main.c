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
#define enStep  GPIOD, GPIO_PIN_4
#define clkStep GPIOD, GPIO_PIN_2
#define dirStep GPIOC, GPIO_PIN_3

#define butLeft GPIOF, GPIO_PIN_5
#define butRight GPIOD, GPIO_PIN_5
#define potLeft GPIOB, GPIO_PIN_1
#define potRight GPIOB, GPIO_PIN_0


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
//uint8_t a[]={0x01,0x04,0x02,0x03,0x02};

//char simvol;		
//---------------------
// ADC_Single
/*
void ADC1_Single_Init(F_div);
uint16_t ADC1_Single_StartSW_getValue(channel);



void ADC1_Scan_Single(F_div, channels)
void ADC1_Start(SourceStart);
void ADC1_waitEndConvertion();
uint16_t ADC1_Scan_getValue(channel);

void ADC1_Scan_Cont(F_div, channels)
void ADC1_Start(SourceStart);
void ADC1_waitEndConvertion();
uint16_t ADC1_Scan_getValue(channel);

*/
//---------------------
uint16_t adcV[10];
// Инициализация 
void ADC1_SingleOne_Init(ADC1_PresSel_TypeDef ADC1_PrescalerSelection, 
												ADC1_Handler_TypeDef Handler)
{
	assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
	/* Reset the ADC1 peripheral */
  ADC1->CR1 = 0;
	// Set the ADC1 peripheral
	ADC1->CR2 = ADC1_ALIGN_RIGHT;
	ADC1->CR3 = ADC1_BufferDisable;	// Disable buffer
	ADC1->eoc=0;
	ADC1->eocie=Handler;
	ADC1->CR1 = ADC1_PrescalerSelection;
}

void ADC1_SingleCont_Init(	ADC1_PresSel_TypeDef ADC1_PrescalerSelection, 
													ADC1_Handler_TypeDef Handler)
{
	assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
	/* Reset the ADC1 peripheral */
  ADC1->CR1 = 0;
	// Set the ADC1 peripheral
	ADC1->CR2 = ADC1_ALIGN_RIGHT;
	ADC1->CR3 = ADC1_BufferDisable;	// Disable buffer
	ADC1->eoc=0;
	ADC1->eocie=Handler;
	ADC1->CR1 = ADC1_PrescalerSelection|ADC1_CONVERSIONMODE_CONTINUOUS;
}
void ADC1_SingleContBuf_Init(ADC1_PresSel_TypeDef ADC1_PrescalerSelection, 
														ADC1_Handler_TypeDef Handler)
{
	assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
	/* Reset the ADC1 peripheral */
  ADC1->CR1 = 0;
	// Set the ADC1 peripheral
	ADC1->CR2 = ADC1_ALIGN_RIGHT;
	ADC1->CR3 = ADC1_BufferEnable;	// Enable buffer
	ADC1->eoc=0;
	ADC1->eocie=Handler;
	ADC1->CR1 = ADC1_PrescalerSelection|ADC1_CONVERSIONMODE_CONTINUOUS;
}

void ADC1_ScanOne_Init(ADC1_PresSel_TypeDef ADC1_PrescalerSelection, 
												ADC1_Handler_TypeDef Handler)
{
	assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
	/* Reset the ADC1 peripheral */
  ADC1->CR1 = 0;
	// Set the ADC1 peripheral
	ADC1->CR2 = ADC1_ALIGN_RIGHT|ADC1_ModeSCAN;
	ADC1->CR3 = ADC1_BufferDisable;	// Disable buffer
	ADC1->eoc=0;
	ADC1->eocie=Handler;
	ADC1->CR1 = ADC1_PrescalerSelection;
}

void ADC1_ScanCont_Init(ADC1_PresSel_TypeDef ADC1_PrescalerSelection, 
												ADC1_Handler_TypeDef Handler)
{
	assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
	/* Reset the ADC1 peripheral */
  ADC1->CR1 = 0;
	// Set the ADC1 peripheral
	ADC1->CR2 = ADC1_ALIGN_RIGHT|ADC1_ModeSCAN;
	ADC1->CR3 = ADC1_BufferDisable;	// Disable buffer
	ADC1->eoc=0;
	ADC1->eocie=Handler;
	ADC1->CR1 = ADC1_PrescalerSelection|ADC1_ModeSCAN;
}


void ADC1_SingleAny_Start(ADC1_SourceStart_TypeDef SourceStart, ADC1_Channel_TypeDef ADC1_CHANNEL)
{
	assert_param(IS_ADC1_SourceStart_OK(SourceStart));
	assert_param(IS_ADC1_CHANNEL_OK(ADC1_CHANNEL));
	//ADC1->TDR=0;
	ADC1->TDR=(uint16_t)1<<ADC1_CHANNEL;
	ADC1->ch=ADC1_CHANNEL;
	ADC1->adon=0;
	ADC1->adon=1;
	if (SourceStart!=ADC1_SOFT)
	{
		
		ADC1->extsel=SourceStart;
		ADC1->exttrig=1;
	}
	else
	{
		ADC1->exttrig=0;
		ADC1->adon=1;
	}
}

void ADC1_ScanAny_Start(ADC1_SourceStart_TypeDef SourceStart,
												ADC1_Channel_TypeDef ADC1_CHANNEL)
{
	assert_param(IS_ADC1_SourceStart_OK(SourceStart));
	assert_param(IS_ADC1_CHANNEL_OK(ADC1_CHANNEL));
	ADC1->TDR=((uint16_t)1<<(ADC1_CHANNEL+1))-1;
	ADC1->ch=ADC1_CHANNEL;
	ADC1->adon=0;
	ADC1->adon=1;
	if (SourceStart!=ADC1_SOFT)
	{
		
		ADC1->extsel=SourceStart;
		ADC1->exttrig=1;
	}
	else
	{
		ADC1->exttrig=0;
		ADC1->adon=1;
	}
}


uint16_t ADC1_SingleOne_GetConversion(void)
{
	while(!ADC1->eoc);
	ADC1->eoc=0;
	ADC1->TDR=0;
	return ADC1->DR;
}

uint16_t ADC1_SingleCont_GetConversion(ADC1_ContStop_TypeDef NextConvert)
{
	while(!ADC1->eoc);
	ADC1->eoc=0;
	if (NextConvert==ADC1_Stop)
	{
		ADC1->adon=0;
	}
	
	ADC1->TDR=0;
	return ADC1->DR;
}

ADC1_OverrunFlag_t ADC1_SingleContBuf_GetConversion(uint16_t* adcValue,
																				uint8_t sizeBuf_1_to_10,
																				ADC1_ContStop_TypeDef NextConvert )
{
	uint8_t i;
	uint16_t adc0,adc1;
	assert_param(sizeBuf_1_to_10>=1&&sizeBuf_1_to_10<=10);
	while(!ADC1->eoc);
	for(i=0;i<sizeBuf_1_to_10;i++)
	{
		do{
			adc0=ADC1->DBR[i];
			adc1=ADC1->DBR[i];
		}while(adc0!=adc1);
		adcValue[i]=adc1;
	}
	ADC1->eoc=0;
	if (NextConvert==ADC1_Stop)
	{
		ADC1->adon=0;
		ADC1->TDR=0;
	}
	i=ADC1->ovr;
	ADC1->ovr=0;
	return i;
}

ADC1_OverrunFlag_t ADC1_ScanOne_GetConversion(uint16_t* adcValue,
																				uint8_t sizeBuf_1_to_10)
{
	uint8_t i=1;
	//uint16_t adc0,adc1;
	assert_param(sizeBuf_1_to_10>=1&&sizeBuf_1_to_10<=10);
	//repeat_ScanOne:
	while(!ADC1->eoc);
	for(i=0;i<sizeBuf_1_to_10;i++)
	{
		adcValue[i]=ADC1->DBR[i];
	}
	// Сброс флага конца преобразования
	ADC1->eoc=0;
	ADC1->TDR=0;
	// Сброс флага перезаписи
	i=ADC1->ovr;
	ADC1->ovr=0;
	return i;
}

ADC1_OverrunFlag_t ADC1_ScanCont_GetConversion(uint16_t* adcValue,
																				uint8_t sizeBuf_1_to_10,
																				ADC1_ContStop_TypeDef NextConvert)
{
	uint8_t i;
	uint16_t adc0,adc1;
	assert_param(sizeBuf_1_to_10>=1&&sizeBuf_1_to_10<=10);
	//repeat_ScanOne:
	while(!ADC1->eoc);
	for(i=0;i<sizeBuf_1_to_10;i++)
	{
		do{
			adc0=ADC1->DBR[i];
			adc1=ADC1->DBR[i];
		}while(adc0!=adc1);
		adcValue[i]=adc1;
	}
	//Подсчет кол-ва каналов
	i=0;
	adc0=ADC1->TDR;
	while(adc0)
	{
		adc0=adc0>>1;
		i++;
	}
	i--;
	ADC1->eoc=0;
	ADC1->ch=i ;
	if (NextConvert==ADC1_Stop)
	{
		ADC1->adon=0;
		ADC1->TDR=0;
	}
	//if (ADC1->ch== sizeBuf_1_to_10)
	
	i=ADC1->ovr;
	ADC1->ovr=0;
	return i;
}
void ADC1_AWD_Init(	ADC1_AWD_CH_t AWD_CH, 
										uint16_t AWD_HTR,
										uint16_t AWD_LTR, 
										ADC1_Handler_TypeDef AWD_Handler)
{
	ADC1->HTRH=(uint8_t) AWD_HTR>>2;
	ADC1->HTRL=(uint8_t) AWD_HTR;
	ADC1->LTRH=(uint8_t) AWD_LTR>>2;
	ADC1->LTRL=(uint8_t) AWD_LTR;
	ADC1->AWCR=AWD_CH;
	ADC1->AWSR=0;
	ADC1->awd=0;
	ADC1->awdie=AWD_Handler;
}
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
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	GPIO_Init(enStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(dirStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	/*
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
               ADC1_CHANNEL_0,
               ADC1_PRESSEL_FCPU_D2, 
               ADC1_EXTTRIG_TIM, 
               DISABLE, // Trig
							 ADC1_ALIGN_RIGHT, 
               ADC1_SCHMITTTRIG_CHANNEL0, 
               DISABLE);
	*/
	//ADC1_StartConversion();
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 9600);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	//TIM2_Cmd(ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
	/*
	ADC1_SingleOne_Init(ADC1_PRESSEL_FCPU_D18, 
												ADC1_Handler_NoIT);
	*/
	/*
	ADC1_SingleContBuf_Init(ADC1_PRESSEL_FCPU_D18, 
												ADC1_Handler_NoIT);
	*/
	/*
	ADC1_ScanOne_Init(ADC1_PRESSEL_FCPU_D18, 
												ADC1_Handler_NoIT);
	*/
	ADC1_ScanCont_Init(ADC1_PRESSEL_FCPU_D18, 
												ADC1_Handler_NoIT);
	ADC1_ScanAny_Start(ADC1_SOFT,ADC1_CHANNEL_1);
	ADC1_AWD_Init(	ADC1_AWD_CH_3, 
										0xFF,
										0xF, 
										ADC1_Handler_IT);
	//enableInterrupts()
	while (1)
  {
		
	
	nmb++;
	/*
	ADC1_SingleAny_Start(ADC1_SOFT, ADC1_CHANNEL_1);
	ADC1_SingleContBuf_GetConversion(adcV,
																				10,
																				ADC1_Continue);
	*/
	//ADC1_ScanAny_Start(ADC1_SOFT,ADC1_CHANNEL_2);
	
	ADC1_ScanCont_GetConversion(adcV,10,ADC1_Continue);
	//ADC1_ScanOne_GetConversion(adcV,10);
	nop();
	//ADC1_SingleOne_GetConversion();
	//if (bl) 
	//GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	//delay_ms(100);
	// Анализ конца преобразования
	/*
	adcStatus=ADC1_GetFlagStatus(ADC1_FLAG_EOC);
	if (adcStatus)
	{
		adc0=ADC1_GetConversionValue();
		ADC1_StartConversion();
	}
	*/
	/*
	else
	{
		nop();
	}
	*/
	/*
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
	*/
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
	/*
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
	
		*/
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
