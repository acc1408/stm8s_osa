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
#include "main.h"
#include "stm8s.h"
#include "stm8s_it.c"
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// частота тактирования таймера

uint32_t Clk;

typedef enum 
{
	off=0,
	on=1
} MotorEn_t;

typedef enum 
{
	rot=0,
	lift=1
} MotorActiv_t;


struct 
{
	MotorEn_t MotorEnRot:1;		// 0 - dis, 1 - en
	MotorEn_t MotorEnLeft:1; // 0 - dis, 1 - en
	MotorActiv_t MotorChoice:1; // 0 -rot, 1 - left
} rtn; 
//-----------
button_t RotStartBtn, RotStopBtn, 
				 LiftStartBtn, LiftStopBtn,RotPauseBtn;
//------------------------
encoder_t RotEnc, LiftEnc;
//-------------------------
void Init(void)
{
	// Инициализация выводов 
	GPIO_Init(RotEnGpio, 			GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(RotDirGpio, 		GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LiftEnGpio, 		GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LiftDirGpio,		GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LiftPulseGpio,	GPIO_MODE_OUT_PP_HIGH_FAST);
	
	GPIO_Init(RotLedGpio,			GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(LiftLedGpio,		GPIO_MODE_OUT_PP_HIGH_FAST);
		// Инициализация светодиода на плате
		// Обозначаем работу платы
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
	// Инициализация кнопок
	ButtonInit(&RotStartBtn,	0,RotStartGpio);
	ButtonInit(&RotStopBtn,		0,RotStopGpio);
	ButtonInit(&LiftStartBtn,	0,LiftStartGpio);
	ButtonInit(&LiftStopBtn,	0,LiftStopGpio);
	ButtonInit(&RotPauseBtn,	0,RotPauseGpio);

	// Init struct
	rtn.MotorEnRot=off;
	rtn.MotorEnLeft=off;
	
	
	// частота тактирования таймера
	Clk=CLK_GetClockFreq(); 
	// Инициализация Таймера 2
	// Тактирование вращения
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 800);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_OC2Init(TIM2_OCMODE_TOGGLE, 
								TIM2_OUTPUTSTATE_ENABLE, 
								0, TIM2_OCPOLARITY_HIGH);
	// Инициализация Таймера 3
	// Тактирование подъема
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
	TIM3_TimeBaseInit(TIM3_PRESCALER_1, 1280);
	TIM3_ARRPreloadConfig(ENABLE);
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
	
}

void Button(void)
{
	
	
	while(1)
	{
		buttoncode_t ret;
		// Включение вращения
		ret=ButtonRead(&RotStartBtn,RotStartGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnRot=on;
			rtn.MotorChoice=rot;
			GPIO_WriteLow(RotEnGpio);
		}
		// Отключение вращения
		ret=ButtonRead(&RotStopBtn,RotStopGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnRot=rot;
			GPIO_WriteHigh(RotEnGpio);
			EncoderSetCount(&RotEnc,0);
			TIM2_Cmd(DISABLE);
		}
		// Выбор подъема
		ret=ButtonRead(&LiftStartBtn,	LiftStartGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnLeft=on;
			rtn.MotorChoice=lift;
			GPIO_WriteLow(LiftEnGpio);
			
		}
		// Отключение подъема
		ret=ButtonRead(&LiftStopBtn,LiftStopGpio);
		if (ret==pressdown) 
		{
			rtn.MotorEnLeft=off;
			GPIO_WriteHigh(LiftEnGpio);
			EncoderSetCount(&LiftEnc,0);
			TIM3_Cmd(DISABLE);
		}
		// Пауза вращения
		ret=ButtonRead(&RotPauseBtn,RotPauseGpio);
		if (ret==pressdown) 
		{
			TIM2_Cmd(DISABLE);
		}
		OS_Delay (20); // 10 ms
	}
}

//------------------------
void Led(void)
{
	//task_led=OS_Task_GetCur();
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5); // Blink work cpu
		if ((rtn.MotorEnRot==on)&&(rtn.MotorChoice==rot))
		{
			GPIO_WriteReverse(RotLedGpio);
		}
		else
		{
			if (rtn.MotorEnRot==1)
			{
				GPIO_WriteHigh(RotLedGpio);
			}
			else
			{
				GPIO_WriteLow(RotLedGpio);
			}
		}
		//---------------
		if ((rtn.MotorEnLeft==on)&&(rtn.MotorChoice==lift))
		{
			GPIO_WriteReverse(LiftLedGpio);
		}
		else
		{
			if (rtn.MotorEnLeft==1)
			{
				GPIO_WriteHigh(LiftLedGpio);
			}
			else
			{
				GPIO_WriteLow(LiftLedGpio);
			}
		}
		OS_Delay (1000);
	}
}


void init_encoder(void)
{
	EncoderInit(&RotEnc, EncoderGpio,
										0,12500,800);
	EncoderInit(&LiftEnc, EncoderGpio,
										0,5250,1000);
}

//-----------------------------------
int32_t RotCnt=0,RotCntLast=0,LeftCnt=0,LeftCntLast=0,temp;
//-----------------------------------------------
void rotation(void)
{
	// направление вращения двигателя
	int8_t dir; 
	uint16_t t2;
	// получить частоту тактирования таймера
	init_encoder();
	while(1)
	{
		if ((rtn.MotorEnRot==1)&&(rtn.MotorChoice==0))
		{
			RotCnt=EncoderRead(&RotEnc,EncoderGpio);
			if (RotCnt>100000) 
			{
				RotCnt=100000;
				EncoderSetCount(&RotEnc,RotCnt);
			}
			if (RotCnt<-100000) 
			{
				RotCnt=-100000;
				EncoderSetCount(&RotEnc,RotCnt);
			}
			
			if (RotCnt!=RotCntLast)
			{
				if ((RotCnt>-123) &&(RotCnt<123) )
				{
					TIM2_Cmd(DISABLE);
				}
				else
				{
					if (RotCnt>=123)
					{
						GPIO_WriteHigh(RotDirGpio);
						dir=1;
					}
					else
					{
						GPIO_WriteLow(RotDirGpio);
						dir=-1;
					}
					t2=Clk/(2*(dir*RotCnt));
					TIM2_SetAutoreload(t2);
					TIM2_Cmd(ENABLE);
				}
			}
			RotCntLast=RotCnt;
		}
		if ((rtn.MotorEnLeft==1)&&(rtn.MotorChoice==1))
		{
			//TIM3_ITConfig(TIM3_IT_UPDATE, DISABLE); 
			TIM3_Cmd(DISABLE);
			EncoderSetCount(&LiftEnc,LeftCnt);
			LeftCnt=EncoderRead(&LiftEnc,EncoderGpio);
			//OS_DI(); 
		
			//LeftCnt+=temp;
			//TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
			//OS_EI(); 
			if (LeftCnt>0)
			{
				GPIO_WriteHigh(LiftDirGpio);
				TIM3_Cmd(ENABLE);
			}
			if (LeftCnt<0)
			{
				GPIO_WriteLow(LiftDirGpio);
				TIM3_Cmd(ENABLE);
			}
			
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
	Init();
	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(500); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Button); // создаем задачу
	OS_Task_Create(7, Led); // создаем задачу
	OS_Task_Create(7, rotation); // создаем задачу
	
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
