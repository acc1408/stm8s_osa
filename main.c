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
SetLCD_t lcd1;

//const char st[]="Hello world!!!qwertyuiopasdffghjj";


union {
uint8_t a[30];
struct{
uint8_t address;
uint16_t dig_T1;
 int16_t dig_T2;
 int16_t dig_T3;
uint16_t dig_P1;
 int16_t dig_P2;
 int16_t dig_P3;
 int16_t dig_P4;
 int16_t dig_P5;
 int16_t dig_P6;
 int16_t dig_P7;
 int16_t dig_P8;
 int16_t dig_P9;
 uint8_t dig_H1;

 
};
}bm;

i2cTask_t i2c_Task={0,0,0,0,0,0,0,0};

// Функция передает в начале массив адреса регистра, а затем передает данные.
void i2c_MasterSendSend(uint8_t DeviceAddress, uint8_t *ArrayAddress, uint8_t NumAddress, uint8_t *ArraySend, uint8_t NumSend)
{
	while(i2c_Task.Func);
	i2c_Task.Func=i2cSendSend; // Загрузка функции
	i2c_Task.DeviceAddrRW=DeviceAddress<<1;//загрузка адреса
	i2c_Task.ArraySend=ArrayAddress; // загрузка указателя на адрес массива
	i2c_Task.NumSend=NumAddress;// загрузка кол-во байт для передачи
	i2c_Task.ArrSendReceive=ArraySend; // загрузка указателя массива на данные 
	i2c_Task.NumSendReceive=NumSend; // загрузка кол-во данных для передачи
	i2c_Task.CurrentIndex=0;
	i2c_Task.Error=0;
	I2C->start=1;  // Генерация старта
}

void i2c_MasterSendReceive(uint8_t DeviceAddress, uint8_t *ArrSend, uint8_t NumSend, uint8_t *ArrReceive, uint8_t NumReceive)
{
	while(i2c_Task.Func);
	i2c_Task.Func=i2cSendReceive; // Загрузка функции
	switch(NumSend)
	{
		case 0:
			i2c_Task.DeviceAddrRW=(DeviceAddress<<1)|1;//загрузка адреса на передачу
		break;
		default:
			i2c_Task.DeviceAddrRW=DeviceAddress<<1;//загрузка адреса на передачу
	}
	
	i2c_Task.ArraySend=ArrSend; // загрузка указателя на адрес массива
	i2c_Task.NumSend=NumSend;// загрузка кол-во байт для передачи
	i2c_Task.ArrSendReceive=ArrReceive; // загрузка указателя массива на данные 
	i2c_Task.NumSendReceive=NumReceive; // загрузка кол-во данных для передачи
	i2c_Task.Error=0;
	I2C->start=1;  // Генерация старта
}


uint8_t a[]={1,2,3,4,5,6,7,8,9,10};
void i2cHandler(void)
{
	i2c_Task.ItEvent=i2cEventGet();
	switch(i2c_Task.ItEvent)
	{
		case I2C_EVENT_MASTER_MODE_SELECT: /*!< BUSY, MSL and SB flag */ 
		{	
				switch(i2c_Task.Dir)
				{
					case I2C_DIRECTION_TX:
					{	
						I2C->DR=i2c_Task.DeviceAddrRW;
					}
					break;
					case I2C_DIRECTION_RX:
					{
						switch(i2c_Task.NumSend||i2c_Task.NumSendReceive)
						{
							case 0: 
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->stop=1;
								break;
							case 1:
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->ack=0;
								break;
							default:
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->ack=1;
								break;
						}
					}
					break;
				}
		}
		break;
		
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: //!< BUSY, MSL, TRA, ADDR, TXE  				flags 
		//
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:	//!< 				BUSY, MSL, TRA, 			TXE 				flags 
		//
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:	//!< EV8_2: 	BUSY, MSL, TRA, 			TXE and BTF flags 
		{
			switch(i2c_Task.Func)
			{
				case i2cSendSend:
				{	
					switch(i2c_Task.NumSend)
					{
						case 0:
						{	
							switch(i2c_Task.NumSendReceive)
							{
								case 0:
								{
									I2C->stop=1;
									i2c_Task.Func=i2cIdle;
									i2c_Task.CurrentIndex=0;
								}
								break;
								case 1:
								{
									I2C->DR=i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++];
									i2c_Task.NumSendReceive--;
									i2c_Task.CurrentIndex=0;
								}
								break;
								default:
								{
									I2C->DR=i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++];
									i2c_Task.NumSendReceive--;
								}
								break;
							}
						}
						break;
						case 1:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
							i2c_Task.CurrentIndex=0;
						}
						break;
						default:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
						}
						break;
					}
				}
				break;
				// Обработка функции отправить и принять
				case i2cSendReceive:
				{
					switch(i2c_Task.NumSend)
					{
						case 0:
						{
							switch(i2c_Task.NumSendReceive)
							{
								case 0:
								{
									I2C->stop=1;
									i2c_Task.Func=i2cIdle;
									i2c_Task.CurrentIndex=0;
								}
								break;
								default:
								{
									i2c_Task.Dir=I2C_DIRECTION_RX;
									I2C->start=1;  // Генерация старта
								}
							}
						}
						break;
						case 1:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
							i2c_Task.CurrentIndex=0;
						}
						break;
						default:
						I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
						i2c_Task.NumSend--;
					}
				}
				break;
			};
		}
		break;
		
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:	/*!< BUSY, MSL and ADDR						flags */
		{
			switch(i2c_Task.NumSendReceive)
			{
				case 1:
				I2C->ack=0;
				break;
				default:
				I2C->ack=1;
			}
		}
		break;
		
		case I2C_EVENT_MASTER_BYTE_RECEIVED:					/*!< BUSY, MSL and 			RXNE 			flags */
		case I2C_EVENT_MASTER_2_BYTE_RECEIVED:				/*!< BUSY, MSL and 			RXNE BTF 	flags */
		{
			//i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++]=I2C->DR;
			//i2c_Task.NumSendReceive--;

			switch(i2c_Task.NumSendReceive)
			{
				
				case 1:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex]=I2C->DR;
					i2c_Task.NumSendReceive--;
					i2c_Task.Func=i2cIdle;
					i2c_Task.CurrentIndex=0;
				}
				break;
				case 2:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++]=I2C->DR;
					i2c_Task.NumSendReceive--;
					I2C->ack=0;
					I2C->stop=1;
					
				}
				break;
				default:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++]=I2C->DR;
					i2c_Task.NumSendReceive--;
					I2C->ack=1;
					
				}
				break;
			}
		}
		break;
		case I2C_EVENT_SLAVE_ACK_FAILURE:
		{
			I2C->stop=1;
			i2c_Task.Func=i2cIdle;
			i2c_Task.Error=I2C->SR2; 
			I2C->SR2=0;
		}
		break;
	}			
}
//uint8_t a[]={1,2,3,4,5,6,7,8,9,10};]


void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	//Lcdi2cInit(&lcd1, 0b0111111);
	// Lcdi2cPrint(&lcd1, st);
	I2C_MasterInit(100000);	
	i2c_MasterSendSend(0b0111111, a, 2, a+3, 4);
	i2c_MasterSendReceive(0b0111111, a, 2, a+5, 3);
	//bm.a[0]=0x88;
	//bm.a[0]=0xD0;
	//i2c_MasterSendReceive(0b1110110, bm.a, 1, bm.a+1, 8);
	/*
	while(i2cCheckStatusTransfer());
	
	for(i=1;i<24;i=i+2)
	{
		temp=bm.a[i];
		bm.a[i]=bm.a[i+1];
		bm.a[i+1]=temp;
	}
	*/
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
