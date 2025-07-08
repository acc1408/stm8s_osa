/**
  ******************************************************************************
  * @file    stm8s_adc1.h
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the prototypes/macros for the ADC1 peripheral.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_TM1637_H
#define __STM8S_TM1637_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
// €ркость светодиода
typedef enum  {
	DiplayOff = 0x80,
	Brightness0=0x88,
	Brightness1,
	Brightness2,
	Brightness3,
	Brightness4,
	Brightness5,
	Brightness6,
	Brightness7
}brightness_t;

// 
typedef struct
{
	GPIO_TypeDef* 		dioPort; 
	GPIO_Pin_TypeDef 	dioPin;
	GPIO_TypeDef* 		clkPort; 
	GPIO_Pin_TypeDef 	clkPin;
	brightness_t 			brightness;
	
	//uint8_t 					cpu_MHz;
}tm1637_t;
/** @addtogroup TM1637_Exported_Types
  * @{
  */
void TM1637_Init( tm1637_t 					*tm1637, 
									GPIO_TypeDef* 		dioPort, 
									GPIO_Pin_TypeDef 	dioPin,
									GPIO_TypeDef* 		clkPort, 
									GPIO_Pin_TypeDef 	clkPin,
									brightness_t 			brightness
									);
									
void TM1637_TextOutput(tm1637_t *tm1637,char *st);



#endif /* __STM8S_TM1637_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
