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
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
//SetLCD_t lcd1;

const char st[]="Hello world!!!qwertyuiopasdffghjj";

typedef enum
{
	BME280_NO_OVERSAMPLING=0,		
	BME280_OVERSAMPLING_1X=1,		
	BME280_OVERSAMPLING_2X=2,		
	BME280_OVERSAMPLING_4X=3,		
	BME280_OVERSAMPLING_8X=4,		
	BME280_OVERSAMPLING_16X=5		
} osrs_type;

typedef enum
{
	BME280_SLEEP_MODE=0,		
	BME280_FORCED_MODE=1,		
	BME280_NORMAL_MODE=3		
} mode_type;

typedef enum
{
	BME280_STANDBY_TIME_1_MS=0,
  BME280_STANDBY_TIME_62_5_MS=1,
	BME280_STANDBY_TIME_125_MS=2,
	BME280_STANDBY_TIME_250_MS=3,
	BME280_STANDBY_TIME_500_MS=4,
	BME280_STANDBY_TIME_1000_MS=5,
	BME280_STANDBY_TIME_10_MS=6,
	BME280_STANDBY_TIME_20_MS=7
} standby_type;

typedef enum
{
	BME280_FILTER_COEFF_OFF=0,               
BME280_FILTER_COEFF_2=1,                 
BME280_FILTER_COEFF_4=2,
BME280_FILTER_COEFF_8=3, 
BME280_FILTER_COEFF_16=4  
} filter_type;
 


union {
uint8_t a[50];
struct{
 uint8_t address; // 0
 uint16_t dig_T1; // 2
 int16_t dig_T2;	// 4
 int16_t dig_T3;	// 6
 uint16_t dig_P1;	// 8
 int16_t dig_P2;	// 10
 int16_t dig_P3;	// 12
 int16_t dig_P4;	// 14
 int16_t dig_P5;	// 16
 int16_t dig_P6;	// 18
 int16_t dig_P7;	// 20
 int16_t dig_P8;	// 22
 int16_t dig_P9;	// 24
 uint8_t dig_H1;	// 25
 //-------------
 int16_t dig_H2;	// 27
 uint8_t dig_H3;	// 28
 int16_t dig_H4;	// 30
 int16_t dig_H5;	// 32
 int8_t  dig_H6;	// 33
 
 uint32_t uncomp_hum;		// 37
 uint32_t uncomp_temp; 	// 41
 uint32_t uncomp_press; // 45
 
union										// 46
	{
		uint8_t config;
		struct
		{
			standby_type	t_sb:3;
			filter_type	filter:3;
			uint8_t res_con:1;
			uint8_t spi3w_en:1;
		};
	};
 
union										// 47
	{
		uint8_t ctrl_hum;
		struct
		{
			osrs_type osrs_h:3;
			uint8_t res_hum:5;
		};
	};
 
union										// 48
	{
		uint8_t ctrl_meas;
		struct
		{
			mode_type mode:2;
			osrs_type osrs_t:3;
			osrs_type osrs_p:3;
		};
	};

 
};
}bm;
//uint8_t a[]={1,2,3,4,5,6,7,8,9,10};
//i2cTask_t i2c_Task={0,0,0,0,0,0,0,0};



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
	bm.a[0]=0x88;
	I2C_MasterSendReceive(0b1110110, bm.a, 1, bm.a+1, 24);
	while(i2cCheckStatusTransfer());
	bm.a[0]=0xA1;
	I2C_MasterSendReceive(0b1110110, bm.a, 1, bm.a+25, 1);
	while(i2cCheckStatusTransfer());
	bm.a[0]=0xE1;
	I2C_MasterSendReceive(0b1110110, bm.a, 1, bm.a+26, 5);
	while(i2cCheckStatusTransfer());
	
	bm.a[0]=0xE5;
	I2C_MasterSendReceive(0b1110110, bm.a, 1, bm.a+31, 3);
	while(i2cCheckStatusTransfer());
	
	
	
	
	for(i=1;i<24;i=i+2)
	{
		temp=bm.a[i];
		bm.a[i]=bm.a[i+1];
		bm.a[i+1]=temp;
	}
		i=27;
		temp=bm.a[i];
		bm.a[i]=bm.a[i+1];
		bm.a[i+1]=temp;
	
	bm.a[30]=bm.a[30]<<4;
	bm.dig_H4=bm.dig_H4>>4;
	bm.a[31]=bm.a[31]>>4;
	bm.dig_H5=bm.dig_H5<<4;
	
	
	bm.t_sb=BME280_STANDBY_TIME_250_MS;
	bm.filter=BME280_FILTER_COEFF_8;
	bm.ctrl_hum=BME280_OVERSAMPLING_2X;
	bm.osrs_t=BME280_OVERSAMPLING_4X;
	bm.osrs_p=BME280_OVERSAMPLING_16X;
	bm.mode=BME280_NORMAL_MODE;
	// Отправка времени и фильтра
	bm.a[0]=0xF5;
	I2C_MasterSendSend(0b1110110, bm.a, 1, bm.a+46, 1);
	while(i2cCheckStatusTransfer());
	// Отправка настройки влажности
	bm.a[0]=0xF2;
	I2C_MasterSendSend(0b1110110, bm.a, 1, bm.a+47, 1);
	while(i2cCheckStatusTransfer());
	// Отправка настроки температуры и давления и режима работы
	bm.a[0]=0xF4;
	I2C_MasterSendSend(0b1110110, bm.a, 1, bm.a+48, 1);
	while(i2cCheckStatusTransfer());
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(1000);
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
