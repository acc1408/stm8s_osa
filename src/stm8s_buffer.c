/**
  ******************************************************************************
  * @file    stm8s_buffer.c
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the circular buffer .
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_BUFFER_C
#define __STM8S_BUFFER_C

/* Includes ------------------------------------------------------------------*/
#include "inc/stm8s_buffer.h"
// безопасна€ секци€ 
#ifndef CRITICAL_SECTION_START
char cc_reg; // регистр дл€ хранени€ регистра состо€ний
#define CRITICAL_SECTION_START _asm("push CC \n sim \n pop _cc_reg \n");
#define CRITICAL_SECTION_END	 _asm("push _cc_reg \n pop CC \n") //
#endif

// инициализаци€ буфера 
void bufferInit(cBuffer_t* buffer, uint8_t *start, uint16_t size)
{
	
	// begin critical section
	CRITICAL_SECTION_START;
	// set start pointer of the buffer
	buffer->dataptr = start; // присвоение указател€
	buffer->size = size;	// присвоение размера
	// initialize index and length
	buffer->dataindex = 0;
	buffer->datalength = 0;
	// end critical section
	CRITICAL_SECTION_END;
}

// получить данные из буфера
uint8_t  bufferGetFromFront(cBuffer_t* buffer)
{
	uint8_t data = 0; 
	// begin critical section
	CRITICAL_SECTION_START;
	// check to see if there's data in the buffer
	if(buffer->datalength)
	{
		// get the first character from buffer
		data = buffer->dataptr[buffer->dataindex];
		// move index down and decrement length
		buffer->dataindex++;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength--;
	}
	// end critical section
	CRITICAL_SECTION_END;
	// return
	return data;
}

// добавить данные в буфер 
uint8_t bufferAddToEnd(cBuffer_t* buffer, uint8_t data)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// make sure the buffer has room
	if(buffer->datalength < buffer->size)
	{
		// save data byte at end of buffer
		buffer->dataptr[(buffer->dataindex + buffer->datalength) % buffer->size] = data;
		// increment the length
		buffer->datalength++;
		// end critical section
		CRITICAL_SECTION_END;
		// return success
		return 0;
	}
	// end critical section
	CRITICAL_SECTION_END;
	// return failure
	return -1;
}




// очистить данные в буфере
void bufferFlush(cBuffer_t* buffer)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// flush contents of the buffer
	buffer->datalength = 0;
	// end critical section
	CRITICAL_SECTION_END;
}

// проверка сколько €чеек буфера свободно
uint16_t bufferIsNotFull(cBuffer_t* buffer)
{
	uint16_t bytesleft;
	// begin critical section
	CRITICAL_SECTION_START;
	// check to see if the buffer has room
	// return true if there is room
	bytesleft = (buffer->size - buffer->datalength);
	// end critical section
	CRITICAL_SECTION_END;
	return bytesleft;
}

// проверка сколько €чеек зан€то
uint16_t bufferIsEmpty(cBuffer_t* buffer)
{
	uint16_t bytesbusy;
	// begin critical section
	CRITICAL_SECTION_START;
	// check to see if the buffer has room
	// return true if there is room
	bytesbusy =  buffer->datalength;
	// end critical section
	CRITICAL_SECTION_END;
	return bytesbusy;
}


// освободить последние numbytes €чеек в буфере от данных
void bufferDumpFromFront(cBuffer_t* buffer, uint16_t numbytes)
{
	// begin critical section
	CRITICAL_SECTION_START;
	// dump numbytes from the front of the buffer
	// are we dumping less than the entire buffer?
	if(numbytes < buffer->datalength)
	{
		// move index down by numbytes and decrement length by numbytes
		buffer->dataindex += numbytes;
		if(buffer->dataindex >= buffer->size)
		{
			buffer->dataindex -= buffer->size;
		}
		buffer->datalength -= numbytes;
	}
	else
	{
		// flush the whole buffer
		buffer->datalength = 0;
	}
	// end critical section
	CRITICAL_SECTION_END;
}

#endif

