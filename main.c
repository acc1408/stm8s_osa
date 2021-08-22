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

//uint8_t mass[400];
// структура функции обработчика
typedef int16_t (*func_t)(uint8_t argc,char *argv[]);

// структура таблицы соответсия название - функции 
typedef struct
{
	char name[20];	// название функции
	func_t func;		// указатель на функции
} tablefunc_t;

// размер входящего буфера
#define cmdrxnum_max 40
// максимальное кол-во параметров
#define cmdargc_max 3
typedef enum
{
	receive=0, 	// прием данных
	exec=0b01,			// принят стоп-байт
	error_ovf=0b10, // переполнение входящего буфера
	exec_error_ovf=0b11 // приннят стоп-бит и было переполнение буфера
}cmdstate_t;

// тип структуры командной строки
typedef struct
{
	char rx[cmdrxnum_max]; // длина
	uint8_t rxnum; // номер текущего байта
	char stopbit; // стоп-бит
	tablefunc_t *tabfunc; // указатель на массив функций
	uint8_t numfunc;			// кол-во функций
	cmdstate_t state; // состояние командной строки
	uint8_t argc; // кол-во аргументов
	char *argv[cmdargc_max]; // массив указателей на параметры 
	//char stpstr[2];
}cmdline_t;


cmdline_t cmd; // структура командной строки
// инициализаци командной строки
void cmdinit(	char stopbit, // вводим символ стоп-бит
							tablefunc_t tabfunc[], //указатель на массив таблицы
							uint8_t numfunc) // кол-во функций
{
	//uint8_t i=0;
	cmd.stopbit=stopbit; // стоп-бит
	cmd.rxnum=0;
	cmd.tabfunc=tabfunc;
	cmd.numfunc=numfunc;
	cmd.argc=0;
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
						cmd.state|=exec; // ставим бит на обработку
				break;
		default:
					nop();
				break;
	}
}
// разбор на слова, вместо пробелов вставляем конец строки
// в массив заносим указатели на начала слов
void cmdparcer(void)
{
	// заменяем все знаки пробелов на конец строки
	uint8_t i;
	for( i=0;i<cmd.rxnum;i++)
	if (cmd.rx[i]==' ') // если символ пробел
			cmd.rx[i]='\0'; // то заменяем его на конец строки
	cmd.rx[cmd.rxnum-1]='\0';
	// определяем начало строк
	if (cmd.rx[0]) 
		{
			cmd.argv[cmd.argc++]=cmd.rx;
		}
	for( i=1;i<cmd.rxnum-1;i++)
		if ((cmd.rx[i-1]=='\0')&&(cmd.rx[i]!='\0')&&(cmd.argc<cmdargc_max))
			cmd.argv[cmd.argc++]=cmd.rx+i;
	nop();
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
	char *funcfind; // указатель на строку
	func_t funcexec;// указатель на функцию
	for(i=0;i<cmd.numfunc;i++)
	{
		funcfind=cmd.tabfunc[i].name; // указатель на строку поиска
		funcexec=cmd.tabfunc[i].func; // указатель на соответствующую строку
		if (strcmp(cmd.argv[0],funcfind)==0) 
		{
			nop();
			funcexec(cmd.argc,cmd.argv);
			break; // если функция найдена, то выходим из цикла поиска 
		}
	}	
		cmdreset(); // сброс автомата для сбора символов
}



// главный цикл для обработки кондной строки
void cmdmainloop(void)
{
	switch(cmd.state) // проверяем состояние строки
		{
			case exec: // 
					cmdparcer(); 
					cmdexec();
				break;
			case 	exec_error_ovf:
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
const tablefunc_t testf[2]={"led", led,
														"blink", blink	
														};
char simvol;														
char putchar(char c)
{
	nop();
	simvol=c;
	nop();
	return 0;
}

uint8_t CondFlag; // Переменная для хранения регистра состояний
void asm_insert(void)
{
	enableInterrupts(); // разрешаем глобальное прерывание
	// начало критической секции
	#pragma asm			// начало ассемблерной вставки
		push CC 			// помещаем регистр состояний в стек
		pop _CondFlag // извлекаем регистр состояний в созданную переменную
	#pragma endasm	// конец ассембленой вставки
	disableInterrupts(); // грантированно запрещаем глобальное прерывание
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST); // тестовый код
	// еще какой-то который должен гарантированно выполняться без прерываний
	#pragma asm			// начало ассемблерной вставки
		push _CondFlag	// помещаем регистра состояния в стек
		pop CC				// загружаем исходное состояние из стека в регистр состояний
	#pragma endasm	// конец ассемблерной вставки
	// конец критической секции
}

//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	I2C_Init_7bit(100000);
	//Lcdi2cInit(&lcd1, 0b0111111);
	Lcdi2cInit(&lcd1, 0b0111111, 
								ENABLE,
								DISABLE,
								ENABLE);
	LcdCursorSet(&lcd1, 0);
	Lcdi2cPrint(&lcd1, st);
	//LcdCursorRight(&lcd1);
	//LcdDisplayLeft(&lcd1);
	//I2C_MasterSendSend(0b0111111, a, 1, a+4, 3);
	//I2C_MasterSendReceive(0b0111111, a, 0, a+1, 4);
	
rez=BME280_Init(&bm, 0b1110110,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_OVERSAMPLING_16X,
						BME280_FILTER_COEFF_8,
						BME280_STANDBY_TIME_250_MS,
						BME280_NORMAL_MODE
						);
	//BME280_Reset(&bm);
	//BME280_StartStop(&bm, BME280_NORMAL_MODE);
	while(1)
	{
		//BME280_StartStop(&bm, BME280_FORCED_MODE);
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
		rez=BME280_GetMeasurement(&bm);
		pres=BME280_compensate_P_int32(&bm);
		t=BME280_compensate_T_int32(&bm);
		hum=BME280_compensate_H_int32(&bm);
		a=t/100;
		b=t%100;
		CursorGoTo(&lcd1, 1, 0);
		sprintf(st,"Temp=%d.%02d C   ", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=pres/1000;
		b=pres%1000;
		LcdCursorSet(&lcd1, 20);
		sprintf(st,"Press=%d%03d Pa", a,b);
		Lcdi2cPrint(&lcd1, st);
		a=hum/1024;
		b=hum%1024;
		if (b>999) b=999;
		LcdCursorSet(&lcd1, 60);
		sprintf(st,"Hum=%d.%03d%%   ", a,b);
		Lcdi2cPrint(&lcd1, st);
	}
}
#endif

#define cond_flag (((uint8_t *) 0x7f0a))


uint8_t *cf_u;
int16_t sdvig=0xa000;
uint16_t sdv2=0xF000;

void rotate_left (void)
{
uint8_t i;
for(i=0;i<4;i++) // Повторяем круговой сдвиг влево 4 раза
		{
			#pragma asm			// начало ассемблерной вставки
				LDW X,_sdvig 	// загружаем в регистр Х значение 16-битной переменной по адресу sdvig
				LDW Y,#$7FFF 	// загружаем число для проверки первого бита на 1 или 0
				CPW Y,_sdvig 	// если число больше, чем $7FFF, то бит переноса загружается в бит С регистра СС 
				RLCW X				// производим круговой сдвиг с учем бита С.
				LDW _sdvig,X	// Загружаем по адресу _sdvig значение из регистра Х
			#pragma endasm	// конец ассемблерной вставки
		}
}

uint8_t ua=0xCD; // беззнаковая переменная 	исходное			205 или 0b11001101
int8_t  sa=0xCD; // знаковая переменная		 	исходное			-51 или 0b11001101
uint8_t ua_right; // беззнаковая переменная сдвиг вправо	 51 или 0b00110011
int8_t  sa_right; // знаковая переменная		сдвиг вправо	-13 или 0b11110011
uint16_t ua_left; // беззнаковая переменная  сдвиг влево		 52 или 0b00110100
int16_t  sa_left; // знаковая переменная		  сдвиг влево		 52 или 0b00110100
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
					testf, // указатель на таблицу название-функция
					2 ); 	// кол-во функций в таблице
	UART2_Init(9600, UART2_WORDLENGTH_8D, 
                UART2_STOPBITS_1, UART2_PARITY_NO, 
                UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_SetRxHandler(cmdinputchar);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
	
	
	ua_right=ua>>2; // результат сдвиг вправо  51 или 0b00110011
	sa_right=sa>>2; // результат сдвиг вправо -13 или 0b11110011
	ua_left =ua<<2; // результат сдвиг влево   52 или 0b00110100
	sa_left =sa<<2; // результат сдвиг влево   52 или 0b00110100

	//cc=	cond_flag;
	{
		//_asm(" \n \n");
		//__cf=_asm("");
		/*
		#pragma asm
			push CC
			pop _cf
		#pragma endasm
		*/
		for(i=0;i<4;i++) // Повторяем круговой сдвиг влево 4 раза
		{
			#pragma asm			// начало ассемблерной вставки
				LDW X,_sdvig 	// загружаем в регистр Х значение 16-битной переменной по адресу sdvig
				LDW Y,#$7FFF 	// загружаем число для проверки первого бита на 1 или 0
				CPW Y,_sdvig 	// если число больше, чем $7FFF, то бит переноса загружается в бит С регистра СС 
				RLCW X				// производим круговой сдвиг с учем бита С.
				LDW _sdvig,X	// Загружаем по адресу _sdvig значение из регистра Х
			#pragma endasm	// конец ассемблерной вставки
		}
		
		sdvig=sdvig>>4;
		sdv2=sdv2>>4;
		//cf=_asm("ld cf,a ");
		//_asm("ld a ");
	
	}
	enableInterrupts();
	//asm_insert();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	/*
	#pragma asm
		push _cf
		pop CC
	#pragma endasm
	//cc=	cond_flag;
		*/
	while (1)
  {
		cmdmainloop(); // обработка командной строки
		printf("qwetr %d",0x54);
	if (bl) GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
	delay_ms(1000);
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
