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
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// частота тактирования таймера
uint32_t* Clk; 
/*
encoder_t ecd;
void init_encoder(void)
{
	Clk=CLK_GetClockFreq(); 
	// инициал
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);							
	EncoderInit(&ecd, GPIOD,GPIO_PIN_6,
										GPIOD,GPIO_PIN_7,
										0,12500,800);
	
}
*/
/*
void init_timer(void)
{
	// Инициализация Таймера 2
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 800);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_OC2Init(TIM2_OCMODE_TOGGLE, 
								TIM2_OUTPUTSTATE_ENABLE, 
								0, TIM2_OCPOLARITY_HIGH);
	// Инициализация Таймера 3
	//CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
	
}
*/


#define ButWhite GPIOB, GPIO_PIN_0
#define ButGreen GPIOB, GPIO_PIN_3

typedef enum
{
	idle=0, // ожидаение нажатия
	pressdown=1, // кнопка нажата
	presslong=2, // кнопка долго нажата
	pressup=3, // кнопка отпущена
	pressdouble=4 // кнопка нажата 2 раза
} buttoncode_t;

typedef struct
{
	uint8_t Level:1;				// состояние линии
	uint8_t LevelLast:1;  // прошлое состояние линии
	//buttoncode_t CodeLast:3;   // прошлое состояние линии
	uint16_t PressLongCycleMin; // минимальная задержка для кода удеражания клавиши
	uint16_t PressDoubleCycleMax; // Максимальная задержка между двумя нажатиями
	uint16_t Cycle; // кол-во циклов с последнего события
} button_t;

OST_TASK_POINTER task_led=0;
//OST_TCB task_led;

void ButtonInit(button_t* button,uint16_t PressLongCycleMin, 
								uint16_t PressDoubleCycleMax,
								GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN)
{
	GPIO_Init(GPIOx, GPIO_PIN, GPIO_MODE_IN_FL_NO_IT);
	button->PressLongCycleMin=PressLongCycleMin;
	button->PressDoubleCycleMax=PressDoubleCycleMax;
	button->LevelLast=1;
	button->Cycle=0xFFFF;
}

buttoncode_t ButtonRead(button_t* button, GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_PIN)
{
	button-> Level=GPIO_ReadInputPin(GPIOx,GPIO_PIN);
	// Если линия находится в покое, то код ожидания 
	// и увеличиваем таймер последних событий
	if ((button->Level==1) &&(button->LevelLast==1))
		{
			if (button->Cycle<0xFFFF) button->Cycle++;
			return idle;
		}
	// Если кнопка нажата начинаем отсчет 
  // Если задержка максимальная, то двойное нажатие отключено	
	if ((button->Level==0) &&(button->LevelLast==0))
		{
			if (button->Cycle<0xFFFF) button->Cycle++;
			if (button->Cycle>button->PressDoubleCycleMax)
			{
				button->Cycle--;
				return presslong;
			}
		  else 
			{ 
			return idle; 
			}
		
		}
	// кнопка нажата первый раз
	if ((button->Level==0)&&(button->LevelLast==1))
		{
			if (button->Cycle>button->PressDoubleCycleMax)
			{
				button->LevelLast=button->Level;
				button->Cycle=0;
				return pressdown;
			}
			else
			{
				button->LevelLast=button->Level;
				button->Cycle=0;
				return pressdouble;
			}
			
		}
	// кнопка отжата
	if ((button->Level==1)&&(button->LevelLast==0))
	{
		button->Cycle=1;
		button->LevelLast=button->Level;
		return pressup;
	}
}

button_t btn;
buttoncode_t ret;
static uint8_t k=0;
void Button(void)
{
	ButtonInit(&btn,1000, 
								0,
								GPIOB, GPIO_PIN_0);
	k=124;
	while(1)
	{
		ret=ButtonRead(&btn,GPIOB, GPIO_PIN_0);
		/*
		switch (ret)
		{
		//case presslong: OS_Task_Continue(task_led);
		//								break;
		//case idle:			OS_Task_Pause(task_led);
		//								break;
		case idle:
		}
		*/
		//if (ret==idle) k=0; 
		if (ret==pressdown) 
		{
			k=~k;
		}
		if (k==124)    
		//OS_Task_Pause(task_led);
		task_led->bStateByte |=0x80 ;
		if (k==0xFF) 
		OS_Task_Continue(task_led);
						
		OS_Delay (20); // 10 ms
	}
}

#define LedGreen    GPIOB, GPIO_PIN_4
#define LedYellow   GPIOB, GPIO_PIN_5

void init_led(void)
{
	GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void Led(void)
{
	task_led=OS_Task_GetCur();
	init_led();
	GPIO_WriteReverse(LedGreen);
	while(1)
	{
		GPIO_WriteReverse(LedGreen);
		GPIO_WriteReverse(LedYellow);
		OS_Delay (1000);
	}
}


/*
void lifting(void)
{
	while(1)
	{
		
	}
}

void rotation(void)
{
	// направление вращения двигателя
	int8_t dir; 
	// получить частоту тактирования таймера
	
	while(1)
	{
		c=EncoderRead(&ecd);
		
	
	
		OS_Delay (1);
	}
}
*/

void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	//-------------------------
	k=0;
	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(500); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Button); // создаем задачу
	OS_Task_Create(7, Led); // создаем задачу
	//task_led=OS_Task_GetCreated();
	//OS_Task_Create(7, encod_1); // создаем задачу
	//OS_Task_Create(7, encode2); // создаем задачу
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
