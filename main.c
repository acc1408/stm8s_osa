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

//
/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
#include <string.h>


//#include <math.h>
/* Private defines -----------------------------------------------------------*/
#define enStep  GPIOD, GPIO_PIN_4
#define clkStep GPIOD, GPIO_PIN_2
#define dirStep GPIOD, GPIO_PIN_3

#define butLeft GPIOD, GPIO_PIN_5
#define butRight GPIOB, GPIO_PIN_5
#define potLeft GPIOB, GPIO_PIN_1
#define potRight GPIOB, GPIO_PIN_0
#define reset_cnt 10

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
//uint8_t a[]={0x01,0x04,0x02,0x03,0x02};

//char simvol;		
//---------------------
// ADC_Single
/*
void ADC1_Single_Init(F_div);
uint16_t ADC1_Single_StartSW_getValue(channel);



void ADC1_Scan_Single(F_div, channels)
void ADC1_Start(SourceStart);
void ADC1_waitEndConvertion();
uint16_t ADC1_Scan_getValue(channel);

void ADC1_Scan_Cont(F_div, channels)
void ADC1_Start(SourceStart);
void ADC1_waitEndConvertion();
uint16_t ADC1_Scan_getValue(channel);

*/
//---------------------
uint16_t adcV[10],cnt2,nm, Vmax,accel;
uint8_t oborot=0, btncode=0,cnt;
button_t btnLeft,btnRight;
buttoncode_t btnLeftCode,btnRightCode; 

uint16_t nmb=0,adc0;
uint32_t clks;
//-----


typedef struct
{
	uint16_t V_pv; // текущее значение
	uint16_t V_sp; // уставка
	uint16_t V_step; // шаг изменения
	//uint16_t V_min; // шаг изменения
	uint32_t F_cpu; // частота процессора в kHz 
	uint16_t n; // текущий предделитель
}
tuner_t;

tuner_t tuner;


//int16_t a,b;
#ifdef  __OSA__
void Task(void)
{
	uint8_t i,temp;
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(200);
	}
}
#endif


i2cStatus_t res;

FlagStatus adcStatus;


void clockStep(void)
{
	GPIO_WriteReverse(clkStep);
	//GPIO_WriteLow(clkStep);
	//GPIO_WriteHigh(clkStep);
	//TIM2_SetAutoreload(10+190*adc0/510);

	//TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	if(cnt)
	{	
		cnt--;
	}	
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
	
	//GPIO_WriteHigh(clkStep);
	/*
	for(nm=0;nm<100;nm++)
		nop();
	GPIO_WriteLow(clkStep);
	*/
}


void Tuner_Init(tuner_t *tuner,
								uint16_t V_pv,// текущее значение
								uint16_t V_sp, // уставка
								uint16_t V_step,
								uint16_t F_cpu,
								uint16_t n)
{
	tuner->V_pv=V_pv;
	tuner->V_sp=V_sp;
	tuner->V_step=V_step;
	tuner->F_cpu=F_cpu;
	tuner->n=n;
	
}
void Tuner_setStep(tuner_t *tuner,uint8_t V_step)
{
	tuner->V_step=V_step;
}
uint8_t g=0;
uint16_t Tuner_computer(tuner_t *tuner, uint16_t V_sp)
{
	uint32_t temp;
	tuner->V_sp=V_sp;
	if (tuner->V_pv == tuner->V_sp)
	{
		// если уставка равна задани
		return tuner->n;
	}
	//g++;
	if (tuner->V_pv < tuner->V_sp)
	{
		if (tuner->V_pv <1000)
		{
			tuner->V_pv +=tuner->V_step;
		}
		else
		if(tuner->V_pv <1500)
		{
			
			//if (g%2==0)
			{
				tuner->V_pv +=20;
			}
			g=0;
		}
		else
		if(tuner->V_pv <2000)
		{
			
			if (g>50)
			{
				tuner->V_pv +=10;
			}
			else
			{
				g++;
			}
		}
		else
		if(tuner->V_pv <3000)
		{
			//if (g%2==0)
				tuner->V_pv +=3;
		}
		if(tuner->V_pv <3500)
		{
			//if (g%2==0)
				tuner->V_pv +=1;
		}
		else
		if(tuner->V_pv <3700)
		{
			g++;
			if (g%3==0)
				tuner->V_pv +=1;
		}
		else
		{
			g++;
			if (g%5==0)
				tuner->V_pv +=1;
		}
		
		if (tuner->V_pv > tuner->V_sp)
		{
			tuner->V_pv = tuner->V_sp;
		}
	}
	else
	{
		// Уменьшение частоты вращения
		
		if (tuner->V_pv > tuner->V_step)
		{
			if (tuner->V_pv >3500)
			{
				tuner->V_pv -=1;
			}
			else
			if (tuner->V_pv >3000)
			{
				tuner->V_pv -=3;
			}
			if (tuner->V_pv >2500)
			{
				tuner->V_pv -=5;
			}
			else
			if (tuner->V_pv >2000)
			{
				tuner->V_pv -=10;
			}
			else
			{
				tuner->V_pv -=tuner->V_step;
			}	
			
			if (tuner->V_pv < tuner->V_sp)
			{
				tuner->V_pv=tuner->V_sp;
			}
			
			
		}
		else
		{
			tuner->V_pv=tuner->V_sp;
		}
	}
	
	if (tuner->V_pv==0)
	{
		tuner->n=0;
		return tuner->n;
	}
	else
	{
		tuner->n=  ((uint32_t)tuner->F_cpu*(uint32_t)60)/tuner->V_pv;
		return tuner->n;
	}
}

uint16_t Tuner_getN(void)
{
	
	return 0;
}
uint16_t n_timer=0;
uint8_t k=0,btr;
// 

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
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	// Отключили двигатель
	GPIO_Init(enStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(dirStep, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(clkStep, GPIO_MODE_OUT_PP_LOW_FAST);
	ButtonInit(&btnLeft,5000, butLeft);
	ButtonInit(&btnRight,5000, butRight);
	
	
	/*
	ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, 
               ADC1_CHANNEL_0,
               ADC1_PRESSEL_FCPU_D2, 
               ADC1_EXTTRIG_TIM, 
               DISABLE, // Trig
							 ADC1_ALIGN_RIGHT, 
               ADC1_SCHMITTTRIG_CHANNEL0, 
               DISABLE);
	*/
	//ADC1_StartConversion();
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 9600);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	//TIM2_Cmd(ENABLE);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_ADC, ENABLE);
	
	ADC1_SingleOne_Init(ADC1_PRESSEL_FCPU_D18, 
												ADC1_Handler_NoIT);
	
	Tuner_Init(&tuner,
								0,// текущее значение
								0, // уставка
								10, // шаг
								16000, // частота
								0); // предделитель таймера
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 9600);
	TIM2_Cmd(DISABLE);
	enableInterrupts()
	k=0;
	cnt=reset_cnt;
	
	for(nm=0;nm<10000;nm++)
		nop();
	GPIO_WriteLow(enStep);
	//GPIO_WriteHigh(enStep);
	//
	btncode=0;
	while (1)
  {
	ADC1_SingleAny_Start(ADC1_SOFT,	ADC1_CHANNEL_0);		
	Vmax=ADC1_SingleOne_GetConversion()*4+10;
	if (Vmax>4000)
	{
		Vmax=4000;
	}
	ADC1_SingleAny_Start(ADC1_SOFT,	ADC1_CHANNEL_1);		
	accel=1+ (ADC1_SingleOne_GetConversion()/8);

	Tuner_setStep(&tuner,accel);
	//for(nm=0;nm<1000;nm++)
	//	nop();
	btnLeftCode=ButtonRead(&btnLeft, butLeft);
	btnRightCode=ButtonRead(&btnRight, butRight);
	if (btnLeftCode==pressup)
	{
		// запуск вращеия по часовой стрелке
		nop();
		if (btncode==0||btncode==2)
		{
			GPIO_WriteLow(enStep);
			btncode=1;
			cnt=reset_cnt;
			GPIO_WriteLow(dirStep);
			n_timer=Tuner_computer(&tuner, Vmax);
			TIM2_SetAutoreload(n_timer);
			TIM2_Cmd(ENABLE);
		}
	}
	if (btnLeftCode==pressdown)
	{
		nop();
		if (btncode==1)
		{
			btncode=2;
			
		}
	}
	if (btnRightCode==pressup)
	{
		// запуск вращения против часовой стрелке
		nop();
		if (btncode==0||btncode==4)
		{
			GPIO_WriteLow(enStep);
			btncode=3;
			cnt=reset_cnt;
			GPIO_WriteHigh(dirStep);
			n_timer=Tuner_computer(&tuner, Vmax);
			TIM2_SetAutoreload(n_timer);
			TIM2_Cmd(ENABLE);
		}
		
	}
	if (btnRightCode==pressdown)
	{
		nop();
		if (btncode==3)
		{
			btncode=4;
		}
	}
	
	switch(btncode)
	{
		case 1:
			if (cnt==0)
			{
				n_timer=Tuner_computer(&tuner, Vmax);
				TIM2_SetAutoreload(n_timer);
				cnt=reset_cnt;
			}
			//TIM2_Cmd(ENABLE);
			break;
		case 2:
			if (cnt==0)
				{
					n_timer=Tuner_computer(&tuner, 0);
					if (n_timer==0)
					{
						TIM2_Cmd(DISABLE);
						GPIO_WriteLow(dirStep);
						btncode=0;
						GPIO_WriteHigh(enStep);
					}
					else
					{
						TIM2_SetAutoreload(n_timer);
						cnt=reset_cnt;
					}
				}
			break;
		case 3:
			if (cnt==0)
			{
				n_timer=Tuner_computer(&tuner, Vmax);
				TIM2_SetAutoreload(n_timer);
				cnt=reset_cnt;
			}
			break;
		case 4:
			if (cnt==0)
				{
					n_timer=Tuner_computer(&tuner, 0);
					if (n_timer==0)
					{
						TIM2_Cmd(DISABLE);
						GPIO_WriteLow(dirStep);
						btncode=0;
						GPIO_WriteHigh(enStep);
					}
					else
					{
						TIM2_SetAutoreload(n_timer);
						cnt=reset_cnt;
					}
				}
			break;
		default:
			TIM2_Cmd(DISABLE);
			nop();
	}
	
	/*
	if (==pressup)
	{
		if (oborot==0)
		{
			oborot=1;
			cnt2=999;
			TIM2_SetAutoreload(6400);
			GPIO_WriteLow(enStep);
			TIM2_Cmd(ENABLE);
		}
		
	}
		
	btr=GPIO_ReadInputPin(butRight);	
	if (GPIO_ReadInputPin(butRight))	
	{
		k=1;
		//GPIO_WriteLow(enStep);
		//TIM2_Cmd(ENABLE);
		GPIO_WriteHigh(enStep);
		//GPIO_WriteLow(enStep);
	}
	else
	{
		k=0;
		GPIO_WriteLow(enStep);
		//GPIO_WriteHigh(enStep);
		//TIM2_Cmd(DISABLE);
		//GPIO_WriteHigh(enStep);
	}
	*/
	/*
	if (k)
	{	
		GPIO_WriteLow(enStep);
		if (cnt==0)
		{
			n_timer=Tuner_computer(&tuner, 3750);
			TIM2_SetAutoreload(n_timer);
			cnt=reset_cnt;
		}
		if (tuner.V_pv==1000)
		{
			nop();
		}
		TIM2_Cmd(ENABLE);
	}
	else
	{	
		
		
		if (n_timer)
		{
			if (cnt==0)
			{
				n_timer=Tuner_computer(&tuner, 0);
				if (n_timer)
				{
					TIM2_SetAutoreload(n_timer);
				}
				cnt=reset_cnt;
			}
				
			
		}
		else
		{
			TIM2_Cmd(DISABLE);
			GPIO_WriteHigh(enStep);
			cnt=reset_cnt;
		}
		
	}
	*/
	nmb++;
	
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
