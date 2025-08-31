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

// Настройка скорости передачи 
// определяем задержку между фронтами
// Ниже 5 не работает протокол
#define tm1637_timeDelay 10

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Exported types ------------------------------------------------------------*/
// яркость светодиода
typedef enum  {
	tm1637_diplayOff = 0x80,
	tm1637_bright_0=0x88,
	tm1637_bright_1,
	tm1637_bright_2,
	tm1637_bright_3,
	tm1637_bright_4,
	tm1637_bright_5,
	tm1637_bright_6,
	tm1637_bright_7
}tm1637_bright_t;



// 
typedef struct
{
	GPIO_TypeDef* 		dioPort; 
	GPIO_Pin_TypeDef 	dioPin;
	GPIO_TypeDef* 		clkPort; 
	GPIO_Pin_TypeDef 	clkPin;
	tm1637_bright_t 			brightness;
	
	//uint8_t 					cpu_MHz;
}tm1637_t;

typedef enum
{
	tm1637_OK=0,
	tm1637_NoAsk
}
tm1637_status_t;

/** @addtogroup TM1637_Exported_Types
  * @{
  */
void TM1637_Init( tm1637_t 					*tm1637, 
									GPIO_TypeDef* 		dioPort, 
									GPIO_Pin_TypeDef 	dioPin,
									GPIO_TypeDef* 		clkPort, 
									GPIO_Pin_TypeDef 	clkPin,
									tm1637_bright_t 	brightness
									);



tm1637_status_t TM1637_4digit_TextOutput(tm1637_t *tm1637, char *st);
tm1637_status_t TM1637_6digit_TextOutput(tm1637_t *tm1637, char *st);
tm1637_status_t TM1637_DisplayOn (tm1637_t *tm1637);
tm1637_status_t TM1637_DisplayOff(tm1637_t *tm1637);
tm1637_status_t TM1637_Brightness(tm1637_t *tm1637, 	
																	tm1637_bright_t brightness);

// Низко уровневая функция отправки команды и данных контроллеру TM1637
tm1637_status_t TM1637_Send(tm1637_t *tm1637, uint8_t command, void* array, uint8_t len);
//  Низко уровневая функция чтения данных из контроллера TM1637
tm1637_status_t TM1637_RecieveData(tm1637_t *tm1637, void* array, uint8_t len);

//void TM1637_Stop(tm1637_t *tm1637);

#endif /* __STM8S_TM1637_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
