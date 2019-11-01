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
//#include "main.h"
#include "stm8s.h"
#include "stm8s_it.c"
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*
bit8_t tst;
//uint8_t ask, sr1,sr2,sr3;
//test_t r;
i2c_sr1_t sr1;
i2c_sr2_t sr2;
i2c_sr3_t sr3;
*/

#define Size_Master_Buf 20


typedef struct
{
uint8_t	i2cDeviceAddrRW;
uint8_t i2cMasterSendPlan;
uint8_t i2cMasterReceivePlan;
uint8_t i2cCurrentIndex;
I2CEventBit_t i2cEvent;
uint8_t	i2cMasterBuf[Size_Master_Buf];
}i2cState_t;

i2cState_t i2cState={0,0,0,0,0};




//----------------------------------------------------------------------*
//	Загрузка ДАННЫХ в Master-буфер для ОТПРАВКИ							*
//	Результат функции успешность загрузки данных в буфер				*
ErrorStatus i2cMasterBufUpload(uint8_t data)							//	*
{																	//	*
	while(i2cState.i2cEvent.event); // проверка что передача закончена
	if (i2cState.i2cMasterSendPlan<Size_Master_Buf)								//	*
	{																//	*
	i2cState.i2cMasterBuf[i2cState.i2cMasterSendPlan]=data;							//	*
	i2cState.i2cMasterSendPlan++;											//	*
	return SUCCESS;														//	*
	}																//	*
	else															//	*
	{																//	*
	// i2cMasterSendPlan == Size_Master_Buf								* 
	return ERROR;														//	*
	}																//	*
}																	//	*
//----------------------------------------------------------------------*

//	Отправка данных из Master-буфера									*
//	Результат функции кол-во отправленных байт							*
void i2cMasterBufSend(uint8_t deviceAdd)							//	*
{																	//	*
while(i2cState.i2cEvent.event);
// Очищаем текущий индекс перед оправкой
i2cState.i2cCurrentIndex=0;
i2cState.i2cDeviceAddrRW=(deviceAdd<<1);	// Загружаем Slave АДРЕС+W				*
I2C_GenerateSTART();

}																	//	*
//----------------------------------------------------------------------*
void i2cMasterBufSendReceive(uint8_t deviceAdd, uint8_t kol)
{
	// проверка что передача закончена
	while(i2cState.i2cEvent.event);
	// Записываем кол-во байт для получения
	i2cState.i2cMasterReceivePlan=kol;
	// Проверяем необходимость отправки данных
	if (i2cState.i2cMasterSendPlan) 
		{
			// Необходима отправка данных перед получением 
			// Отправляем данные
			i2cMasterBufSend(uint8_t deviceAdd);
		}
		else
		{
			// Очищаем текущий индекс перед оправкой
			i2cState.i2cCurrentIndex=0;
			// 
			i2cState.i2cDeviceAddrRW=(deviceAdd<<1)|(1);
			I2C_GenerateSTART();
		}
}
//------------------------
uint8_t i2cAddressSend(void)
{
	I2C->DR=i2cState.i2cDeviceAddrRW;
	return ((i2cState.i2cDeviceAddrRW)&1);
}

//----------------------------------------------------------------------*

// Проверка на отправку массива
ErrorStatus i2cMasterBufCheckSend(void)
{
	if (i2cState.i2cMasterSendPlan)
	return SUCCESS; // Массив не отправлен полностью
	else
	{
	//i2cState.i2cMasterSendPlan=0;
	//i2cState.i2cCurrentIndex=0;
	return ERROR;	// Массив полностью отправлен
	}
}
//-------------
// Проверка на посылку массива
uint8_t i2cMasterBufCheckReceive(void)
{
	return i2cState.i2cMasterReceivePlan;
}
//-------------------------------------
// 

void i2cMasterBufReadDataI(void)							//	*
{																	//	*
	i2cState.i2cMasterBuf[i2cState.i2cCurrentIndex++]=I2C->DR;							//	*
	i2cState.i2cMasterSendPlan--;											//	*
}						

//	Результат функции кол-во отправленных байт							*
void i2cMasterReceiveBuf(uint8_t deviceAdd)							//	*
{																	//	*
while(i2cState.i2cEvent.event);
// Очищаем текущий индекс перед оправкой
i2cState.i2cCurrentIndex=0;
i2cState.i2cDeviceAddrRW=(deviceAdd<<1);	// Загружаем Slave АДРЕС+W				*
I2C_GenerateSTART();

}						



// Читает адрес из структуры Для записи
uint8_t i2cAddressWrite(void)
{
	return i2cState.i2cDeviceAddrRW;
}

uint8_t i2cAddressRead(void)
{
	// Очищаем текущий индекс перед оправкой
	i2cState.i2cCurrentIndex=0;
	return i2cState.i2cDeviceAddrRW=i2cState.i2cDeviceAddrRW|1;
}

// считываем структуру
I2C_Event_TypeDef i2cEventGetI(void)
{
	i2cState.i2cEvent.event=I2C_GetLastEvent();
	return i2cState.i2cEvent.event;
}
// можно объединить с предыдущим
void i2cStateSet(I2C_Event_TypeDef state)
{
	i2cState.i2cEvent.event=state;
}



//----------------------------------------
// считываем значение из буфера
uint8_t i2cMasterDownloadBuf(void)
{
	i2cState.i2cMasterSendPlan--;
	return i2cState.i2cMasterBuf[i2cState.i2cCurrentIndex++];
}



/*
void i2cMasterReceive(uint8_t deviceAdd, uint8_t lenght)
{
	if (i2cMasterBufCheck()) i2cMasterSendBuf(deviceAdd);
													else 
	i2cState.i2cMasterReceivePlan=lenght;
	
}

*/



void Task(void)
{
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(100000, 50, 
              I2C_DUTYCYCLE_2, I2C_ACK_NONE, 
              I2C_ADDMODE_7BIT, 16 );
	I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	I2C_Cmd(ENABLE);
	
	i2cMasterUploadBuf(100);
	i2cMasterUploadBuf(101);
	i2cMasterUploadBuf(102);
	i2cMasterSendBuf(0b0111111);	
	i2cMasterUploadBuf(102);
	
	
	
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
