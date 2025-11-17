/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_I2C_SW_H
#define __STM8S_I2C_SW_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_gpio.h"
// счетчик таймаута
#define I2Csw_downcount 0x3FFF

// структура для инициалиазции ножек I2C 
typedef struct
{
	GPIO_TypeDef* SdaPort; 
	GPIO_Pin_TypeDef SdaPin;
	GPIO_TypeDef* SclPort;
	GPIO_Pin_TypeDef SclPin;
	uint8_t t;
} I2Csw_gpio_t;

// размер адреса памяти
typedef enum
{
	I2Csw_addrMemoryByte1=0,
	I2Csw_addrMemoryByte2=1,
	I2Csw_addrMemoryByte3=2,
	I2Csw_addrMemoryByte4=3
}I2Csw_addrMemorySize_t;

// порядок пересылки адреса карты памяти 
typedef enum
{
	I2Csw_LittleEndian=0,
	I2Csw_BigEndian
}I2Csw_addrMemoryOrder_t;

/// для передачи или приема байт по I2C 
typedef enum
{
	I2Csw_success=0,
	I2Csw_noAck=1,
	I2Csw_timeout=2
}I2Csw_result_t;


//---------------------------------------

// инициализация данных
// Результат примерная скорость работы I2C шины kHz
uint8_t I2Csoft_INIT(I2Csw_gpio_t *I2Csw_Gpio,
											GPIO_TypeDef* SdaPort, 
											GPIO_Pin_TypeDef SdaPin,
											GPIO_TypeDef* SclPort,
											GPIO_Pin_TypeDef SclPin,
											uint8_t f_cpu_MHz,
											uint8_t f_i2c_kHz,
											uint8_t timeout_ms
											);

I2Csw_result_t I2sw_send( I2Csw_gpio_t *I2Csw_Gpio,
													uint8_t addrDevice0_127,
													uint8_t *data,
													uint16_t sizeData);


I2Csw_result_t I2Csw_sendToMemoryByte1( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint8_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData);
// По-умолчанию LittleEndian
I2Csw_result_t I2Csw_sendToMemoryByte2( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint16_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData);
																				
I2Csw_result_t I2Csw_sendToMemoryFull(I2Csw_gpio_t *I2Csw_Gpio,
																			uint8_t  addrDevice0_127,
																			uint32_t addrMemory,
																			I2Csw_addrMemorySize_t  memorySize, 
																			I2Csw_addrMemoryOrder_t memoryOrder,
																			uint8_t *data,
																			uint16_t sizeData);
																			
I2Csw_result_t I2Csw_read(I2Csw_gpio_t *I2Csw_Gpio,
													uint8_t addrDevice0_127,
													uint8_t *data,
													uint16_t sizeData);
													
I2Csw_result_t I2Csw_readFromMemoryByte1( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint8_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData);
// По-умолчанию LittleEndian
I2Csw_result_t I2Csw_readFromMemoryByte2( I2Csw_gpio_t *I2Csw_Gpio,
																				uint8_t addrDevice0_127,
																				uint16_t addrMemory,
																				uint8_t *data,
																				uint16_t sizeData);

I2Csw_result_t I2Csw_readFromMemoryFull(I2Csw_gpio_t *I2Csw_Gpio,
																			uint8_t addrDevice0_127,
																			uint32_t addrMemory,
																			I2Csw_addrMemorySize_t  memorySize, 
																			I2Csw_addrMemoryOrder_t memoryOrder,
																			uint8_t *data,
																			uint16_t sizeData);
																			

#endif /* __STM8S_I2C_H */