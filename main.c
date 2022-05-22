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

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************


uint16_t cnt_on2=0;
//extern  i2cTask_t i2c_Task;
#define i2cBufferSize 5
uint8_t a[2]={0,2};
uint8_t b[4]={3,4,5,6};
I2C_TypeDef* i2c_reg=I2C;


typedef enum
{
	i2cTxTx=0x00,
	i2cTxRx=0x01
}i2cModeTxRx_t;
typedef enum
{
	i2cIdleTxRx=0x00,
	i2cRunTxRx=0x01
}i2cStateTxRx_t;
typedef enum
{
	i2cSuccess=0x00,
	i2cFault=0x01
}i2cResult_t;


typedef struct
{
	// адрес запроса для мастера
	union
	{
		struct
		{
			uint8_t i2cReserv:2;			// резерв
			i2cModeTxRx_t ModeTxRx:1;// Режим передачи
			i2cStateTxRx_t State:1; 	// Состояние линии
			i2cResult_t Result:1; 				// Результат передачи
			uint16_t adr9_0bit:11; 		// 10 бит адрес
		};
		struct
		{
			uint8_t i2cReserv2:5;			// резерв
			uint8_t header; // 3 бита
			uint8_t addr; // 7 бит
		};
	};
	uint8_t*  Pointer; 
	uint8_t 	PointerSize; 
	uint8_t*	Array; 
	uint8_t 	ArraySize;
	
}i2cStack_t;

i2cStack_t i2cStack;

void I2C_Init(uint32_t OutputClockFrequencyHz)
{
  byte16_t result3, result25;
  CCRH_t tmpCCRH;
	uint32_t Clock;
	uint8_t ClockMHz;
	//bufferInit(&i2cStack.i2cBuf, i2cStack.i2cData, i2cBufferSize);
  /* Check the parameters */
  assert_param(IS_I2C_OUTPUT_CLOCK_FREQ_OK(OutputClockFrequencyHz));
	// Enable Peripheral
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	// Get Clock
	Clock= CLK_GetClockFreq();
	ClockMHz=Clock/1000000;
	// Reset I2C
	I2C_SoftwareResetCmd();
	/*
	/*						
	/*------------------------- I2C FREQ Configuration ------------------------*/
  /* Write new value */
  I2C->freq = (uint8_t)(ClockMHz);

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->pe= (uint8_t)0;
  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
	if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpCCRH.ccrh=1<<7;
    result3.byte16  = (uint16_t) ((Clock ) / (OutputClockFrequencyHz * 3));
		result25.byte16 = (uint16_t) ((Clock ) / (OutputClockFrequencyHz * 25));
		if (result3.byte16*3<result25.byte16*25)
		{
			// Set Tlow/Thigh=16/9 
			tmpCCRH.duty=1;
		}
		else
		{
			result25.byte16=result3.byte16;
		}
		// Verify and correct CCR value if below minimum value 
    if (result25.byte16 < (uint16_t)0x01)
    {
      // Set the minimum allowed value 
      result25.byte16 = (uint16_t)0x0001;
    }
		 /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
   I2C->TRISER = (((uint8_t)ClockMHz * 3) / 10) + 1;
	}
  else /* STANDARD MODE */
  {
    tmpCCRH.ccrh=0;
		/* Calculate standard mode speed */
    result25.byte16 = (uint16_t)((Clock ) / (OutputClockFrequencyHz << (uint8_t)1));
		
    /* Verify and correct CCR value if below minimum value */
    if (result25.byte16 < (uint16_t)0x0004)
    {
      /* Set the minimum allowed value */
      result25.byte16 = (uint16_t)0x0004;
    }
    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(ClockMHz + (uint8_t)1);
  }
  /* Write CCR with new calculated value */
  I2C->CCRL = result25.bytelow;
  tmpCCRH.ccr11_8=result25.bytehigh;
	I2C->CCRH =tmpCCRH.ccrh;
  
  /* Configure I2C acknowledgement */
	// I2C->ack=0;
	// I2C->pos=0;
  /*--------------------------- I2C OAR Configuration ------------------------*/
  //I2C->OARL = (uint8_t)(OwnAddress);
	//I2C->OARH=1<<6; // enable 7bit Mode
	I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	/* Enable I2C */
  I2C->pe=1;
	
}

uint8_t i2cMaster7BitSendSend(uint8_t DeviceAddress, 
															void*   Pointer, 
															uint8_t PointerSize, 
															void*   Array, 
															uint8_t ArraySize)
{
	
	if (!I2C->busy)
	{
		// режим записи данных
		//i2cStack.Flag=send;
		// настраиваем режим передачи адреса в 7битном режиме
		I2C->OARH=(I2C->OARH&0x06)||0x40;
		// записываем адрес для записи
		i2cStack.addr=DeviceAddress<<1;
		i2cStack.Pointer=(uint8_t*)Pointer;
		i2cStack.PointerSize=PointerSize;
		i2cStack.Array=Array;
		i2cStack.ArraySize=ArraySize;
		i2cStack.ModeTxRx=i2cTxTx;
		i2cStack.State=i2cRunTxRx;
		i2cStack.Result=i2cSuccess;
		I2C->start=1;
		return 0;
	}
	return 1;
}

uint8_t i2cMaster7BitSendReceive(uint8_t DeviceAddress, 
															void*   Pointer, 
															uint8_t PointerSize, 
															void*   Array, 
															uint8_t ArraySize)
{
	
	if (!I2C->busy)
	{
		// режим записи данных
		//i2cStack.Flag=send;
		// настраиваем режим передачи адреса в 7битном режиме
		I2C->OARH=(I2C->OARH&0x06)||0x40;
		// записываем адрес для записи
		i2cStack.addr=DeviceAddress<<1;
		i2cStack.Pointer=(uint8_t*)Pointer;
		i2cStack.PointerSize=PointerSize;
		i2cStack.Array=Array;
		i2cStack.ArraySize=ArraySize;
		i2cStack.ModeTxRx=i2cTxRx;
		i2cStack.State=i2cRunTxRx;
		i2cStack.Result=i2cSuccess;
		I2C->start=1;
		return 0;
	}
	return 1;
}


void interrupt_i2c(void)
{
	// Обработка ошибок
	if (I2C->SR2)
	{
		// ошибка передачи/приема
		//i2cStack.Flag=idle;
		// нет ответа
		if(I2C->af)
		{
			I2C->af=0;// сброс флага
			I2C->stop=1; // генерация STOP
		}
		
		// ошибка на шине
		if (I2C->berr)
		{
			I2C->berr=0; // сброс флага
			I2C->stop=1; // генерация STOP
		}
		// потеря тайминга
		if (I2C->ovr)
		{
			I2C->ovr=0;
		}
		// потеря арбитража
		if (I2C->arlo)
		{
			I2C->arlo=0;
		}
		// slave in Halt mode
		if (I2C->wufh)
		{
			I2C->wufh=0;
		}
		return;
	}
	// 
	if (I2C->msl)
	{
		// если режим мастера и стартовый бита передан
		if (I2C->sb)
		{
			
			// определние режима адресации
			if (I2C->addmode)
			{
				// 10-bit address
				I2C->DR=i2cStack.header;				
			}
			else
			{
				// 7-bit address
				I2C->DR=i2cStack.addr;
				// Если идет чтение данных
				if (i2cStack.addr&0x01)
				{
					// проверяем сколько байт необходимо скачать
					switch(i2cStack.ArraySize)
					{
						case 0:
							I2C->stop=1;
							break;
						case 1:
							break;
						default:
							I2C->ack=1;
					}
				}
				
				return;
			}
		}
		// отправляет адрес
		if (I2C->add10)
		{
			I2C->DR=i2cStack.addr;
			return;
		}
		if (I2C->tra)
		{
			if (i2cStack.ModeTxRx==i2cTxTx)
			{
				// режим запись-запись
				if (i2cStack.PointerSize)
				{
					I2C->DR=*i2cStack.Pointer;
					i2cStack.PointerSize--;
					i2cStack.Pointer++;
					return;
				}
				if (i2cStack.ArraySize)
				{
					I2C->DR=*i2cStack.Array;
					i2cStack.ArraySize--;
					i2cStack.Array++;
					return;
				}
				I2C->stop=1;
				return;
			}
			else
			{
				// режим запись-чтение
				if (i2cStack.PointerSize)
				{
					I2C->DR=*i2cStack.Pointer;
					i2cStack.PointerSize--;
					i2cStack.Pointer++;
					return;
				}
				// Проверка на необходимость чтения
				if (i2cStack.ArraySize)
				{
					i2cStack.addr|=1; // Режим чтения
					I2C->start=1;
				}
				else 
				{
					I2C->stop=1;
				}
			}
			
		}
		else
		{
			// режим мастера-приемника
			switch (i2cStack.ArraySize)
			{
				// нет данных для чтения
				
				case 1:
					i2cStack.ArraySize--;
					*i2cStack.Array=I2C->DR;
					I2C->stop=1;
					break;
				case 2:
					I2C->ack=0;
					i2cStack.ArraySize--;
					*i2cStack.Array=I2C->DR;
					
				default:
					i2cStack.ArraySize--;
					*i2cStack.Array=I2C->DR;
					I2C->ack=1;
					i2cStack.Array++;
			}
			
			if (i2cStack.ArraySize)
				{
					I2C->DR=*i2cStack.Array;
					i2cStack.ArraySize--;
					i2cStack.Array++;
					return;
				}
				I2C->stop=1;
		}
			
		
	}
	
	//i2c_Task.
}
	/*
	uint8_t*  Pointer; 
	uint8_t 	PointerSize; 
	uint8_t*	Array; 
	uint8_t 	ArraySize;
	*/
// отключения и включение светодиода
int16_t led(uint8_t argc,char *argv[])
{
	if (argc==2)  
	{
		if (strcmp(argv[1],"on")==0) // включаем светодиод
		{
			cnt_on2++;
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
			printf("Светодиод включен %d\r\n",cnt_on2);
			return 0;
		}
		else 
			if (strcmp(argv[1],"off")==0)  // отключаем светодиод
			{
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
				printf("Светодиод отключен\r\n");
				return 0;
			}
			else
			{
				return 2;
			}
	}
	else
		return 1;
}


uint8_t bl=1; // переменная для включения и отключения мигания
// функция для включения и отключения мигания
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc==2) 
	{
		if (strcmp(argv[1],"on")==0) // отключаем светодиод
			bl=1;
		else 
			if (strcmp(argv[1],"off")==0) // включаем светодиод
				bl=0;
		return 0;
	}
	return 1;
}

// таблица соответствия название-функция храниться не в ОЗУ,
// а во FLESH, т.е. можно не беспокопиться за кол-во функций
// и длину строк

const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};
// таблица соответсвия возврат-текст для резултататов возврата

const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};

//char simvol;		
//============================ вывод данных из UART
#ifndef _INPUTCHAR_T
#define _INPUTCHAR_T
typedef void (*inputchar_t)(char);
#endif





//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
	}
}
#endif


uint16_t nmb=0;
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
	Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	// Инициализация командной строки.
	
	cmdinit('\r', // стоп-символ
					'\n', // 2 стоп-символ
					testf, // указатель на таблицу название-функция
					2,  	// кол-во функций в таблице
					RetText, // таблица номер-возврат текста
					2);		// размер массива номер-текст
	/*
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// настройка приема символов из uart2 в командную строку
	UART2_SetRxHandler(cmdinputchar);
	//UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// Инициализация для вывода printf
	stdio_InitPrintf(uart2_sendtobuffer);
	UART2_Cmd(ENABLE);
	*/
	I2C_Init(100000);
	enableInterrupts();
	/*
	i2cMaster7BitSendSend(0b1111000, 
															a, 
															2, 
															b, 
															4);
	*/
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	printf("Check cmdline\r\n");
	while (1)
  {
		cmdmainloop(); // обработка командной строки
	//	printf("qwe5675089tr %d\r\n",nmb);
		nmb++;
		/*
		i2cMaster7BitSendSend(0b1010111, 
															a, 
															2, 
															b, 
															4);
	*/
	i2cMaster7BitSendReceive(0b1010111, 
															a, 
															2, 
															b, 
															4);
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(100);
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
