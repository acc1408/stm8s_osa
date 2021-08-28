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
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
//void cmdreset(void);
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
/*
SetLCD_t lcd1;

 char st[40]="Test";

bme280_t bm;
uint32_t pres;
uint32_t hum;
int32_t t;
uint8_t rez;
*/
/*
char st[50];
char st_tx[100]; //  буфер для отправки
static uint8_t num_st=0;
unsigned short kol_tx=0; // счетчик номера передачи
unsigned char tx_num=0; // номер байт для передачи
float tx_float=0; // пример передачи плавающего числа
bool tx_start=0; // в начальный момент передачи данных нет
char st_tx2[10];

void putchar_st(char c)
{
	st[num_st] = c;
	num_st = (num_st+1) % 50;
}
*/
// обработчик прерывания передачи данных
// данная функция вызывается в stm8s_it.c
/*
void handler_tx(void)
{
	UART2_SendData8(st_tx[tx_num]); // передаем данные в сдвиговый регистр
	tx_num++; // увеличиваем счетчик
	if (st_tx[tx_num]=='\0') { 
														UART2_ITConfig(UART2_IT_TXE, DISABLE); // отключаем  прерывание если последний байт конец строки
														tx_num=0; // обнуляем счетчик для отпрвленных данных
														tx_start=0; // сбрасываем флаг передачи,
														// больше данные в буфере не нужны
														}
}
*/


// структура функции обработчика
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// структура таблицы соответсия название - функции 
typedef struct
{
	char name[20];	// название функции
	func_t func;		// указатель на функции
} tablefunc_t;
// структура таблицы соответствия номер возврата функции- текст вывода
typedef struct
{
	int16_t retfunc;	// номер возврата 
	char name[20];		// текст для вывода  
} tableRet_t;

// размер входящего буфера
#define cmdrxnum_max 20
// максимальное кол-во параметров
#define cmdargc_max 3
typedef enum
{
	receive=0, 	// прием данных
	exec=0b01,			// принят стоп-байт
	error_ovf=0b10, // переполнение входящего буфера
	//error_param=0b100, //переполнение кол-ва параметров
	//exec_error_param=0b101, //принят стоп-бит и было переполнение параметров
	//exec_error_ovf_param=0x111, // принят стоп-бит, переполнение параметров и буфера
	exec_error_ovf=0b011 // приннят стоп-бит и было переполнение буфера
	
}cmdstate_t;

// тип структуры командной строки
typedef struct
{
	char rx[cmdrxnum_max]; // длина
	uint8_t rxnum; // номер текущего байта
	char stopbit; // первый стоп-бит обязательный
	char stopbit2; // второй стоп-бит дополнительный
	tablefunc_t *tabfunc; // указатель на массив функций
	uint8_t numfunc;			// кол-во функций
	cmdstate_t state; // состояние командной строки
	uint8_t argc; // кол-во аргументов
	char *argv[cmdargc_max]; // массив указателей на параметры 
	tableRet_t *TabRet; //указатель-таблица на массив возврата результат функции
	uint8_t NumTabRet; // размер массива возврат-текст 
}cmdline_t;

//====================кольцевой буфер
// безопасная секция 
#ifndef CRITICAL_SECTION_START
char cc_reg; // регистр для хранения регистра состояний
#define CRITICAL_SECTION_START _asm("push CC \n sim \n pop _cc_reg \n");
#define CRITICAL_SECTION_END	 _asm("push _cc_reg \n pop CC \n") //
#endif

//! cBuffer structure
typedef struct struct_cBuffer
{
	uint8_t  *dataptr;		/// указатель на реальный буфер
	uint16_t size;				/// размер буфера обмена
	uint16_t datalength;	/// размер занятого буфера обмена
	uint16_t dataindex;		/// индекс начала буфера
} cBuffer_t;

// инициализация буфера 
void bufferInit(cBuffer_t* buffer, uint8_t *start, uint16_t size)
{
	
	// begin critical section
	CRITICAL_SECTION_START;
	// set start pointer of the buffer
	buffer->dataptr = start; // присвоение указателя
	buffer->size = size;	// присвоение размера
	// initialize index and length
	buffer->dataindex = 0;
	buffer->datalength = 0;
	// end critical section
	CRITICAL_SECTION_END;
}

cBuffer_t buf;
uint8_t mas[30];

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

// проверка сколько ячеек буфера свободно
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

// проверка сколько ячеек занято
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


// освободить последние numbytes ячеек в буфере от данных
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

//==================командная строка===================
cmdline_t cmd; // структура командной строки
// инициализаци командной строки
void cmdinit(	char stopbit, // вводим символ стоп-бит
							char stopbit2, // вводим символ стоп-бит
							tablefunc_t tabfunc[], //указатель на массив таблицы
							uint8_t numfunc, // кол-во функций
							tableRet_t *tabret, //указатель-таблица на массив возврата результат функции
							uint8_t NumTabRet// размер массива возврат-текст
							) 
{
	//uint8_t i=0;
	cmd.stopbit=stopbit; //первый стоп-бит
	cmd.stopbit2=stopbit2; //второй  стоп-бит
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
	cmd.TabRet=tabret;
	cmd.NumTabRet=NumTabRet;
	//while(*st && i<2)
	//cmd.stpstr[i++]=*st++;
}

void cmdinputchar(char c)
{
	switch(cmd.state)
	{
		// прием данных
		case receive:
				
				// проверяем место в буфере
				if (cmd.rxnum<cmdrxnum_max) 
					cmd.rx[cmd.rxnum++]=c; // заполняем буфер 
				else 	
					{
					cmd.state=error_ovf; // фиксируем ошибку переполнения буфера
					}
				//break;
		// если произошло переполнение ожидаем стоп-бита
		case error_ovf:
				if (c==cmd.stopbit) // если получен стоп-бит, 
						cmd.state|=exec; // ставим стоп-бит на обработку
				break;
		default:
					nop();
				break;
	}
}
// разбор на слова, вместо пробелов вставляем конец строки
// в массив заносим указатели на начала слов
// Если в результате разбора функции больше параметров, чем 
// допустимо, то результат будет 0, если меньше, то результат 1
uint8_t cmdparcer(void)
{
	// заменяем все знаки пробелов на конец строки
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // если символ пробел
			cmd.rx[i]='\0'; // то заменяем его на конец строки
	cmd.rx[cmd.rxnum-1]='\0';
	//фильтруем последний стоп-бит если он станет первым
	if (cmd.rx[0]==cmd.stopbit2) 
		cmd.rx[0]='\0'; // заменяем второй стоп-бит на пробел
	// определяем начало строк
	// Для первого символа в строке определяем что это начало функции
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx; // заменяем все пробелы на знак конец строки
		}
	// Для следующих символов определяем начало строк.
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0'))
			if (cmd.argc<cmdargc_max)
				cmd.argv[cmd.argc++]=cmd.rx+i;
			else
				return -1; //ошибка кол-во параметров больше, чем допустимо
	
	return 0; // кол-во параметров допустимо
	
			
}

// сброс состояния автомата командной строки
void cmdreset(void)
{
	cmd.rxnum=0;
	cmd.state=receive;
	cmd.argc=0;
}

// поиск фукнции для выполнения
void cmdexec(void)
{
	uint8_t i;
	int16_t ret_func; // номер возврата функции
	char *funcfind; // указатель на строку
	func_t funcexec;// указатель на функцию
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // указатель на строку поиска
		funcexec=cmd.tabfunc[i].func; // указатель на соответствующую строку
		if (strcmp(cmd.argv[0],funcfind)==0) // если нашли соответствующую строчку
		{
			// выполняем функцию 
			ret_func=funcexec(cmd.argc,cmd.argv);
			// выполняем поиск соответвия возврат-текст
			for(i=0;i<cmd.NumTabRet;i++)
			{
				if (ret_func==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name);
					cmdreset(); // сброс автомата для сбора символов
					return;		// выход из функции
				}
			}
		}
	}	
		// если не нашли функцию, то производим поиск ошибки на неверную команду
		for(i=0;i<cmd.NumTabRet;i++)
			{
				// -1 - код функции не найдена команда
				if (-1==cmd.TabRet[i].retfunc) 
				{
					printf("%s",cmd.TabRet[i].name); // если найдено совпадение, то печатаем ошибку
					break; // выходим из цикла
				}
			}
		cmdreset(); // сброс автомата для сбора символов
		return;		// выход из функции
		
}



// главный цикл для обработки кондной строки
void cmdmainloop(void)
{
	uint8_t i;
	switch(cmd.state) // проверяем состояние строки
		{
			case exec: // 
					if (!cmdparcer()) 	
							cmdexec();
						else
						{
							for(i=0;i<cmd.NumTabRet;i++)
							{
								// -3 - слишком много параметров
								if (-3==cmd.TabRet[i].retfunc) 
								printf("%s",cmd.TabRet[i].name);
							}
							cmdreset();
						}
				break;
			case 	exec_error_ovf:
					for(i=0;i<cmd.NumTabRet;i++)
					{
						// -2 - слишком длинная строка
						if (-2==cmd.TabRet[i].retfunc) 
							printf("%s",cmd.TabRet[i].name);
					}
					cmdreset();
				break;
		}
}

char st2[40]="test string"; // строка для передачи данных

// отключения и включение светодиода
int16_t led(uint8_t argc,char *argv[])
{
	if (argc=2)  
	{
		if (strcmp(argv[1],"on")==0) // включаем светодиод
			GPIO_WriteLow(GPIOE, GPIO_PIN_5);
		else 
			if (strcmp(argv[1],"off")==0)  // отключаем светодиод
				GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
		return 0;
	}
	else
		return 1;
}


uint8_t bl=1; // переменная для включения и отключения мигания
// функция для включения и отключения мигания
int16_t blink(uint8_t argc,char *argv[])
{
	if (argc=2) 
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
const tableRet_t RetText[4]={ 
															-3, "too many parametr\r\n",
															-2, "too long string\r\n",
															-1, "command not found\r\n",
															 0, "ok\r\n"
														};

//char simvol;		
//============================ вывод данных из UART

void uart2_sendtobuffer(uint8_t chr)
{
	while(bufferAddToEnd(&buf,  chr));
	UART2_ITConfig(UART2_IT_TXE, ENABLE);
}

void uart2_GetFromBuffer(void)
{
	
	if (bufferIsEmpty(&buf)>1) 
			UART2->DR=bufferGetFromFront(&buf);
		else
			{
			UART2->DR=bufferGetFromFront(&buf);
			UART2_ITConfig(UART2_IT_TXE, DISABLE);
		}
}

char putchar(char c)
{
	//nop();
	//simvol=c;
	uart2_sendtobuffer(c);
	//nop();
	return 0;
}


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

cBuffer_t buf;
uint8_t mas[30];
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
  //testf={.name="nvvjhggvn",.func=yui};
	cmdinit('\r', // стоп-символ
					'\n',
					testf, // указатель на таблицу название-функция
					2,  	// кол-во функций в таблице
					RetText,
					4);
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
	
	enableInterrupts();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	bufferInit(&buf, mas, 30);
	
	while (1)
  {
		cmdmainloop(); // обработка командной строки
		//printf("qwetr %d\r\n",nmb);
		nmb++;
	//if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
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
