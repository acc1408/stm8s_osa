/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_I2C_SW_C
#define __STM8S_I2C_SW_C

/* Includes ------------------------------------------------------------------*/
#include "inc/stm8s_i2c_sw.h"


// структура для передачи данных
typedef struct
{
	// адрес устройства
	uint8_t addrDevice0_127;
	// адрес памяти
	union
	{
	// адрес памяти
	uint32_t addrMemory;
	// доступ к памяти побайтово
	uint8_t  addrMemoryByte[4];
	};
	// размер адреса памяти устройка
	I2Csw_addrMemorySize_t  memorySize ;
	// порядок передачи байт
	I2Csw_addrMemoryOrder_t memoryOrder;
	
	uint8_t *buf;
	uint16_t bufSize;
} I2Csw_data_t;

// структура для автомата состояний 

// конечные состояния автомата передачи данных
typedef enum
{
	I2Csw_state_success=0, // успешное окончание передачи
	I2Csw_state_noAck=1,	// нет ответа от устройства
	I2Csw_state_timeout=2, // выход по завершению ожидания
	//-----------
	I2Csw_Data_w,
	I2Csw_txMem0_w,
	I2Csw_txMem1_w,
	I2Csw_txMem2_w,
	I2Csw_txMem3_w,
	I2Csw_addrDevice_Mem0_w,
	I2Csw_addrDevice_Mem1_w,
	I2Csw_addrDevice_Mem2_w,
	I2Csw_addrDevice_Mem3_w,
	I2Csw_addrDevice_w,
	//------------------
	I2Csw_rxData_r,
	I2Csw_rxDataPre_r,
	I2Csw_addrDevice_r,
	I2Csw_restart,
	I2Csw_txMem0_r,
	I2Csw_txMem1_r,
	I2Csw_txMem2_r,
	I2Csw_txMem3_r,
	I2Csw_addrDevice_Mem0_r,
	I2Csw_addrDevice_Mem1_r,
	I2Csw_addrDevice_Mem2_r,
	I2Csw_addrDevice_Mem3_r
	//----------------
} I2Csw_stateFSM_t;


/// глобальные  переменные
// состояние шины
I2Csw_stateFSM_t I2Csw_state_curr;
// указатель на текущие ножки
I2Csw_gpio_t*    I2Csw_gpio_curr;
// укзатель на структуру данных
I2Csw_data_t 			I2Csw_data_curr;
// timeoutMs
uint32_t timeout_ms_start;

// инициализация данных
// Результат примерная скорость работы I2C шины kHz

uint8_t I2Csoft_INIT(I2Csw_gpio_t *I2Csw_GPIO,
											GPIO_TypeDef* SdaPort, 
											GPIO_Pin_TypeDef SdaPin,
											GPIO_TypeDef* SclPort,
											GPIO_Pin_TypeDef SclPin,
											uint8_t f_cpu_MHz,
											uint8_t f_i2c_kHz,
											uint8_t timeout_ms
											)
{
	uint32_t k;
	uint16_t n;
	GPIO_Init(SclPort, SclPin, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SdaPort, SdaPin, GPIO_MODE_IN_PU_NO_IT);
	I2Csw_GPIO->SdaPort=SdaPort;
	I2Csw_GPIO->SdaPin=SdaPin;
	I2Csw_GPIO->SclPort=SclPort;
	I2Csw_GPIO->SclPin=SclPin;
	// расчет задержки коэффициента
	k=(uint32_t)f_cpu_MHz*100000/( (uint32_t)16*(uint32_t)f_i2c_kHz);
	if (k<=910)
	{
		I2Csw_GPIO->t=0; // добавил код
	}
	else
	{
		k=(k-800)/11;
		n=k/5;
		if (n>=510)
		{
			I2Csw_GPIO->t=255;
		}
		else
		if (n%2==0)
		{
			n=(n*5)/10;
			I2Csw_GPIO->t=n;
		}
		else
		{
			n=( (n+1)*5)/10;
			I2Csw_GPIO->t=n;
		}
	}
	// расчет частоты
	k=(uint32_t)f_cpu_MHz*10000/(16*(80+11*I2Csw_GPIO->t));
	timeout_ms_start=timeout_ms*f_cpu_MHz*(uint32_t)13+(uint32_t)26;
	return (uint8_t)k;
}

//slaveTest
@inline void I2Csw_SdaInputToOutput(void);

void I2Csw_SdaInputToOutput(void)
{
	I2Csw_gpio_curr->SdaPort ->CR1 |= I2Csw_gpio_curr->SdaPin;
	
	I2Csw_gpio_curr->SdaPort ->DDR |= I2Csw_gpio_curr->SdaPin;
	
	I2Csw_gpio_curr->SdaPort ->CR2 |= I2Csw_gpio_curr->SdaPin;
}

@inline void I2Csw_SdaOutputToInput(void);

void I2Csw_SdaOutputToInput(void)
{
	I2Csw_gpio_curr->SdaPort ->CR1 |= I2Csw_gpio_curr->SdaPin;
	
	I2Csw_gpio_curr->SdaPort->CR2 &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
	
	I2Csw_gpio_curr->SdaPort->DDR &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
}



@inline void I2Csw_SclInputToOutput(void);

void I2Csw_SclInputToOutput(void)
{
	I2Csw_gpio_curr->SclPort ->CR1 |= I2Csw_gpio_curr->SclPin;
	
	I2Csw_gpio_curr->SclPort ->DDR |= I2Csw_gpio_curr->SclPin;
	
	I2Csw_gpio_curr->SclPort ->CR2 |= I2Csw_gpio_curr->SclPin;
}




@inline void I2Csw_SclOutputToInput(void);

void I2Csw_SclOutputToInput(void)
{
	I2Csw_gpio_curr->SclPort ->CR1 |= I2Csw_gpio_curr->SclPin;
	
	I2Csw_gpio_curr->SclPort->CR2 &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
	
	I2Csw_gpio_curr->SclPort->DDR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
}
// 
I2Csw_result_t I2Csw_FSM(void);

I2Csw_result_t I2sw_send( I2Csw_gpio_t *I2Csw_Gpio,
													uint8_t addrDevice0_127,
													uint8_t *data,
													uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// проверка на 7-битность
	I2Csw_state_curr=I2Csw_addrDevice_w;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}



I2Csw_result_t I2Csw_sendToMemoryByte1( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint8_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=I2Csw_addrDevice_Mem0_w;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=I2Csw_addrMemoryByte1;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=I2Csw_LittleEndian;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}

I2Csw_result_t I2Csw_sendToMemoryByte2( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint16_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=I2Csw_addrDevice_Mem1_w;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=I2Csw_addrMemoryByte1;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=I2Csw_LittleEndian;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}

I2Csw_result_t I2Csw_sendToMemoryFull(I2Csw_gpio_t *I2Csw_Gpio,
																			uint8_t  addrDevice0_127,
																			uint32_t addrMemory,
																			I2Csw_addrMemorySize_t  memorySize, 
																			I2Csw_addrMemoryOrder_t memoryOrder,
																			uint8_t *data,
																			uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=(uint8_t)I2Csw_addrDevice_Mem0_w+(uint8_t)memorySize;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=memorySize;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=memoryOrder;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}


I2Csw_result_t I2Csw_read(I2Csw_gpio_t *I2Csw_Gpio,
													uint8_t addrDevice0_127,
													uint8_t *data,
													uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// проверка на 7-битность
	I2Csw_state_curr=I2Csw_addrDevice_r;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}


I2Csw_result_t I2Csw_readFromMemoryByte1( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint8_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=I2Csw_addrDevice_Mem0_r;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=I2Csw_addrMemoryByte1;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=I2Csw_LittleEndian;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}



I2Csw_result_t I2Csw_readFromMemoryByte2( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint16_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=I2Csw_addrDevice_Mem1_r;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=I2Csw_addrMemoryByte1;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=I2Csw_LittleEndian;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}

I2Csw_result_t I2Csw_readFromMemoryFull(I2Csw_gpio_t *I2Csw_Gpio,
																			uint8_t addrDevice0_127,
																			uint32_t addrMemory,
																			I2Csw_addrMemorySize_t  memorySize, 
																			I2Csw_addrMemoryOrder_t memoryOrder,
																			uint8_t *data,
																			uint16_t sizeData)
{
	// Сохраняем указатель для выводов
	I2Csw_gpio_curr=I2Csw_Gpio;
	// присваиваем начальное состояние
	I2Csw_state_curr=(uint8_t)I2Csw_addrDevice_Mem0_r+(uint8_t)memorySize;
	// проверка на 7-битность
	while(addrDevice0_127&0x80);
	I2Csw_data_curr.addrDevice0_127=addrDevice0_127;
	I2Csw_data_curr.addrMemory=addrMemory;
		// размер адреса памяти устройка
	I2Csw_data_curr.memorySize=memorySize;
	// порядок передачи байт
	I2Csw_data_curr.memoryOrder=memoryOrder;
	I2Csw_data_curr.buf=data;
	I2Csw_data_curr.bufSize=sizeData;
	return I2Csw_FSM();
}

//---------------------------------------------

// автомат передачи или примема данных
#define delay_test 10
I2Csw_result_t I2Csw_FSM(void)
{
  uint16_t ta[]={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
	I2Csw_result_t I2Csw_result;
	uint8_t buf,i,ack,index=0,del;
	uint32_t wait;
	uint8_t j;
	///int8_t index=-1;
	//--------------------
	// отсылаем сигнал старт
	//------------------------
	i2cFSM_restart:
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
	// очищаем буфер перед передачей адреса
	//index=0;
	// ожидаем освобождение линии
	I2Csw_SdaOutputToInput();
	I2Csw_SclOutputToInput();
	wait=timeout_ms_start;
	while( !((I2Csw_gpio_curr->SclPort->IDR)&(I2Csw_gpio_curr->SclPin))  )
	{
		if (!wait--)
		{
			I2Csw_state_curr=I2Csw_state_timeout;
			goto i2cFSM_Stop;
		}
	}
	wait=timeout_ms_start;
	while( !((I2Csw_gpio_curr->SdaPort->IDR)&(I2Csw_gpio_curr->SdaPin))  )
	{
		if (!wait--)
		{
			I2Csw_state_curr=I2Csw_state_timeout;
			goto i2cFSM_Stop;
		}
	}
	// отсылаем старт
	I2Csw_gpio_curr->SdaPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
	I2Csw_SdaInputToOutput();
	for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
	//I2Csw_delay(i2cSoft->fall);
	I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
	I2Csw_SclInputToOutput();
	for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
	//I2Csw_delay(i2cSoft->fall);
	// готови
	
	
	i2cFSM_checkState:
	switch(I2Csw_state_curr)
		{
			//------------------------------------
			// Передача байта данных
			case I2Csw_Data_w:
				if (I2Csw_data_curr.bufSize)
				{
					buf=*(I2Csw_data_curr.buf);
					I2Csw_data_curr.buf++;
					I2Csw_data_curr.bufSize--;
					goto i2cFSM_ByteSend;
				}
				else
				{
					I2Csw_state_curr=I2Csw_state_success;
					goto i2cFSM_Stop;
				}
			//-------------------------------	
			// чтение данных I2C
			case I2Csw_rxData_r:
					*(I2Csw_data_curr.buf)=buf;
					I2Csw_data_curr.buf++;
			case I2Csw_rxDataPre_r:
					switch(I2Csw_data_curr.bufSize)
					{
						case 0:
							I2Csw_state_curr=I2Csw_state_success;
							goto i2cFSM_Stop;
						case 1:
							ack=1; // Nack
							break;
						default:
							ack=0; // Ack
					}
					I2Csw_state_curr=I2Csw_rxData_r;
					I2Csw_data_curr.bufSize--;
					goto i2cFSM_ByteReceive;	
			//----------------------------
			// Передача байта адреса памяти			
			case I2Csw_txMem0_w:
			case I2Csw_txMem0_r:
			case I2Csw_txMem1_w:
			case I2Csw_txMem1_r:
			case I2Csw_txMem2_w:
			case I2Csw_txMem2_r:
			case I2Csw_txMem3_w:
			case I2Csw_txMem3_r:
				// загружаем данные для передачи
				buf=I2Csw_data_curr.addrMemoryByte[index];
				if (I2Csw_data_curr.memoryOrder == I2Csw_LittleEndian)
				{
					index++;
				}
				else
				{
					index--;
				}
				// Переходим в другое состояние
				I2Csw_state_curr--;
				goto i2cFSM_ByteSend;
			//------------------------
			// Оправка данных
			case I2Csw_addrDevice_Mem0_w:
			case I2Csw_addrDevice_Mem0_r:
				index++;
			case I2Csw_addrDevice_Mem1_w:
			case I2Csw_addrDevice_Mem1_r:
				index++;
			case I2Csw_addrDevice_Mem2_w:
			case I2Csw_addrDevice_Mem2_r:
				index++;
			case I2Csw_addrDevice_Mem3_w:
			case I2Csw_addrDevice_Mem3_r:
			if (I2Csw_data_curr.memoryOrder != I2Csw_LittleEndian)
				{
					index=3;
				}
				buf=(I2Csw_data_curr.addrDevice0_127)<<1;
				// Переходим в другое состояние
				I2Csw_state_curr  = I2Csw_state_curr-4;
				goto i2cFSM_ByteSend;
			//---------------------------
			// отправка адреса на чтение
			case I2Csw_addrDevice_r:
				buf=((I2Csw_data_curr.addrDevice0_127)<<1)+1;
				// Переходим в другое состояние
				I2Csw_state_curr=I2Csw_rxDataPre_r;
				goto i2cFSM_ByteSend;
			//--------------------------
			// Рестарт для чтения
			case I2Csw_restart:
				I2Csw_state_curr=I2Csw_addrDevice_r;
				goto i2cFSM_restart;	
			//-----------------------
			// отправка адреса на запись
			case I2Csw_addrDevice_w:
				buf=(I2Csw_data_curr.addrDevice0_127)<<1;
				// Переходим в другое состояние
				I2Csw_state_curr=I2Csw_Data_w;
				goto i2cFSM_ByteSend;
			
		}	
	// страховка если что-то пойдет не так
	goto i2cFSM_Stop;	
	//--------------------------
	// блок передачи байта
	i2cFSM_ByteSend:
	// 
		for (i=0;i<8;i++)
		{ 
			
			if (buf&0x80)
			{
				I2Csw_gpio_curr->SdaPort->ODR |= I2Csw_gpio_curr->SdaPin;
			}
			else
			{
				I2Csw_gpio_curr->SdaPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
			}
			//while(ta[i*2]--); // добавил код
			//delay_test(I2Csw_gpio_curr->t ); // добавил
			for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
			if (i==0)
			{
				I2Csw_SdaInputToOutput();
				//----------------------------
				// Поднимаем шину SCL
				//I2Csw_delay(i2cSoft->rise);
				I2Csw_SclOutputToInput();
				// Проверка на отпускание шины	
				wait=timeout_ms_start;
				while( !((I2Csw_gpio_curr->SclPort->IDR)&(I2Csw_gpio_curr->SclPin))  )
				{
					if (!wait--)
					{
						I2Csw_state_curr=I2Csw_state_timeout;
						goto i2cFSM_Stop;
					}
				}
				//------
			}
			else
			{
				del=1;
				while(del--);
			}
			// опускаем шину в ноль
			I2Csw_gpio_curr->SclPort->ODR |= I2Csw_gpio_curr->SclPin;	
			I2Csw_SclInputToOutput();
			//I2Csw_delay(i2cSoft->fall);
			del=1;
			while(del--);
			//while(ta[i*2+1]--); // добавил код
			//delay_test(I2Csw_gpio_curr->t); // добавил
			for(j=0;j<I2Csw_gpio_curr->t;j++); // добавил текст
			I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
			buf=buf<<1;
			//------------------------
		}
		// чтение ACK
		I2Csw_SdaOutputToInput();
		// Поднимаем шину SCL
		//I2Csw_delay(i2cSoft->rise);
		I2Csw_SclOutputToInput();
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		// Проверка на отпускание шины
		wait=timeout_ms_start;
		while( !((I2Csw_gpio_curr->SclPort->IDR)&(I2Csw_gpio_curr->SclPin))  )
		{
			if (!wait--)
			{
				I2Csw_state_curr=I2Csw_state_timeout;
				goto i2cFSM_Stop;
			}
		}
				// wait=I2Csw_downcount;
		// опускаем шину в ноль
		I2Csw_gpio_curr->SclPort->ODR |= I2Csw_gpio_curr->SclPin;	
		I2Csw_SclInputToOutput();
		//I2Csw_delay(i2cSoft->fall);
		//for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		//for(j=0;j<I2Csw_gpio_curr->t;j++); // добавил текст
		// чтение сигнала присутствия 
		ack=(I2Csw_gpio_curr->SdaPort->IDR)&(I2Csw_gpio_curr->SdaPin);
		I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
		if (ack) 
		{
			// Nack
			I2Csw_state_curr=I2Csw_state_noAck;
			goto i2cFSM_Stop;
		}
	goto i2cFSM_checkState;
	//----------------------------
	// Получить 1 байт
	i2cFSM_ByteReceive:
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		for (i=0;i<8;i++)
		{
			buf=buf<<1;
			if (i==0)
			{
				I2Csw_SdaOutputToInput();
				// Поднимаем шину SCL
				//I2Csw_delay(i2cSoft->rise);
				I2Csw_SclOutputToInput();
				// Проверка на отпускание шины
				wait=timeout_ms_start;
				while( !((I2Csw_gpio_curr->SclPort->IDR)&(I2Csw_gpio_curr->SclPin))  )
				{
					if (!wait--)
					{
						I2Csw_state_curr=I2Csw_state_timeout;
						goto i2cFSM_Stop;
					}
				}
				// wait=I2Csw_downcount;
				// опускаем шину в ноль
			}
			I2Csw_gpio_curr->SclPort->ODR |= I2Csw_gpio_curr->SclPin;	
			I2Csw_SclInputToOutput();
			//I2Csw_delay(i2cSoft->fall);
				for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
			// чтение сигнала присутствия 
			if  ((I2Csw_gpio_curr->SdaPort->IDR)&(I2Csw_gpio_curr->SdaPin))
			{
				buf++;
			}
			I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
				for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
			// Используем блок в качестве задержки
			del=0x05;
			while(del--);
		}
		// Отправка Ack
		if (ack)
		{
			// Nack
			I2Csw_gpio_curr->SdaPort->ODR |= I2Csw_gpio_curr->SdaPin;
		}
		else
		{
			// Ack
			I2Csw_gpio_curr->SdaPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
		}
		I2Csw_SdaInputToOutput();
		// Поднимаем шину SCL
		//I2Csw_delay(i2cSoft->rise);
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		I2Csw_SclOutputToInput();
		// Проверка на отпускание шины	
		wait=I2Csw_downcount;
		while( !((I2Csw_gpio_curr->SclPort->IDR)&(I2Csw_gpio_curr->SclPin))  )
		{
			if (!wait--)
			{
				I2Csw_state_curr=I2Csw_state_timeout;
				goto i2cFSM_Stop;
			}
		}
		// wait=I2Csw_downcount;
		// опускаем шину в ноль
		I2Csw_gpio_curr->SclPort->ODR |= I2Csw_gpio_curr->SclPin;	
		I2Csw_SclInputToOutput();
		//I2Csw_delay(i2cSoft->fall);
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
			
	goto i2cFSM_checkState;
	
	//--------------------
	// Передача стоп байта
	i2cFSM_Stop:
		I2Csw_gpio_curr->SdaPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SdaPin));
		I2Csw_SdaInputToOutput();
		//for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		I2Csw_gpio_curr->SclPort->ODR &= (uint8_t)(~(I2Csw_gpio_curr->SclPin));
		I2Csw_SclInputToOutput();
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		
		I2Csw_gpio_curr->SclPort->ODR |= I2Csw_gpio_curr->SclPin;
		//nop();
		I2Csw_SclOutputToInput();
		for(j=0;j<I2Csw_gpio_curr->t;j++); /// добавил ткст
		//I2Csw_delay(i2cSoft->fall);
		I2Csw_gpio_curr->SdaPort->ODR |= I2Csw_gpio_curr->SdaPin;
		//nop();
		I2Csw_SdaOutputToInput();
	//------------------
	//i2cFSM_exit:		
	return I2Csw_state_curr;
}


/*
void I2Csw_delay(uint16_t del)
{
	uint16_t d=del;
	while(d--);
}
*/

#endif /* __STM8S_I2C_H */