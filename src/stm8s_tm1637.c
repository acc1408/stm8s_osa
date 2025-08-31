/**
  ******************************************************************************
  * @file    stm8s_adc2.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the functions/macros for the ADC2 peripheral.
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
#include "inc/stm8s_tm1637.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define tm1637_maskBright 0x8F
#define tm1637_maskDisplayOn 0x88
#define tm1637_maskDisplayOff 0x87
#define tm1637_CMD_dataWrite	0x40
#define tm1637_CMD_dataRead 	0x42
#define tm1637_CMD_adrReg_C0H  0xC0


/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/**
  * @addtogroup tm1637_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the ADC2 peripheral registers to their default reset
  * values.
  * @param  None
  * @retval None
  */
// Экран TM1637
const 	uint8_t ASCIIMap[128] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0,
		0x8, // , (_)
		0x40, // -
		0x8, // . (_)
		0,
		0x3f, // 0
		0x06, // 1
		0x5b, // 2
		0x4f, // 3
		0x66, // 4
		0x6d, // 5
		0x7d, // 6
		0x07, // 7
		0x7f, // 8
		0x6f, // 9
		0, 0, 0, 0, 0, 0, 0,
		0x77, // a
		0x7c, // b
		0x39, // c
		0x5e, // d
		0x79, // e
		0x71, // f
		0x3d, // g
		0x76, // h
		0x04, // i
		0x1e, // j
		0x75, // k
		0x38, // l
		0x49, // m
		0x54, // n
		0x5c, // o
		0x73, // p
		0x67, // q
		0x50, // r
		0x6d, // s
		0x78, // t
		0x1c, // u
		0x3e, // V
		0x7e, // W
		0x76, // x (H)
		0x6e, // y
		0x5b, // z (2)
		0, 0, 0, 0,
		0x8, // _
		0,
		0x77, // a
		0x7c, // b
		0x39, // c
		0x5e, // d
		0x79, // e
		0x71, // f
		0x3d, // g
		0x76, // h
		0x04, // i
		0x1e, // j
		0x75, // k
		0x38, // l
		0x49, // m
		0x54, // n
		0x5c, // o
		0x73, // p
		0x67, // q
		0x50, // r
		0x6d, // s
		0x78, // t
		0x1c, // u
		0x3e, // V
		0x7e, // W
		0x76, // x (H)
		0x6e, // y
		0x5b, // z (2)
		0
	};
	
	


void delay_tm1637(void)
{
	volatile uint8_t delay_tm1637=tm1637_timeDelay;
  
	while(delay_tm1637--); 
	
}
	
void TM1637_Init( tm1637_t 					*tm1637, 
									GPIO_TypeDef* 		dioPort, 
									GPIO_Pin_TypeDef 	dioPin,
									GPIO_TypeDef* 		clkPort, 
									GPIO_Pin_TypeDef 	clkPin,
									tm1637_bright_t 			brightness

									)
{
	uint8_t delay_tm1637=10;
	
	tm1637->dioPort=dioPort;
	tm1637->dioPin=dioPin;
	tm1637->clkPort=clkPort;
	tm1637->clkPin=clkPin;
	tm1637->brightness=brightness&tm1637_maskBright;
	GPIO_Init(dioPort, dioPin,  GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkPort, clkPin, GPIO_MODE_OUT_PP_HIGH_FAST);
}

/*
void TM1637_Stop(tm1637_t *tm1637)
{
	//volatile uint16_t t=10000;
	//uint8_t delay_tm1637=10;
	// старт бита
	GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
	delay_tm1637();
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	delay_tm1637();
	delay_tm1637();
	delay_tm1637();
	//while(t--);
	// стоп
	GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	delay_tm1637();
	GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);
	delay_tm1637();
}
 */

tm1637_status_t TM1637_RecieveData(tm1637_t *tm1637, void* array, uint8_t len)
{
	uint8_t i=8,j;
	uint8_t data;
	//uint8_t delay_tm1637=10;
	uint8_t *ar=(uint8_t*)array;
	uint8_t state=0;
	tm1637_status_t ret=tm1637_OK;
	// старт бита
	GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
	delay_tm1637();
	//delay_tm1637=10;
	//while(delay_tm1637--);
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	delay_tm1637();
	//delay_tm1637=5;
	//while(delay_tm1637--);
	//передача 8 бит
	
	data=tm1637_CMD_dataRead; // передаем команду для чтения
	//--------------------------
	i=8; // кол-во бит
	delay_tm1637();
	while(i--)
	{
		if (data&1 )
		{
			GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);
		}
		else
		{
			GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
		}
		data=data>>1;
		delay_tm1637();
		GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
		delay_tm1637();
		GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
		delay_tm1637();
	}
	// чтение отклика
	GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_IN_PU_NO_IT);
	GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);

	delay_tm1637();
	if ( GPIO_ReadInputPin(tm1637->dioPort, tm1637->dioPin) )
	{
		ret=tm1637_NoAsk;
	}
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	
	delay_tm1637();
	GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_OUT_PP_LOW_FAST);
	
	if (ret == tm1637_NoAsk)
	{
		goto lbl_tm1637_stop;
	}
	//delay_tm1637();
	// прием данных
	
	while(ret == tm1637_OK && len>0)
	{
		len--;
		
		GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_IN_PU_NO_IT);
		delay_tm1637();
		i=8; // кол-во бит
		while(i--)
		{
			data=data>>1;
			GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
			delay_tm1637();
			if (GPIO_ReadInputPin(tm1637->dioPort, tm1637->dioPin) )
			{
				data|=0x80;
			}
			GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
			delay_tm1637();
		}
		// проверка отклика
		// чтение отклика
		GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_IN_PU_NO_IT);
		GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	
		delay_tm1637();
		if ( GPIO_ReadInputPin(tm1637->dioPort, tm1637->dioPin) )
		{
			ret=tm1637_NoAsk;
		}
		GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
		
		delay_tm1637();
		GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_OUT_PP_LOW_FAST);
		
		if (ret == tm1637_NoAsk)
		{
			goto lbl_tm1637_stop;
		}
		else
		{
			*ar=data;
			ar++;
		}
		
	}
	
	// окончание пересылки
	lbl_tm1637_stop:
	delay_tm1637();
	GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	delay_tm1637();
	GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);	
	delay_tm1637();
	return ret;
	
	
}


tm1637_status_t TM1637_Send(tm1637_t *tm1637, uint8_t command, void* array, uint8_t len)
{
	uint8_t i=8,j;
	uint8_t data;
	//uint8_t delay_tm1637=10;
	uint8_t *ar=(uint8_t*)array;
	uint8_t state=0;
	tm1637_status_t ret=tm1637_OK;
	
	// старт бита
	GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
	delay_tm1637();
	//delay_tm1637=10;
	//while(delay_tm1637--);
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	delay_tm1637();
	//delay_tm1637=5;
	//while(delay_tm1637--);
	//передача 8 бит
	state=0;
	while(ret == tm1637_OK)
	{
		if (state==0)
		{
			data=command;
			state=1;
		}
		else
		{
			if (len--)
			{
				data=*ar++;
			}
			else
			{
				break;
			}
		}
		
		i=8; // кол-во бит
		//delay_tm1637=2; // задержка
		delay_tm1637();
		while(i--)
		{
			if (data&1 )
			{
				GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);
			}
			else
			{
				GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
			}
			data=data>>1;
			delay_tm1637();
			GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
			delay_tm1637();
			//delay_tm1637=5;
			//while(delay_tm1637--);
			GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
			delay_tm1637();
			//delay_tm1637();
		}
		//GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
		//delay_tm1637=10;
		//while(delay_tm1637--);
		//-----------------------------
		// чтение отклика
		
		GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_IN_PU_NO_IT);
		GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	
		delay_tm1637();
		if ( GPIO_ReadInputPin(tm1637->dioPort, tm1637->dioPin) )
		{
			ret=tm1637_NoAsk;
		}
		GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
		
		delay_tm1637();
		GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_OUT_PP_LOW_FAST);
		//delay_tm1637();
	}
	
	// окончание пересылки
	//stop_tm1637:
	//GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	//GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_OUT_PP_LOW_FAST);
	delay_tm1637();
	GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	
	//delay_tm1637();
	delay_tm1637();
	GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);	
	//delay_tm1637();
	delay_tm1637();
	return ret;
	
}

//------------
tm1637_status_t TM1637_4digit_TextOutput(tm1637_t *tm1637, char *st)
{
	tm1637_status_t ret=tm1637_OK;
	uint8_t seg[4];
	uint8_t j=0;
	while(*st!='\0' && j<4 )
	{
		seg[ j ]=ASCIIMap[ *st++ ];
		if (*st != '\0')
		{
			switch(*st)
			{
				case '.' :
				case ',':
				case ':':
					seg[ j ]|=0x80;
					*st++ ;
					break;
			}
		}
		j++;
	}
	
	// последние символы заменяем пробелами
	while(j<4)
	{
		seg[ j++  ]= ASCIIMap[' '];
	}
	//TM1637_Stop(tm1637);
	ret|=TM1637_Send(tm1637, tm1637_CMD_dataWrite, 0, 0); // тип записи
	if (ret!=tm1637_OK)
	{
		return ret;
	}
	ret|=TM1637_Send(tm1637, tm1637_CMD_adrReg_C0H,seg, j);
	if (ret!=tm1637_OK)
	{
		return ret;
	}
	ret|=TM1637_Send(tm1637, tm1637->brightness, 0, 0);
	return ret;

}

// Перевод строки в нужном направлении
const uint8_t TM1637_charToDigit[]={2,1,0,5,4,3};

tm1637_status_t TM1637_6digit_TextOutput(tm1637_t *tm1637,char *st)
{
	uint8_t seg[6];
	uint8_t j=0;
	tm1637_status_t ret=tm1637_OK;
	
	while(*st!='\0' && j<6 )
	{
		seg[ TM1637_charToDigit[j]  ]=ASCIIMap[ *st++ ];
		if (*st != '\0')
		{
			switch(*st)
			{
				case '.' :
				case ',':
				case ':':
					seg[ TM1637_charToDigit[j]  ]|=0x80;
					*st++ ;
					break;
			}
		}
		j++;
	}
	// последние символы заменяем пробелами
	while(j<6)
	{
		seg[ TM1637_charToDigit[j++]  ]= ASCIIMap[' '];
	}
	
	//TM1637_Stop(tm1637);
	ret|=TM1637_Send(tm1637, tm1637_CMD_dataWrite, 0, 0); // тип записи
	if (ret!=tm1637_OK)
	{
		return ret;
	}
	ret|=TM1637_Send(tm1637, tm1637_CMD_adrReg_C0H,seg, j);
	if (ret!=tm1637_OK)
	{
		return ret;
	}
	ret|=TM1637_Send(tm1637, tm1637->brightness, 0, 0);
	return ret;
}

tm1637_status_t TM1637_DisplayOn(tm1637_t *tm1637)
{
	tm1637_status_t ret;
	tm1637->brightness|=tm1637_maskDisplayOn;
	ret=TM1637_Send(tm1637, tm1637->brightness, 0, 0);
	return ret;
}
tm1637_status_t TM1637_DisplayOff(tm1637_t *tm1637)
{
	tm1637_status_t ret;
	tm1637->brightness&=tm1637_maskDisplayOff;
	ret=TM1637_Send(tm1637, tm1637->brightness, 0, 0);
	return ret;
}

tm1637_status_t TM1637_Brightness(tm1637_t *tm1637, 	
																	tm1637_bright_t brightness)
{
	tm1637_status_t ret;
	tm1637->brightness=brightness&tm1637_maskBright;
	ret=TM1637_Send(tm1637, tm1637->brightness, 0, 0);
	return ret;
}



/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
