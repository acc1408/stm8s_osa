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

//FunctionalState t2_pd2=DISABLE;
uint8_t but,but_last;
void Button(void)
{
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(GPIOD, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);
	GPIO_ExternalPullUpConfig(GPIOD,  GPIO_PIN_5, ENABLE);
	but_last=GPIO_ReadInputPin(GPIOD, GPIO_PIN_5);
	while(1)
	{
		but=GPIO_ReadInputPin(GPIOD, GPIO_PIN_5);
		if ((but==0) && (but_last==1))
		{
		GPIO_WriteHigh(GPIOE,  GPIO_PIN_5); // светодиод потух
		GPIO_WriteHigh(GPIOD,  GPIO_PIN_2); // выключил двигатель
		TIM2_Cmd(DISABLE);
		}
		but_last=but;
		OS_Delay (50);
	}
}


int32_t c=0,c_last=0;
encoder_t ecd;
uint32_t Clk;
uint16_t t2;

void encod_1(void)
{
	int8_t dir;
	Clk=CLK_GetClockFreq();
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 800);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_OC1Init(TIM2_OCMODE_TOGGLE, 
								TIM2_OUTPUTSTATE_ENABLE, 
								0, TIM2_OCPOLARITY_HIGH);
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);							
	EncoderInit(&ecd, GPIOD,GPIO_PIN_6,
										GPIOD,GPIO_PIN_7,
										0,12500,800 );
	while(1)
	{
		c=EncoderRead(&ecd);
		if (c!=c_last)
	{
		if ((c>-123) &&(c<123) )
		{
			TIM2_Cmd(DISABLE);
			GPIO_WriteHigh(GPIOE,  GPIO_PIN_5); // потушил светодиод
			//GPIO_WriteHigh(GPIOD,  GPIO_PIN_3); 
		}
		else
		{
			GPIO_WriteLow(GPIOE,  GPIO_PIN_5); // зажгли светодиод
			GPIO_WriteLow(GPIOD,  GPIO_PIN_2); // Включили двигатель
			if (c>=123)
			{
				GPIO_WriteHigh(GPIOD, GPIO_PIN_3);
				//GPIO_WriteHigh(GPIOE, GPIO_PIN_5);
				dir=1;
			}
			else
			{
				GPIO_WriteLow(GPIOD, GPIO_PIN_3);
				dir=-1;
			}
			t2=Clk/(2*(dir*c));
			TIM2_SetAutoreload(t2);
			TIM2_Cmd(ENABLE);
		}
	}
	c_last=c;
		OS_Delay (1);
	}
}


void main(void)
{
 #ifdef  __OSA__
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	//-------------------------
	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(500); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Button); // создаем задачу
	OS_Task_Create(7, encod_1); // создаем задачу
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
