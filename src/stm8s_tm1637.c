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
	
void TM1637_Init( tm1637_t 					*tm1637, 
									GPIO_TypeDef* 		dioPort, 
									GPIO_Pin_TypeDef 	dioPin,
									GPIO_TypeDef* 		clkPort, 
									GPIO_Pin_TypeDef 	clkPin,
									brightness_t 			brightness
									//,uint8_t 					cpu_MHz
									)
{
	tm1637->dioPort=dioPort;
	tm1637->dioPin=dioPin;
	tm1637->clkPort=clkPort;
	tm1637->clkPin=clkPin;
	tm1637->brightness=brightness;
	GPIO_Init(dioPort, dioPin,  GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkPort, clkPin, GPIO_MODE_OUT_PP_HIGH_FAST);
	
}


void TM1637_Send(tm1637_t *tm1637, uint8_t command, void* array, uint8_t len)
{
	uint8_t i=8,j;
	uint8_t data;
	uint8_t delay_tm1637=10;
	uint8_t *ar=(uint8_t*)array;
	uint8_t state=0;
	// старт бита
	GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
	
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	//передача 8 бит
	state=0;
	while(1)
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
		
		i=9; // кол-во бит
		//delay_tm1637=2; // задержка
		while(i--)
		{
			
			GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
			if (data%2==0)
			{
				GPIO_WriteLow(tm1637->dioPort, tm1637->dioPin);
			}
			else
			{
				GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);	
			}
			data=data/2;
			
			GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
		}
		GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
		//while(delay_tm1637--);
	}
	
	
	// окончание пересылки
	GPIO_WriteLow(tm1637->clkPort, tm1637->clkPin);
	GPIO_Init(tm1637->dioPort, tm1637->dioPin,  GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteHigh(tm1637->clkPort, tm1637->clkPin);
	GPIO_WriteHigh(tm1637->dioPort, tm1637->dioPin);	
	
}

//------------
void TM1637_TextOutput(tm1637_t *tm1637,
													char *st)
{
	uint8_t seg[6]={0,0,0,0,0,0};
	uint8_t j=0;
	while(*st!='\0' && j<6 )
	{
		seg[j++]=ASCIIMap[ *st++ ];
	}
	TM1637_Send(tm1637, 0x40, 0, 0); // Normal
	TM1637_Send(tm1637, 0xC0, seg, j); // En
	//TM1637_Send(&disp, 0x87, 0, 0);
	TM1637_Send(tm1637, tm1637->brightness, 0, 0);// En
}

/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
