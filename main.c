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
//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
//SetLCD_t lcd1;

//const char st[]="Hello world!!!qwertyuiopasdffghjj";


bme280_t bm;
uint32_t pres;
uint32_t hum;
int32_t t;
uint8_t rez;



void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	//Lcdi2cInit(&lcd1, 0b0111111);
	//Lcdi2cPrint(&lcd1, st);
	//LcdCursorRight(&lcd1);
	//LcdDisplayLeft(&lcd1);
	
	
	I2C_Init_7bit(100000);
	//I2C_MasterSendSend(0b0111111, a, 1, a+4, 3);
	//I2C_MasterSendReceive(0b0111111, a, 0, a+1, 4);
	
	
rez=BME280_Init(&bm, 0b1110110,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_FILTER_COEFF_8,
						BME280_STANDBY_TIME_250_MS,
						BME280_NORMAL_MODE,
						DISABLE
						);
	//BME280_Reset(&bm);
	//BME280_StartStop(&bm, BME280_NORMAL_MODE);
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
		rez=BME280_GetMeasurement(&bm);
		pres=BME280_compensate_P_int32(&bm);
		t=BME280_compensate_T_int32(&bm);
		hum=BME280_compensate_H_int32(&bm);
	}
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
	
	//Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  while (1)
  {
  //GPIO_WriteReverse(GPIOC, GPIO_PIN_1);
	//delay_ms(1000);
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
