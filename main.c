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
#include  <stdio.h>
//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
SetLCD_t lcd1;

 char st[40]="Test";

bme280_t bm;
uint32_t pres;
uint32_t hum;
int32_t t;
uint8_t rez;

<<<<<<< HEAD
bit8_t tst;
//uint8_t ask, sr1,sr2,sr3;
//test_t r;
/*
i2c_sr1_t sr1;
i2c_sr2_t sr2;
i2c_sr3_t sr3;
*/
=======
>>>>>>> i2c



int16_t a,b;
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
<<<<<<< HEAD
	/*
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(100000, 50, 
              I2C_DUTYCYCLE_2, I2C_ACK_NONE, 
              I2C_ADDMODE_7BIT, 16 );
	I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	I2C_Cmd(ENABLE);

	I2C_GenerateSTART();
	*/
=======
	I2C_Init_7bit(100000);
	//Lcdi2cInit(&lcd1, 0b0111111);
	Lcdi2cInit(&lcd1, 0b0111111, 
								ENABLE,
								DISABLE,
								ENABLE);
	LcdCursorSet(&lcd1, 0);
	Lcdi2cPrint(&lcd1, st);
	//LcdCursorRight(&lcd1);
	//LcdDisplayLeft(&lcd1);
	//I2C_MasterSendSend(0b0111111, a, 1, a+4, 3);
	//I2C_MasterSendReceive(0b0111111, a, 0, a+1, 4);
	
rez=BME280_Init(&bm, 0b1110110,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_FILTER_COEFF_8,
						BME280_STANDBY_TIME_250_MS,
						BME280_NORMAL_MODE
						);
	//BME280_Reset(&bm);
	//BME280_StartStop(&bm, BME280_NORMAL_MODE);
>>>>>>> i2c
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
<<<<<<< HEAD
		//OS_Delay(1000);
=======
		OS_Delay(200);
		rez=BME280_GetMeasurement(&bm);
		pres=BME280_compensate_P_int32(&bm);
		t=BME280_compensate_T_int32(&bm);
		hum=BME280_compensate_H_int32(&bm);
		a=t/100;
		b=t%100;
		CursorGoTo(&lcd1, 1, 0);
		sprintf(st,"Temp=%d.%02d C   ", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=pres/1000;
		b=pres%1000;
		LcdCursorSet(&lcd1, 20);
		sprintf(st,"Press=%d%03d Pa", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=hum/1024;
		b=hum%1024;
		if (b>999) b=999;
		LcdCursorSet(&lcd1, 60);
		sprintf(st,"Hum=%d.%03d%%   ", a,b);
		Lcdi2cPrint(&lcd1, st);
>>>>>>> i2c
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
