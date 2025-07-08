/**
  ******************************************************************************
  * @file    stm8s_adc1.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the functions/macros for the ADC1 peripheral.
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
#include "inc/stm8s_adc1.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup ADC1_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the ADC1 peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
	
	
	
	
	
	
	

//---------Начало моего драйвера
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


void ADC1_SingleAny_Start(ADC1_SourceStart_TypeDef SourceStart, 
													ADC1_Channel_TypeDef ADC1_CHANNEL)
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
void ADC1_SingleContBuf_AWD_Init(	uint16_t AWD_HTR,
																	uint16_t AWD_LTR, 
																	ADC1_Handler_TypeDef AWD_Handler)
{
	ADC1->HTRH=(uint8_t) AWD_HTR>>2;
	ADC1->HTRL=(uint8_t) AWD_HTR;
	ADC1->LTRH=(uint8_t) AWD_LTR>>2;
	ADC1->LTRL=(uint8_t) AWD_LTR;
	ADC1->AWCR=0x03FF;
	ADC1->AWSR=0; // Сброс флага каналов
	ADC1->awd=0; //  Сброс флага прерывания
	ADC1->awdie=AWD_Handler; // Включение прерывания
}

void ADC1_ScanAny_AWD_Init(	ADC1_AWD_CH_t AWD_CH, 
										uint16_t AWD_HTR,
										uint16_t AWD_LTR, 
										ADC1_Handler_TypeDef AWD_Handler)
{
	ADC1->HTRH=(uint8_t) AWD_HTR>>2;
	ADC1->HTRL=(uint8_t) AWD_HTR;
	ADC1->LTRH=(uint8_t) AWD_LTR>>2;
	ADC1->LTRL=(uint8_t) AWD_LTR;
	ADC1->AWCR=AWD_CH;
	ADC1->AWSR=0; // Сброс флага каналов
	ADC1->awd=0; //  Сброс флага прерывания
	ADC1->awdie=AWD_Handler; // Включение прерывания
}

ADC1_AWD_Flag_t ADC1_SingleContBuf_AWD_getFlag(void)
{
	uint8_t f,i;
	f=ADC1->awd;
	ADC1->awd=0;
	return f;
}

ADC1_AWD_Flag_t ADC1_ScanAny_AWD_getFlag(void)
{
	uint8_t f,i;
	uint16_t adc0;
	f=ADC1->awd;
	if (f)
	{
		//Подсчет кол-ва каналов
		i=0;
		adc0=ADC1->TDR;
		while(adc0)
		{
			adc0=adc0>>1;
			i++;
		}
		i--;
		ADC1->awd=0;
		ADC1->ch=i ;
	}
	return f;
}

AWSR_AIN_t ADC1_ScanAny_AWD_getFlagAIN(void)
{
	AWSR_AIN_t t;
	t.AIN_All=ADC1->AWSR;
	ADC1->AWSR=0;
	return t;
}	
//---------Конец моего драйвера
	
	
void ADC1_DeInit(void)
{
  ADC1->CSR  = ADC1_CSR_RESET_VALUE;
  ADC1->CR1  = ADC1_CR1_RESET_VALUE;
  ADC1->CR2  = ADC1_CR2_RESET_VALUE;
  ADC1->CR3  = ADC1_CR3_RESET_VALUE;
  ADC1->TDRH = ADC1_TDRH_RESET_VALUE;
  ADC1->TDRL = ADC1_TDRL_RESET_VALUE;
  ADC1->HTRH = ADC1_HTRH_RESET_VALUE;
  ADC1->HTRL = ADC1_HTRL_RESET_VALUE;
  ADC1->LTRH = ADC1_LTRH_RESET_VALUE;
  ADC1->LTRL = ADC1_LTRL_RESET_VALUE;
  ADC1->AWCRH = ADC1_AWCRH_RESET_VALUE;
  ADC1->AWCRL = ADC1_AWCRL_RESET_VALUE;
}

/**
  * @brief  Initializes the ADC1 peripheral according to the specified parameters
  * @param   ADC1_ConversionMode: specifies the conversion mode
  * can be one of the values of @ref ADC1_ConvMode_TypeDef.
  * @param   ADC1_Channel: specifies the channel to convert
  * can be one of the values of @ref ADC1_Channel_TypeDef.
  * @param   ADC1_PrescalerSelection: specifies the ADC1 prescaler
  * can be one of the values of @ref ADC1_PresSel_TypeDef.
  * @param   ADC1_ExtTrigger: specifies the external trigger
  * can be one of the values of @ref ADC1_ExtTrig_TypeDef.
  * @param   ADC1_ExtTriggerState: specifies the external trigger new state
  * can be one of the values of @ref FunctionalState.
  * @param   ADC1_Align: specifies the converted data alignment
  * can be one of the values of @ref ADC1_Align_TypeDef.
  * @param   ADC1_SchmittTriggerChannel: specifies the schmitt trigger channel
  * can be one of the values of @ref ADC1_SchmittTrigg_TypeDef.
  * @param   ADC1_SchmittTriggerState: specifies the schmitt trigger state
  * can be one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_Init(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_PresSel_TypeDef ADC1_PrescalerSelection, ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState ADC1_ExtTriggerState, ADC1_Align_TypeDef ADC1_Align, ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState ADC1_SchmittTriggerState)
{
  /* Check the parameters */
  assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
  assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
  assert_param(IS_ADC1_PRESSEL_OK(ADC1_PrescalerSelection));
  assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
  assert_param(IS_FUNCTIONALSTATE_OK(((ADC1_ExtTriggerState))));
  assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));
  assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
  assert_param(IS_FUNCTIONALSTATE_OK(ADC1_SchmittTriggerState));
  
  /*-----------------CR1 & CSR configuration --------------------*/
  /* Configure the conversion mode and the channel to convert
  respectively according to ADC1_ConversionMode & ADC1_Channel values  &  ADC1_Align values */
  ADC1_ConversionConfig(ADC1_ConversionMode, ADC1_Channel, ADC1_Align);
  /* Select the prescaler division factor according to ADC1_PrescalerSelection values */
  ADC1_PrescalerConfig(ADC1_PrescalerSelection);
  
  /*-----------------CR2 configuration --------------------*/
  /* Configure the external trigger state and event respectively
  according to NewState, ADC1_ExtTrigger */
  ADC1_ExternalTriggerConfig(ADC1_ExtTrigger, ADC1_ExtTriggerState);
  
  /*------------------TDR configuration ---------------------------*/
  /* Configure the schmitt trigger channel and state respectively
  according to ADC1_SchmittTriggerChannel & ADC1_SchmittTriggerNewState  values */
  ADC1_SchmittTriggerConfig(ADC1_SchmittTriggerChannel, ADC1_SchmittTriggerState);
  
  /* Enable the ADC1 peripheral */
  ADC1->CR1 |= ADC1_CR1_ADON;
}

/**
  * @brief  Enables or Disables the ADC1 peripheral.
  * @param  NewState: specifies the peripheral enabled or disabled state.
  * @retval None
  */
void ADC1_Cmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    ADC1->CR1 |= ADC1_CR1_ADON;
  }
  else /* NewState == DISABLE */
  {
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_ADON);
  }
}

/**
  * @brief  Enables or Disables the ADC1 scan mode.
  * @param  NewState: specifies the selected mode enabled or disabled state.
  * @retval None
  */
void ADC1_ScanModeCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    ADC1->CR2 |= ADC1_CR2_SCAN;
  }
  else /* NewState == DISABLE */
  {
    ADC1->CR2 &= (uint8_t)(~ADC1_CR2_SCAN);
  }
}

/**
  * @brief  Enables or Disables the ADC1 data store into the Data Buffer registers rather than in the Data Register
  * @param   NewState: specifies the selected mode enabled or disabled state.
  * @retval None
  */
void ADC1_DataBufferCmd(FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    ADC1->CR3 |= ADC1_CR3_DBUF;
  }
  else /* NewState == DISABLE */
  {
    ADC1->CR3 &= (uint8_t)(~ADC1_CR3_DBUF);
  }
}

/**
  * @brief  Enables or disables the ADC1 interrupt.
  * @param   ADC1_IT specifies the name of the interrupt to enable or disable.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWDITEN : Analog WDG interrupt enable
  *    - ADC1_IT_EOCITEN  : EOC iterrupt enable
  * @param   NewState specifies the state of the interrupt to apply.
  * @retval None
  */
void ADC1_ITConfig(ADC1_IT_TypeDef ADC1_IT, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ADC1_IT_OK(ADC1_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the ADC1 interrupts */
    ADC1->CSR |= (uint8_t)ADC1_IT;
  }
  else  /* NewState == DISABLE */
  {
    /* Disable the ADC1 interrupts */
    ADC1->CSR &= (uint8_t)((uint16_t)~(uint16_t)ADC1_IT);
  }
}

/**
  * @brief  Configure the ADC1 prescaler division factor.
  * @param   ADC1_Prescaler: the selected precaler.
  * It can be one of the values of @ref ADC1_PresSel_TypeDef.
  * @retval None
  */
void ADC1_PrescalerConfig(ADC1_PresSel_TypeDef ADC1_Prescaler)
{
  /* Check the parameter */
  assert_param(IS_ADC1_PRESSEL_OK(ADC1_Prescaler));
  
  /* Clear the SPSEL bits */
  ADC1->CR1 &= (uint8_t)(~ADC1_CR1_SPSEL);
  /* Select the prescaler division factor according to ADC1_PrescalerSelection values */
  ADC1->CR1 |= (uint8_t)(ADC1_Prescaler);
}

/**
  * @brief  Enables or disables the ADC1 Schmitt Trigger on a selected channel.
  * @param   ADC1_SchmittTriggerChannel specifies the desired Channel.
  * It can be set of the values of @ref ADC1_SchmittTrigg_TypeDef.
  * @param   NewState specifies Channel new status.
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_SchmittTriggerConfig(ADC1_SchmittTrigg_TypeDef ADC1_SchmittTriggerChannel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ADC1_SCHMITTTRIG_OK(ADC1_SchmittTriggerChannel));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  if (ADC1_SchmittTriggerChannel == ADC1_SCHMITTTRIG_ALL)
  {
    if (NewState != DISABLE)
    {
      ADC1->TDRL &= (uint8_t)0x0;
      ADC1->TDRH &= (uint8_t)0x0;
    }
    else /* NewState == DISABLE */
    {
      ADC1->TDRL |= (uint8_t)0xFF;
      ADC1->TDRH |= (uint8_t)0xFF;
    }
  }
  else if (ADC1_SchmittTriggerChannel < ADC1_SCHMITTTRIG_CHANNEL8)
  {
    if (NewState != DISABLE)
    {
      ADC1->TDRL &= (uint8_t)(~(uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SchmittTriggerChannel));
    }
    else /* NewState == DISABLE */
    {
      ADC1->TDRL |= (uint8_t)((uint8_t)0x01 << (uint8_t)ADC1_SchmittTriggerChannel);
    }
  }
  else /* ADC1_SchmittTriggerChannel >= ADC1_SCHMITTTRIG_CHANNEL8 */
  {
    if (NewState != DISABLE)
    {
      ADC1->TDRH &= (uint8_t)(~(uint8_t)((uint8_t)0x01 << ((uint8_t)ADC1_SchmittTriggerChannel - (uint8_t)8)));
    }
    else /* NewState == DISABLE */
    {
      ADC1->TDRH |= (uint8_t)((uint8_t)0x01 << ((uint8_t)ADC1_SchmittTriggerChannel - (uint8_t)8));
    }
  }
}

/**
  * @brief  Configure the ADC1 conversion on selected channel.
  * @param   ADC1_ConversionMode Specifies the conversion type.
  * It can be set of the values of @ref ADC1_ConvMode_TypeDef
  * @param   ADC1_Channel specifies the ADC1 Channel.
  * It can be set of the values of @ref ADC1_Channel_TypeDef
  * @param   ADC1_Align specifies the converted data alignment.
  * It can be set of the values of @ref ADC1_Align_TypeDef
  * @retval None
  */
void ADC1_ConversionConfig(ADC1_ConvMode_TypeDef ADC1_ConversionMode, ADC1_Channel_TypeDef ADC1_Channel, ADC1_Align_TypeDef ADC1_Align)
{
  /* Check the parameters */
  assert_param(IS_ADC1_CONVERSIONMODE_OK(ADC1_ConversionMode));
  assert_param(IS_ADC1_CHANNEL_OK(ADC1_Channel));
  assert_param(IS_ADC1_ALIGN_OK(ADC1_Align));
  
  /* Clear the align bit */
  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_ALIGN);
  /* Configure the data alignment */
  ADC1->CR2 |= (uint8_t)(ADC1_Align);
  
  if (ADC1_ConversionMode == ADC1_CONVERSIONMODE_CONTINUOUS)
  {
    /* Set the continuous conversion mode */
    ADC1->CR1 |= ADC1_CR1_CONT;
  }
  else /* ADC1_ConversionMode == ADC1_CONVERSIONMODE_SINGLE */
  {
    /* Set the single conversion mode */
    ADC1->CR1 &= (uint8_t)(~ADC1_CR1_CONT);
  }
  
  /* Clear the ADC1 channels */
  ADC1->CSR &= (uint8_t)(~ADC1_CSR_CH);
  /* Select the ADC1 channel */
  ADC1->CSR |= (uint8_t)(ADC1_Channel);
}

/**
  * @brief  Configure the ADC1 conversion on external trigger event.
  * @par Full description:
  * The selected external trigger event can be enabled or disabled.
  * @param   ADC1_ExtTrigger to select the External trigger event.
  * can have one of the values of @ref ADC1_ExtTrig_TypeDef.
  * @param   NewState to enable/disable the selected external trigger
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_ExternalTriggerConfig(ADC1_ExtTrig_TypeDef ADC1_ExtTrigger, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_ADC1_EXTTRIG_OK(ADC1_ExtTrigger));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  
  /* Clear the external trigger selection bits */
  ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTSEL);
  
  if (NewState != DISABLE)
  {
    /* Enable the selected external Trigger */
    ADC1->CR2 |= (uint8_t)(ADC1_CR2_EXTTRIG);
  }
  else /* NewState == DISABLE */
  {
    /* Disable the selected external trigger */
    ADC1->CR2 &= (uint8_t)(~ADC1_CR2_EXTTRIG);
  }
  
  /* Set the selected external trigger */
  ADC1->CR2 |= (uint8_t)(ADC1_ExtTrigger);
}

/**
  * @brief  Start ADC1 conversion
  * @par Full description:
  * This function  triggers the start of conversion, after ADC1 configuration.
  * @param  None
  * @retval None
  * @par Required preconditions:
  * Enable the ADC1 peripheral before calling this function
  */
void ADC1_StartConversion(void)
{
  ADC1->CR1 |= ADC1_CR1_ADON;
}

/**
  * @brief  Get one sample of measured signal.
  * @param  None
  * @retval ConversionValue:  value of the measured signal.
  * @par Required preconditions:
  * ADC1 conversion finished.
  */
uint16_t ADC1_GetConversionValue(void)
{
  uint16_t temph = 0;
  uint8_t templ = 0;
  
  if ((ADC1->CR2 & ADC1_CR2_ALIGN) != 0) /* Right alignment */
  {
    /* Read LSB first */
    templ = ADC1->DRL;
    /* Then read MSB */
    temph = ADC1->DRH;
    
    temph = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));
  }
  else /* Left alignment */
  {
    /* Read MSB first*/
    temph = ADC1->DRH;
    /* Then read LSB */
    templ = ADC1->DRL;
    
    temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)((uint16_t)temph << 8));
  }
  
  return ((uint16_t)temph);
}

/**
  * @brief  Enables or disables the analog watchdog for the given channel.
  * @param   Channel specifies the desired Channel.
  * It can be set of the values of @ref ADC1_Channel_TypeDef.
  * @param   NewState specifies the analog watchdog new state.
  * can have one of the values of @ref FunctionalState.
  * @retval None
  */
void ADC1_AWDChannelConfig(ADC1_Channel_TypeDef Channel, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  assert_param(IS_ADC1_CHANNEL_OK(Channel));
  
  if (Channel < (uint8_t)8)
  {
    if (NewState != DISABLE)
    {
      ADC1->AWCRL |= (uint8_t)((uint8_t)1 << Channel);
    }
    else /* NewState == DISABLE */
    {
      ADC1->AWCRL &= (uint8_t)~(uint8_t)((uint8_t)1 << Channel);
    }
  }
  else
  {
    if (NewState != DISABLE)
    {
      ADC1->AWCRH |= (uint8_t)((uint8_t)1 << (Channel - (uint8_t)8));
    }
    else /* NewState == DISABLE */
    {
      ADC1->AWCRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (uint8_t)(Channel - (uint8_t)8));
    }
  }
}

/**
  * @brief  Sets the high threshold of the analog watchdog.
  * @param   Threshold specifies the high threshold value.
  * this value depends on the reference voltage range.
  * @retval None
  */
void ADC1_SetHighThreshold(uint16_t Threshold)
{
  ADC1->HTRH = (uint8_t)(Threshold >> (uint8_t)2);
  ADC1->HTRL = (uint8_t)Threshold;
}

/**
  * @brief  Sets the low threshold of the analog watchdog.
  * @param   Threshold specifies the low threshold value.
  * this value depends on the reference voltage range.
  * @retval None
  */
void ADC1_SetLowThreshold(uint16_t Threshold)
{
  ADC1->LTRL = (uint8_t)Threshold;
  ADC1->LTRH = (uint8_t)(Threshold >> (uint8_t)2);
}

/**
  * @brief  Get one sample of measured signal.
  * @param   Buffer specifies the buffer to read.
  * @retval BufferValue:  value read from the given buffer.
  * @par Required preconditions:
  * ADC1 conversion finished.
  */
uint16_t ADC1_GetBufferValue(uint8_t Buffer)
{
  uint16_t temph = 0;
  uint8_t templ = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_BUFFER_OK(Buffer));
  
  if ((ADC1->CR2 & ADC1_CR2_ALIGN) != 0) /* Right alignment */
  {
    /* Read LSB first */
    templ = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1) + 1);
    /* Then read MSB */
    temph = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1));
    
    temph = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));
  }
  else /* Left alignment */
  {
    /* Read MSB first*/
    temph = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1));
    /* Then read LSB */
    templ = *(uint8_t*)(uint16_t)((uint16_t)ADC1_BaseAddress + (uint8_t)(Buffer << 1) + 1);
    
    temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)(temph << 8));
  }
  
  return ((uint16_t)temph);
}

/**
  * @brief  Checks the specified analog watchdog channel status.
  * @param   Channel: specify the channel of which to check the analog watchdog
  * can be one of the values of @ref ADC1_Channel_TypeDef.
  * @retval FlagStatus Status of the analog watchdog.
  */
FlagStatus ADC1_GetAWDChannelStatus(ADC1_Channel_TypeDef Channel)
{
  uint8_t status = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_CHANNEL_OK(Channel));
  
  if (Channel < (uint8_t)8)
  {
    status = (uint8_t)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << Channel));
  }
  else /* Channel = 8 | 9 */
  {
    status = (uint8_t)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (Channel - (uint8_t)8)));
  }
  
  return ((FlagStatus)status);
}

/**
  * @brief  Checks the specified ADC1 flag status.
  * @param   Flag: ADC1 flag.
  * can be one of the values of @ref ADC1_Flag_TypeDef.
  * @retval FlagStatus Status of the ADC1 flag.
  */
FlagStatus ADC1_GetFlagStatus(ADC1_Flag_TypeDef Flag)
{
  uint8_t flagstatus = 0;
  uint8_t temp = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_FLAG_OK(Flag));
  
  if ((Flag & 0x0F) == 0x01)
  {
    /* Get OVR flag status */
    flagstatus = (uint8_t)(ADC1->CR3 & ADC1_CR3_OVR);
  }
  else if ((Flag & 0xF0) == 0x10)
  {
    /* Get analog watchdog channel status */
    temp = (uint8_t)(Flag & (uint8_t)0x0F);
    if (temp < 8)
    {
      flagstatus = (uint8_t)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << temp));
    }
    else
    {
      flagstatus = (uint8_t)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (temp - 8)));
    }
  }
  else  /* Get EOC | AWD flag status */
  {
    flagstatus = (uint8_t)(ADC1->CSR & Flag);
  }
  return ((FlagStatus)flagstatus);
  
}

/**
  * @brief  Clear the specified ADC1 Flag.
  * @param   Flag: ADC1 flag.
  * can be one of the values of @ref ADC1_Flag_TypeDef.
  * @retval None
  */
void ADC1_ClearFlag(ADC1_Flag_TypeDef Flag)
{
  uint8_t temp = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_FLAG_OK(Flag));
  
  if ((Flag & 0x0F) == 0x01)
  {
    /* Clear OVR flag status */
    ADC1->CR3 &= (uint8_t)(~ADC1_CR3_OVR);
  }
  else if ((Flag & 0xF0) == 0x10)
  {
    /* Clear analog watchdog channel status */
    temp = (uint8_t)(Flag & (uint8_t)0x0F);
    if (temp < 8)
    {
      ADC1->AWSRL &= (uint8_t)~(uint8_t)((uint8_t)1 << temp);
    }
    else
    {
      ADC1->AWSRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (temp - 8));
    }
  }
  else  /* Clear EOC | AWD flag status */
  {
    ADC1->CSR &= (uint8_t) (~Flag);
  }
}

/**
  * @brief  Returns the specified pending bit status
  * @param   ITPendingBit : the IT pending bit to check.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWD   : Analog WDG IT status
  *    - ADC1_IT_AWS0 : Analog channel 0 IT status
  *    - ADC1_IT_AWS1 : Analog channel 1 IT status
  *    - ADC1_IT_AWS2 : Analog channel 2 IT status
  *    - ADC1_IT_AWS3 : Analog channel 3 IT status
  *    - ADC1_IT_AWS4 : Analog channel 4 IT status
  *    - ADC1_IT_AWS5 : Analog channel 5 IT status
  *    - ADC1_IT_AWS6 : Analog channel 6 IT status
  *    - ADC1_IT_AWS7 : Analog channel 7 IT status
  *    - ADC1_IT_AWS8 : Analog channel 8 IT status
  *    - ADC1_IT_AWS9 : Analog channel 9 IT status
  *    - ADC1_IT_EOC    : EOC pending bit
  * @retval ITStatus: status of the specified pending bit.
  */
ITStatus ADC1_GetITStatus(ADC1_IT_TypeDef ITPendingBit)
{
  ITStatus itstatus = RESET;
  uint8_t temp = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));
  
  if (((uint16_t)ITPendingBit & 0xF0) == 0x10)
  {
    /* Get analog watchdog channel status */
    temp = (uint8_t)((uint16_t)ITPendingBit & 0x0F);
    if (temp < 8)
    {
      itstatus = (ITStatus)(ADC1->AWSRL & (uint8_t)((uint8_t)1 << temp));
    }
    else
    {
      itstatus = (ITStatus)(ADC1->AWSRH & (uint8_t)((uint8_t)1 << (temp - 8)));
    }
  }
  else  /* Get EOC | AWD flag status */
  {
    itstatus = (ITStatus)(ADC1->CSR & (uint8_t)ITPendingBit);
  }
  return ((ITStatus)itstatus);
}

/**
  * @brief  Clear the ADC1 End of Conversion pending bit.
  * @param   ITPendingBit : the IT pending bit to clear.
  * This parameter can be one of the following values:
  *    - ADC1_IT_AWD   : Analog WDG IT status
  *    - ADC1_IT_AWS0 : Analog channel 0 IT status
  *    - ADC1_IT_AWS1 : Analog channel 1 IT status
  *    - ADC1_IT_AWS2 : Analog channel 2 IT status
  *    - ADC1_IT_AWS3 : Analog channel 3 IT status
  *    - ADC1_IT_AWS4 : Analog channel 4 IT status
  *    - ADC1_IT_AWS5 : Analog channel 5 IT status
  *    - ADC1_IT_AWS6 : Analog channel 6 IT status
  *    - ADC1_IT_AWS7 : Analog channel 7 IT status
  *    - ADC1_IT_AWS8 : Analog channel 8 IT status
  *    - ADC1_IT_AWS9 : Analog channel 9 IT status
  *    - ADC1_IT_EOC  : EOC pending bit
  * @retval None
  */
void ADC1_ClearITPendingBit(ADC1_IT_TypeDef ITPendingBit)
{
  uint8_t temp = 0;
  
  /* Check the parameters */
  assert_param(IS_ADC1_ITPENDINGBIT_OK(ITPendingBit));
  
  if (((uint16_t)ITPendingBit & 0xF0) == 0x10)
  {
    /* Clear analog watchdog channel status */
    temp = (uint8_t)((uint16_t)ITPendingBit & 0x0F);
    if (temp < 8)
    {
      ADC1->AWSRL &= (uint8_t)~(uint8_t)((uint8_t)1 << temp);
    }
    else
    {
      ADC1->AWSRH &= (uint8_t)~(uint8_t)((uint8_t)1 << (temp - 8));
    }
  }
  else  /* Clear EOC | AWD flag status */
  {
    ADC1->CSR &= (uint8_t)((uint16_t)~(uint16_t)ITPendingBit);
  }
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
