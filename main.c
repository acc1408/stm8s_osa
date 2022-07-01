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
uint8_t a[]={0x00,0x05,0x42,0x13,0x02};

uint16_t cnt_on2=0;
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
/*
const tablefunc_t testf[2]={{"led", led},
													  {"blink", blink	}
														};*/
// таблица соответсвия возврат-текст для резултататов возврата
/*
const tableRet_t RetText[2]={
															 0, "ok\r\n",
															 2,"Led error param\r\n"
														};
*/
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
//-----
typedef enum
{
	i2cSuccess=0,
	i2cBusError=1,
	i2cArbitLost=2,
	i2cNack=3,
	i2cOverrun=4,
	i2cWakeup=5,
	i2cBusy=6
}i2cStatus_t;

i2cStatus_t res;
void I2C_restartInit(void)
{
	//uint8_t I2C_CR1,I2C_CR2,I2C_FREQR,	I2C_OARL,I2C_OARH,
	//I2C_ITR,I2C_CCRL,	I2C_CCRH,I2C_TRISER;
	uint8_t i,i2cSetting[9];
/* Disable I2C to configure TRISER */
  I2C->pe= (uint8_t)0;
	//-- Save config I2C
	for(i=0;i<5;i++)
	{
		i2cSetting[i]=*((uint8_t*)I2C+i);
	}
	for(i;i<9;i++)
	{
		i2cSetting[i]=*((uint8_t*)I2C+i+5);
	}
	//reset I2C
	I2C->swrst=1;
	nop();
	I2C->swrst=0;
	//-- Download config I2C
	for(i=0;i<5;i++)
	{
		*((uint8_t*)I2C+i)=i2cSetting[i];
	}
	for(i;i<9;i++)
	{
		*((uint8_t*)I2C+i+5)=i2cSetting[i];
	}
	// Enable I2C
	I2C->pe= (uint8_t)1;
}

i2cStatus_t I2C_MasterSend(uint8_t DeviceAddress, void *ArraySend, uint16_t NumSend)
{
	uint16_t i=0;
	//uint8_t stBit=0;
	i2cStatusReg_t SR;
	uint8_t *ArSend=(uint8_t *)ArraySend;
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			goto HandlerErrorI2CSend;
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			I2C->start=1;  // generation start-bit
		}
		else
		{
			//if (SR.msl||SR.busy)
					I2C_restartInit(); 
			return i2cBusy;
		}
	//i=0;
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//Begin Safe block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			
			I2C_restartInit(); 
			return i2cBusy;
		}
		//End Safe block
		//---------
		//Begin Handle Error
		if (SR.SR2)
		{
			HandlerErrorI2CSend:
			if (SR.berr)  //Error 
			{
				I2C->berr=0; // na?in oeaaa
				//i2cStack.busError=1;
				I2C_restartInit(); 
				//I2C->stop=1; // aaia?aoey STOP
				return i2cBusError;
			}
			if (SR.af)  //Error 
			{
				I2C->af=0;
				I2C->stop=1;
				return i2cNack;			
			}	
			if (SR.arlo)  //Error 
			{
				I2C->arlo=0;
				//I2C->stop=1;
				return i2cArbitLost;			
			}	
			if (SR.ovr)  //Error 
			{
				I2C->ovr=0;
				//I2C->stop=1;
				return i2cOverrun;			
			}	
			if (SR.wufh)  //Error 
			{
				I2C->wufh=0;
				//I2C->stop=1;
				return i2cWakeup;			
			}
			return i2cBusError;
		}
		//End Handle Error
		//------------
		//---Begin Send Adress-----
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1;
			//if (!NumSend)
				
		}
		//---End Send adress-----
		//----------------------
		// 
		if (SR.addr)
		{
			switch(NumSend)
			{
				case 1:
					I2C->DR=*ArSend++;
					NumSend--;
					break;
				case 0:	
					I2C->stop=1;
					return i2cSuccess;
					break;	
				default:
					I2C->DR=*ArSend++;
					I2C->DR=*ArSend++;
					NumSend=NumSend-2;
					//SR.txe=0;
			}
			continue;
		}
		//-------------
		if (SR.txe)
		{
			switch(NumSend)
			{
				case 0:
					if (SR.btf)
					{
						I2C->stop=1;
						return i2cSuccess;
					}
					break;
				case 1:
					I2C->DR=*ArSend++;
					NumSend--;
					break;
					//I2C->stop=1;
					//return 0;
				break;
					default:
					I2C->DR=*ArSend++;
					//I2C->ack=1;
					NumSend--;
					i=0;
			}
		}
		
	}
	//----------------
}

i2cStatus_t I2C_MasterReceive(uint8_t DeviceAddress, void *ArrayReceive, uint16_t NumReceive)
{
	uint16_t i=0;
	i2cStatusReg_t SR;
	uint8_t *ArReceive=(uint8_t *)ArrayReceive;
	//I2C->start=1;  // Генерация старта
	//if (!SR.busy )
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			goto HandlerErrorI2CReceive;
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			I2C->start=1;  // generation start-bit
		}
		else
		{
			//if (SR.msl||SR.busy)
					I2C_restartInit(); 
			return i2cBusy;
		}
	
	
	//i=0;
	while(1)
	{
		//repeat_busy:
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		
		if (SR.SR2)
		{
			HandlerErrorI2CReceive:
			if (SR.berr)  //Error 
			{
			I2C->berr=0; // na?in oeaaa
			//i2cStack.busError=1;
			I2C_restartInit(); 
			//I2C->stop=1; // aaia?aoey STOP
			return i2cBusError;
			}
			if (SR.af)  //Error 
			{
			I2C->af=0;
			I2C->stop=1;
			return i2cNack;			
			}	
			if (SR.arlo)  //Error 
			{
			I2C->arlo=0;
			//I2C->stop=1;
			return i2cArbitLost;			
			}	
			if (SR.ovr)  //Error 
			{
				I2C->ovr=0;
				//I2C->stop=1;
				return i2cOverrun;			
			}	
			if (SR.wufh)  //Error 
			{
				I2C->wufh=0;
				//I2C->stop=1;
				return i2cWakeup;			
			}	
			return i2cBusError;
		}
		//---Send adress-----
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1|1;
			switch(NumReceive)
			{
				case 0:
					//I2C->DR=DeviceAddress<<1|1;
					I2C->stop=1;
					
					break;
				case 1:
					//I2C->DR=DeviceAddress<<1|1;
					I2C->ack=0;
					break;
				default:
					//I2C->DR=DeviceAddress<<1|1;
					I2C->ack=1;
					break;
			}
			
		}
		
		if (SR.addr)
		{
			switch(NumReceive)
			{
				case 0:
					return i2cSuccess;
				case 1:
					I2C->stop=1;
					break;	
				default:
					I2C->ack=1;
			}
		}
		
		if (SR.rxne)
		{
			switch(NumReceive)
			{
				case 0:
					return i2cSuccess;
				case 1:
					*ArReceive=I2C->DR;
					return i2cSuccess;
				case 2:
					*ArReceive++=I2C->DR;
					I2C->ack=0;
					I2C->stop=1;
					NumReceive--;
					break;	
				default:
					*ArReceive++=I2C->DR;
					I2C->ack=1;
					NumReceive--;
					
			}
		}
		
	}
	//----------------	
	
		
}

i2cStatus_t I2C_MasterSendPtrSendData(uint8_t DeviceAddress,void *Ptr, uint16_t PtrSize, void *ArraySend, uint16_t NumSend)
{
	uint16_t i=0;
	i2cStatusReg_t SR;
	uint8_t *ArSend=(uint8_t *)ArraySend;
	//I2C->start=1;  // Генерация старта
	//if (!SR.busy )
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
			//I2C->ack=0;
			if (!SR.busy )
			{
				I2C->start=1;  // generation start-bit
				i=0;
				break;
			}
		// Begin Safe Block	
		if (i<10000) 
			i++;
			else
			{
				if (SR.SR3==0b00000010)
				I2C_restartInit(); 
			return i2cBusy;
			}
			
		if (SR.SR2)
			break;
		// End Safe Block	
	};
	i=0;
	while(1)
	{
		
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		// Begin Safe Block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		
		if (SR.SR2)
		{
			if (SR.berr)  //Error 
			{
				
			I2C->berr=0; // na?in oeaaa
			//i2cStack.busError=1;
			I2C_restartInit(); 
			//I2C->stop=1; // aaia?aoey STOP
			return i2cBusError;
			}
			if (SR.af)  //Error 
			{
			I2C->af=0;
			I2C->stop=1;
			return i2cNack;			
			}	
			if (SR.arlo)  //Error 
			{
			I2C->arlo=0;
			//I2C->stop=1;
			return i2cArbitLost;			
			}	
			if (SR.ovr)  //Error 
			{
				I2C->ovr=0;
				//I2C->stop=1;
				return i2cOverrun;			
			}	
			if (SR.wufh)  //Error 
			{
				I2C->wufh=0;
				//I2C->stop=1;
				return i2cWakeup;			
			}	
		}
		// End Safe Block	
		//-------------------
		//---Send start-----
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1;
			continue;
		}
		if (SR.addr)
		{
			switch(NumSend)
			{
				case 1:
					I2C->DR=*ArSend++;
					NumSend--;
				case 0:	
					SR.txe=0;
					break;	
				default:
					I2C->DR=*ArSend++;
					I2C->DR=*ArSend++;
					NumSend--;
					NumSend--;
					SR.txe=0;
			}
			continue;
		}
		if (SR.txe)
		{
			switch(NumSend)
			{
				case 0:
				//I2C->ack=0;
				//NumSend--;
					break;
				case 1:
					I2C->DR=*ArSend++;
					NumSend--;
					break;
			
				//I2C->stop=1;
				//return 0;
				break;
					default:
					I2C->DR=*ArSend++;
					//I2C->ack=1;
					NumSend--;
			}
		}
	if (!NumSend)
	{
		if (SR.btf)
		{
			I2C->stop=1;
			return i2cSuccess;
		}
	}
	
	
			
	}
	//----------------	
	
		
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
//	cf_u=&cf;
	uint8_t i;
	Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	// Инициализация командной строки.
	/*
	cmdinit('\r', // стоп-символ
					'\n', // 2 стоп-символ
					testf, // указатель на таблицу название-функция
					2,  	// кол-во функций в таблице
					RetText, // таблица номер-возврат текста
					2);		// размер массива номер-текст*/
	/*				
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	// настройка приема символов из uart2 в командную строку
	//UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	// Инициализация для вывода printf
	stdio_InitPrintf(uart2_sendtobuffer);
	UART2_Cmd(ENABLE);
	
	enableInterrupts();
	*/
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	//printf("Check cmdline\r\n");
	
	I2C_Init_7bit(100000);
	//I2C_MasterSend(0x57, a, 1);
	while (1)
  {
		//cmdmainloop(); // обработка командной строки
	//	printf("qwe5675089tr %d\r\n",nmb);
	res=I2C_MasterSend(0x68, a, 1);
	res=I2C_MasterReceive(0x68, &a[1],3);
		nmb++;
	//if (bl) 
	GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
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
