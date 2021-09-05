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
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************


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
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	printf("Check cmdline\r\n");
	while (1)
  {
		//cmdmainloop(); // обработка командной строки
	//	printf("qwe5675089tr %d\r\n",nmb);
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
