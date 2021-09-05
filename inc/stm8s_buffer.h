/**
  ******************************************************************************
  * @file    stm8s_buffer.h
  * @author  Salnikov Aleksey
	* @mail		 acc1408@mail.ru
	* @github  acc1408
  * @version V1.0.0
  * @date    29-08-2021
  * @brief   This file contains all functions prototype and macros for the circular buffer.
   ******************************************************************************
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_BUFFER_H
#define __STM8S_BUFFER_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"


//! cBuffer structure
typedef struct struct_cBuffer
{
	uint8_t  *dataptr;		/// указатель на реальный буфер
	uint16_t size;				/// размер буфера обмена
	uint16_t datalength;	/// размер зан€того буфера обмена
	uint16_t dataindex;		/// индекс начала буфера
} cBuffer_t;

// инициализаци€ буфера 
void bufferInit(cBuffer_t* buffer, uint8_t *start, uint16_t size);

// получить данные из буфера
uint8_t  bufferGetFromFront(cBuffer_t* buffer);


// добавить данные в буфер 
// успех 0, провал -1
uint8_t bufferAddToEnd(cBuffer_t* buffer, uint8_t data);

// очистить данные в буфере
void bufferFlush(cBuffer_t* buffer);

// проверка сколько €чеек буфера свободно
uint16_t bufferIsNotFull(cBuffer_t* buffer);

// проверка сколько €чеек зан€то
uint16_t bufferIsEmpty(cBuffer_t* buffer);

// освободить последние (более старые данные) numbytes €чеек в буфере от данных
void bufferDumpFromFront(cBuffer_t* buffer, uint16_t numbytes);

#endif