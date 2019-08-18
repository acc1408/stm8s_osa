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
//uint32_t Clk; 
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

struct 
{
	uint8_t MotorEnRot:1;		// 0 - dis, 1 - en
	uint8_t MotorEnLeft:1; // 0 - dis, 1 - en
	uint8_t MotorChoice:1; // 0 -rot, 1 - left
	
} rtn;

OST_TASK_POINTER task_led=0;
#define ButWhiteGpio			GPIOB, GPIO_PIN_0
#define ButWhiteStopGpio 	GPIOB, GPIO_PIN_1
#define ButGreenStopGpio 	GPIOB, GPIO_PIN_2
#define ButGreenGpio 			GPIOB, GPIO_PIN_3
#define ButEncoderGpio 		GPIOF, GPIO_PIN_4

button_t BtnWhite, BtnWhiteStop,BtnGreen, BtnGreenStop,ButEncoder;

//uint8_t k=0;
void Button(void)
{
	ButtonInit(&BtnWhite,			0,	ButWhiteGpio);
	ButtonInit(&BtnWhiteStop,	0,	ButWhiteStopGpio);
	ButtonInit(&BtnGreenStop,	0,	ButGreenStopGpio);
	ButtonInit(&BtnGreen,			0,	ButGreenGpio);
	ButtonInit(&ButEncoder,		0,	ButEncoderGpio);
	// Init struct
	rtn.MotorEnRot=0;
	rtn.MotorEnLeft=0;
	
	while(1)
	{
		buttoncode_t ret;
		// Включение вращения
		ret=ButtonRead(&BtnWhite ,ButWhiteGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnRot=1;
			rtn.MotorChoice=0;
		}
		// Отключение вращения
		ret=ButtonRead(&BtnWhiteStop ,ButWhiteStopGpio);
		if (ret==pressdown) rtn.MotorEnRot=0;
		// Выбор подъема
		ret=ButtonRead(&BtnGreen ,ButGreenGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnLeft=1;
			rtn.MotorChoice=1;
		}
		// Отключение подъема
		ret=ButtonRead(&BtnGreenStop ,ButGreenStopGpio);
		if (ret==pressdown) rtn.MotorEnLeft=0;
		
		OS_Delay (20); // 10 ms
	}
}

#define LedGreen    GPIOB, GPIO_PIN_5
#define LedYellow   GPIOB, GPIO_PIN_4

void init_led(void)
{
	GPIO_Init(LedYellow, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LedGreen, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void Led(void)
{
	task_led=OS_Task_GetCur();
	init_led();
	
	while(1)
	{
		if ((rtn.MotorEnRot==1)&&(rtn.MotorChoice==0))
		{
			GPIO_WriteReverse(LedGreen);
		}
		else
		{
			if (rtn.MotorEnRot==1)
			{
				GPIO_WriteLow(LedGreen);
			}
			else
			{
				GPIO_WriteHigh(LedGreen);
			}
		}
		//---------------
		if ((rtn.MotorEnLeft==1)&&(rtn.MotorChoice==1))
		{
			GPIO_WriteReverse(LedYellow);
		}
		else
		{
			if (rtn.MotorEnLeft==1)
			{
				GPIO_WriteLow(LedYellow);
			}
			else
			{
				GPIO_WriteHigh(LedYellow);
			}
		}
		//GPIO_WriteReverse(LedGreen);
		//GPIO_WriteReverse(LedYellow);
		OS_Delay (1000);
	}
}
encoder_t ecd_rot, ecd_left;
#define EncoderGpio GPIOD,GPIO_PIN_6,GPIOD,GPIO_PIN_7
void init_encoder(void)
{
	//Clk=CLK_GetClockFreq(); 
	// инициал
	EncoderInit(&ecd_rot, EncoderGpio,
										0,12500,800);
	EncoderInit(&ecd_left, EncoderGpio,
										0,84000,1000);

}
int32_t c;
void rotation(void)
{
	// направление вращения двигателя
	int8_t dir; 
	// получить частоту тактирования таймера
	init_encoder();
	while(1)
	{
		if ((rtn.MotorEnRot==1)&&(rtn.MotorChoice==0))
		{
			c=EncoderRead(&ecd_rot,EncoderGpio);
		}
		if ((rtn.MotorEnLeft==1)&&(rtn.MotorChoice==1))
		{
			c=EncoderRead(&ecd_left,EncoderGpio);
		}
	
		OS_Delay (1);
	}
}


void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	//-------------------------
	init_timer();
	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(500); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Button); // создаем задачу
	OS_Task_Create(7, Led); // создаем задачу
	OS_Task_Create(7, rotation); // создаем задачу
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
