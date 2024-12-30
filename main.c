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
// Height
// Подъемник
#define enStepH  	GPIOC, GPIO_PIN_2
#define clkStepH 	GPIOB, GPIO_PIN_2
#define dirStepH 	GPIOC, GPIO_PIN_1
#define endMinH  	GPIOB, GPIO_PIN_0
#define endMaxH 	GPIOB, GPIO_PIN_1
#define ledMinH  	GPIOD, GPIO_PIN_6
#define ledMaxH  	GPIOD, GPIO_PIN_7
#define butMinH 	GPIOC, GPIO_PIN_7
#define butMaxH 	GPIOD, GPIO_PIN_0


// Каратка 
//Caretka
#define enStepK  	GPIOF, GPIO_PIN_4
#define clkStepK 	GPIOB, GPIO_PIN_4
#define dirStepK 	GPIOB, GPIO_PIN_5
#define endMinK  	GPIOB, GPIO_PIN_3
#define endMaxK  	GPIOA, GPIO_PIN_2
#define ledMinK  	GPIOD, GPIO_PIN_4
#define ledMaxK  	GPIOD, GPIO_PIN_5
#define butMinK 	GPIOD, GPIO_PIN_3
#define butMaxK 	GPIOD, GPIO_PIN_2

#define max_V 1000 // максимальная скорость работы шагового двигателя
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************

//---------------------
// кнопки 
button_t btnLeft,btnRight,btnUp,btnDown;
buttoncode_t btnLeftCode,btnRightCode,btnUpCode,btnDownCode; 

uint16_t nmb=0,adc0,nm,n_timer;
uint32_t clks,cnt;
//-----


typedef struct
{
	int16_t V_pv; // текущее значение скорости в об/мин
	int16_t V_sp; // уставка значение скорости в об/мин
	// настройка ускорения драйвера
	uint8_t dt_step; // Частота расчета
	uint8_t cnt_step; // счетчик расчета
	//uint16_t V_step; // шаг изменения
	// настройка форумулы расчета драйвера
	uint32_t F_cpu; // частота процессора в kHz 
	uint16_t microstep; // кол-во шагов на оборот
	// результат измерения
	uint16_t n; // текущее значение для перезагрузки таймера cnt
}
tuner_t;

tuner_t tuner, tunerK;


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

void Tuner_decCntStep(tuner_t *tuner);

void clockStep(void)
{
	GPIO_WriteReverse(clkStepH);
	//GPIO_WriteLow(clkStep);
	//GPIO_WriteHigh(clkStep);
	//TIM2_SetAutoreload(10+190*adc0/510);

	//TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	Tuner_decCntStep(&tuner);
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
	
	//GPIO_WriteHigh(clkStep);
	/*
	for(nm=0;nm<100;nm++)
		nop();
	GPIO_WriteLow(clkStep);
	*/
}

void clockStepK(void)
{
	GPIO_WriteReverse(clkStepK);
	//GPIO_WriteLow(clkStep);
	//GPIO_WriteHigh(clkStep);
	//TIM2_SetAutoreload(10+190*adc0/510);

	//TIM2_ClearFlag(TIM2_FLAG_UPDATE);
	Tuner_decCntStep(&tunerK);
	TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
	
	//GPIO_WriteHigh(clkStep);
	/*
	for(nm=0;nm<100;nm++)
		nop();
	GPIO_WriteLow(clkStep);
	*/
}

// настройка ПИД регулятора
void Tuner_Init(tuner_t *tuner,
								uint8_t dt_step, // частота расчета
								uint32_t F_cpu,
								uint16_t microstep)
{
	tuner->V_pv=0;
	tuner->V_sp=0;
	tuner->n=0;
	tuner->dt_step=dt_step;
	tuner->F_cpu=F_cpu;
	tuner->microstep=microstep;
	tuner->cnt_step=0;
	
}
// настройка периода расчета нового значения
void Tuner_setStep(tuner_t *tuner,uint8_t dt_step)
{
	tuner->dt_step=dt_step;
}

void Tuner_setSP(tuner_t *tuner,int16_t V_sp)
{
	tuner->V_sp=V_sp;
}

void Tuner_decCntStep(tuner_t *tuner)
{
	if (tuner->cnt_step)
	{
		tuner->cnt_step--;
	}
}

void Tuner_resetCntStep(tuner_t *tuner)
{
	tuner->cnt_step=0;
}


//uint8_t g=0;
uint16_t Tuner_computer(tuner_t *tuner)
{
	//uint32_t temp;
	//tuner->V_sp=V_sp;
	if (tuner->cnt_step)
	{
		return tuner->n;
	}
	else
	{
		tuner->cnt_step=tuner->dt_step;
	}
	
	if (tuner->V_pv == tuner->V_sp)
	{
		// если уставка равна задани
		return tuner->n;
	}
	else
	if (tuner->V_pv < tuner->V_sp)
	{
		// ускорение движения
		if (tuner->V_pv <1000)
		{
			tuner->V_pv +=5;
		}
		else
		if(tuner->V_pv <1500)
		{
				tuner->V_pv +=2;
		}
		else
		if(tuner->V_pv <2000)
		{
			tuner->V_pv +=1;
		}
		else
		if(tuner->V_pv <3000)
		{
			tuner->V_pv +=1;
		}
		else
		{
			tuner->V_pv +=1;
		}
		// Проверка на переполнение задания
		if (tuner->V_pv > tuner->V_sp)
		{
			tuner->V_pv = tuner->V_sp;
		}
	}
	else
	{
		// торможение
		if (tuner->V_pv <1000)
		{
			tuner->V_pv -=35;
		}
		else
		if(tuner->V_pv <1500)
		{
				tuner->V_pv -=25;
		}
		else
		if(tuner->V_pv <2000)
		{
			tuner->V_pv -=20;
		}
		else
		if(tuner->V_pv <3000)
		{
			tuner->V_pv -=3;
		}
		else
		{
			tuner->V_pv -=1;
		}
		if (tuner->V_pv<0)
		{
			nop();
		}
		// Проверка на переполнение задания
		if (tuner->V_pv < tuner->V_sp)
		{
			tuner->V_pv = tuner->V_sp;
		}
		
	}
	// Расчет счетчика таймера
	if (tuner->V_pv==0)
	{
		tuner->n=0;
		tuner->cnt_step=0;
		return tuner->n;
	}
	else
	{
		tuner->n=  ((uint32_t)tuner->F_cpu*(uint32_t)60)/((uint32_t)tuner->microstep*tuner->V_pv);
		return tuner->n;
	}
}
/*
uint16_t Tuner_getN(void)
{
	
	return 0;
}
uint16_t n_timer=0;
uint8_t k=0,btr;
// 
*/
enum 
{
	stopH,
	stopMinH,
	stopMaxH,
	moveUpAccelH,
	moveUpBrakeH,
	moveDownAccelH,
	moveDownBrakeH,
	errorH
} heightMot;

enum 
{
	stopK,
	stopMinK,
	stopMaxK,
	moveLeftAccelK,
	moveLeftBrakeK,
	moveRightAccelK,
	moveRightBrakeK,
	errorK
} karetMot;


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
	// увеличиваем частоту ЦПУ
	CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
	// Настройка Таймера 2
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 9600);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
	
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
	TIM3_TimeBaseInit(TIM3_PRESCALER_1, 9600);
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
	TIM3_ARRPreloadConfig(ENABLE);
	
	
	//TIM2_Cmd(ENABLE);
	//-----------------------
	ButtonInit(&btnLeft,5000, butMinK);
	ButtonInit(&btnRight,5000, butMaxK);
	ButtonInit(&btnUp,5000, butMaxH);
	ButtonInit(&btnDown,5000, butMinH);
	// Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);

	//настройка светодиода на плате
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
	// настройка светодиодов
	GPIO_Init(ledMinH, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(ledMaxH, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(ledMinK, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(ledMaxK, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_WriteLow(ledMinK);
	GPIO_WriteLow(ledMaxK);
	GPIO_WriteLow(ledMinH);
	GPIO_WriteLow(ledMaxH);
	// настройка ШД каратки
	GPIO_Init(enStepK, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkStepK, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(dirStepK, GPIO_MODE_OUT_PP_HIGH_FAST);
	// настройка ШД подъемника
	GPIO_Init(enStepH, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(clkStepH, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_Init(dirStepH, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	// кнопки
	//GPIO_Init(butLowH, GPIO_MODE_IN_PU_NO_IT);
	//GPIO_Init(butHighH, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(butMinK, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(butMaxK, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(butMinH, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(butMaxH, GPIO_MODE_IN_PU_NO_IT);
	// концевики
	GPIO_Init(endMinH, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(endMaxH, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(endMinK, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(endMaxK, GPIO_MODE_IN_PU_NO_IT);
	
	Tuner_Init(&tuner,
								20, // частота расчета
								16000000, // частота ЦПУ
								800 // кол-во микрошагов на оборот
								); 
	Tuner_Init(&tunerK,
								20, // частота расчета
								16000000, // частота ЦПУ
								800 // кол-во микрошагов на оборот
								); 
	
	
	
	for(nm=0;nm<10000;nm++)
	{
		nop();
	}
	// настройка начального состояния подъемника
	if (GPIO_ReadInputPin(endMinH)==0)
	{
		heightMot=stopMinH;
	}
	else
	if (	GPIO_ReadInputPin(endMaxH)==0)
	{
		heightMot=stopMaxH;
	}
	else
	{
		heightMot=stopH;
	}
	
	// настройка начального состояния подъемника
	if (GPIO_ReadInputPin(endMinK)==0)
	{
		karetMot=stopMinK;
	}
	else
	if (	GPIO_ReadInputPin(endMaxK)==0)
	{
		karetMot=stopMaxK;
	}
	else
	{
		karetMot=stopK;
	}
	
	
	//btncode=0;	
	enableInterrupts();
	while (1)
  {
		btnLeftCode=ButtonRead(&btnLeft, butMinK);
		btnRightCode=ButtonRead(&btnRight, butMaxK);
		btnUpCode=ButtonRead(&btnUp, butMaxH);
		btnDownCode=ButtonRead(&btnDown, butMinH);
		nop();
		
		switch(heightMot)
		{
			case stopH:
				if (btnUpCode==pressdown)
				{
					GPIO_WriteLow(enStepH);
					GPIO_WriteLow(dirStepH);
					
					Tuner_resetCntStep(&tuner);
					Tuner_setSP(&tuner,max_V);
					n_timer=Tuner_computer(&tuner);
					TIM2_SetAutoreload(n_timer);
					TIM2_Cmd(ENABLE);
					
					heightMot=moveUpAccelH;
				}
				else
				if (btnDownCode==pressdown)
				{
					GPIO_WriteLow(enStepH);
					GPIO_WriteHigh(dirStepH);
					
					Tuner_resetCntStep(&tuner);
					Tuner_setSP(&tuner,max_V);
					n_timer=Tuner_computer(&tuner);
					TIM2_SetAutoreload(n_timer);
					TIM2_Cmd(ENABLE);
					
					heightMot=moveDownAccelH;
				}
				break;
			case	stopMinH:
				if (btnUpCode==pressdown)
				{
					GPIO_WriteLow(enStepH);
					GPIO_WriteLow(dirStepH);
					Tuner_resetCntStep(&tuner);
					Tuner_setSP(&tuner,max_V);
					n_timer=Tuner_computer(&tuner);
					TIM2_SetAutoreload(n_timer);
					
					TIM2_Cmd(ENABLE);
					GPIO_WriteLow(ledMinH);
					heightMot=moveUpAccelH;
				}
				break;	
			case stopMaxH:
				if (btnDownCode==pressdown)
					{
						GPIO_WriteLow(enStepH);
						GPIO_WriteHigh(dirStepH);
						
						Tuner_setSP(&tuner,max_V);
						n_timer=Tuner_computer(&tuner);
						TIM2_SetAutoreload(n_timer);
						
						TIM2_Cmd(ENABLE);
						GPIO_WriteLow(ledMaxH);
						heightMot=moveDownAccelH;
					}
				break;
			
				
				

			case moveUpAccelH:
				if (btnDownCode==pressdown)
				{
					heightMot=moveUpBrakeH;
					Tuner_setSP(&tuner,0);
				}
			
				if (GPIO_ReadInputPin(endMaxH)==0)
					{
						//TIM2_Cmd(DISABLE);
						GPIO_WriteHigh(ledMaxH);
						heightMot=moveUpBrakeH;
						Tuner_setSP(&tuner,0);	
						//break;
					}
				
				n_timer=Tuner_computer(&tuner);
				TIM2_SetAutoreload(n_timer);	
				break;
				
			case moveDownAccelH:
				if (btnUpCode==pressdown)
				{
					heightMot=moveDownBrakeH;
					Tuner_setSP(&tuner,0);	
				}
				
				if (GPIO_ReadInputPin(endMinH)==0)
					{
						//TIM2_Cmd(DISABLE);
						//GPIO_WriteHigh(ledMinH);
						heightMot=moveDownBrakeH;
						Tuner_setSP(&tuner,0);	
						//break;
					}
				//else
				n_timer=Tuner_computer(&tuner);
				TIM2_SetAutoreload(n_timer);	
				break;
				
			case moveUpBrakeH:
				n_timer=Tuner_computer(&tuner);
					
				if (n_timer==0)
				{
					GPIO_WriteHigh(enStepH);
					TIM2_Cmd(DISABLE);
					heightMot=stopMaxH;
					if (GPIO_ReadInputPin(endMaxH)==0)
					{
						heightMot=stopMaxH;
						//GPIO_WriteHigh(ledMaxH);
					}
					else
					{
						heightMot=stopH;
					}
				}
				else
				{
					TIM2_SetAutoreload(n_timer);
				}
				break;
				
			case moveDownBrakeH:
				n_timer=Tuner_computer(&tuner);
					
				if (n_timer==0)
				{
					GPIO_WriteHigh(enStepH);
					TIM2_Cmd(DISABLE);
					
					if (GPIO_ReadInputPin(endMinH)==0)
					{
						heightMot=stopMinH;
						//GPIO_WriteHigh(ledMinH);
					}
					else
					{
						heightMot=stopH;
					}
					
					
				}
				else
				{
					TIM2_SetAutoreload(n_timer);
				}
				break;
				
			case errorH:
			
				break;
		}
		
	if (heightMot==stopH||	heightMot==stopMinH||heightMot==stopMaxH )
	{
		if (GPIO_ReadInputPin(endMinH)==0)
		{
			GPIO_WriteHigh(ledMinH);
		}
		else
		{
			GPIO_WriteLow(ledMinH);
		}
		
		if (GPIO_ReadInputPin(endMaxH)==0)
		{
			GPIO_WriteHigh(ledMaxH);
		}
		else
		{
			GPIO_WriteLow(ledMaxH);
		}
	}	
		
	switch(karetMot)
		{
			case stopK:
				if (btnRightCode==pressdown)
				{
					GPIO_WriteLow(enStepK);
					GPIO_WriteLow(dirStepK);
					Tuner_resetCntStep(&tunerK);
					Tuner_setSP(&tunerK,max_V);
					n_timer=Tuner_computer(&tunerK);
					TIM3_SetAutoreload(n_timer);
					TIM3_Cmd(ENABLE);
					
					GPIO_WriteLow(ledMinK);
					GPIO_WriteLow(ledMaxK);
					
					karetMot=moveRightAccelK;
					break;
				}
				else 
				if (btnLeftCode==pressdown)
				{
					GPIO_WriteLow(enStepK);
					GPIO_WriteHigh(dirStepK);
					
					Tuner_resetCntStep(&tunerK);
					Tuner_setSP(&tunerK,max_V);
					n_timer=Tuner_computer(&tunerK);
					TIM3_SetAutoreload(n_timer);
					TIM3_Cmd(ENABLE);
					
					GPIO_WriteLow(ledMinK);
					GPIO_WriteLow(ledMaxK);
					
					karetMot=moveLeftAccelK;
				}
				break;
			case	stopMinK:
				if (btnRightCode==pressdown)
				{
					GPIO_WriteLow(enStepK);
					GPIO_WriteLow(dirStepK);
					Tuner_resetCntStep(&tunerK);
					Tuner_setSP(&tunerK,max_V);
					n_timer=Tuner_computer(&tunerK);
					TIM3_SetAutoreload(n_timer);
					TIM3_Cmd(ENABLE);
					
					GPIO_WriteLow(ledMinK);
					GPIO_WriteLow(ledMaxK);
					
					karetMot=moveRightAccelK;
				}
				
				break;	
			case stopMaxK:
				if (btnLeftCode==pressdown)
				{
					GPIO_WriteLow(enStepK);
					GPIO_WriteHigh(dirStepK);
					
					Tuner_resetCntStep(&tunerK);
					Tuner_setSP(&tunerK,max_V);
					n_timer=Tuner_computer(&tunerK);
					TIM3_SetAutoreload(n_timer);
					TIM3_Cmd(ENABLE);
					
					GPIO_WriteLow(ledMinK);
					GPIO_WriteLow(ledMaxK);
					
					GPIO_WriteLow(ledMinK);
					GPIO_WriteLow(ledMaxK);
					
					karetMot=moveLeftAccelK;
				}
				break;
			
			case moveRightAccelK:
				if (btnRightCode==pressup||GPIO_ReadInputPin(endMaxK)==0)
				{
					karetMot=moveRightBrakeK;
					Tuner_setSP(&tunerK,0);
				}
	
				n_timer=Tuner_computer(&tunerK);
				TIM3_SetAutoreload(n_timer);	
				break;
				
			case moveLeftAccelK:
				if (btnLeftCode==pressup ||GPIO_ReadInputPin(endMinK)==0)
				{
					karetMot=moveLeftBrakeK;
					Tuner_setSP(&tunerK,0);	
				}
				
				n_timer=Tuner_computer(&tunerK);
				TIM3_SetAutoreload(n_timer);	
				break;
				
			case moveRightBrakeK:
				n_timer=Tuner_computer(&tunerK);
					
				if (n_timer==0)
				{
					GPIO_WriteHigh(enStepK);
					TIM3_Cmd(DISABLE);
					karetMot=stopMaxK;
					if (GPIO_ReadInputPin(endMaxK)==0)
					{
						karetMot=stopMaxK;
						//GPIO_WriteHigh(ledMaxK);
					}
					else
					{
						karetMot=stopK;
					}
				}
				else
				{
					TIM2_SetAutoreload(n_timer);
				}
				break;
				
			case moveLeftBrakeK:
				n_timer=Tuner_computer(&tunerK);
					
				if (n_timer==0)
				{
					GPIO_WriteHigh(enStepK);
					TIM3_Cmd(DISABLE);
					
					if (GPIO_ReadInputPin(endMinK)==0)
					{
						karetMot=stopMinK;
						//GPIO_WriteHigh(ledMinK);
					}
					else
					{
						karetMot=stopK;
					}
				}
				else
				{
					TIM3_SetAutoreload(n_timer);
				}
				break;
				
			case errorK:
			
				break;
		}	
	
	if (karetMot==stopK||	karetMot==stopMinK||karetMot==stopMaxK )
	{
		if (GPIO_ReadInputPin(endMinK)==0)
		{
			GPIO_WriteHigh(ledMinK);
		}
		else
		{
			GPIO_WriteLow(ledMinK);
		}
		
		if (GPIO_ReadInputPin(endMaxK)==0)
		{
			GPIO_WriteHigh(ledMaxK);
		}
		else
		{
			GPIO_WriteLow(ledMaxK);
		}
	}
		
		
		/*
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
	*/
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
	// конец цикла
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
