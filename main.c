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
#define enStep1  GPIOF, GPIO_PIN_5
#define clkStep1 GPIOH, GPIO_PIN_2
#define dirStep1 GPIOF, GPIO_PIN_7
#define Buzz 	  GPIOD, GPIO_PIN_2
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
// Описание пинов
#define tm1637_1	GPIOB, GPIO_PIN_3, GPIOB, GPIO_PIN_6
#define tm1637_2	GPIOB, GPIO_PIN_2, GPIOB, GPIO_PIN_6
#define tm1637_3	GPIOB, GPIO_PIN_1, GPIOB, GPIO_PIN_6
#define tm1637_4	GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_6
// Окончание пинов
uint8_t a[]={0x71,0x74,0x07,0x33,0x02};

char st[50];
//char simvol;		



tm1637_t disp1, disp2, disp3, disp4 ;


// NACK - 1
// ACK  - 0


char st1[]=" FFF";
uint16_t timer1,adc2;
//int16_t a,b;
// регулятор оборотов
typedef enum
{
	autoreg,
  manual	
} typeReg_t;
typedef enum
{
	stop,
	calc,
	work
} stateReg_t;
typedef enum
{
	stateV_minus,
	stateVmax_plus
}stateDrive_t;

typedef enum
{
	dirStepFar,	// дальше от источника ++
	dirStepNear	// ближе к источнику	--
}dirStep_t;

#define max_ar 200 // размер массива
typedef struct
{
	// настройка таблицы для разгона и торможения
	uint16_t v_max; 			// максимальные обороты 
	uint16_t t_accel_ms; 	// время разгона
	uint16_t t_break_ms; 	// время торможения
	uint16_t stepTurn; 		// шагов на оборот 400-800-1000
	uint8_t dt; 					// дискретное время расчета 10-20 мс
	// настройка регулятора
	int32_t stepPV;			// текущее значение шага
	int32_t stepSP;			// задание для перемещения
	
	
	int32_t stepAbs;			// абсолютная координата
	dirStep_t dirStep;		//  направление движения
	
	uint16_t F_accel[max_ar]; // частота тактирования
	uint16_t Step_accel[max_ar]; // кол-во шагов ускорения 
	uint16_t Cnt_accel[max_ar]; // Счетчик для таймера 
  TIM2_Prescaler_TypeDef Pres_accel[max_ar];	// Предделитель для таймера
	
	uint16_t F_break[max_ar];
	uint16_t Step_break[max_ar];
	uint16_t Cnt_break[max_ar];
  uint8_t Pres_break[max_ar];
	
	uint8_t cur_accel;   	// текушая позиция для отработки по Ускорению
	uint8_t cur_break;		// текущая позиция для отработки по Торможению
	uint32_t cur_step;    // текущий шаг
	
	uint8_t num_accel;
	uint8_t num_break;
	uint32_t dStepAccel; // шаги для
	uint32_t dStepBreak;
	stateDrive_t stateDrive;
}stepreg_t;

stepreg_t stepreg;



void StepReg_Init(stepreg_t *stepreg, 
									uint16_t v_max, 			// максимальные обороты 
									uint16_t t_accel_ms, 	// время разгона
									uint16_t t_break_ms, 	// время торможения
									uint16_t stepTurn, 		// шагов на оборот
									uint32_t Fcpu,			// частота тактирования таймера
									uint8_t dt 					// дискретное время расчета 10-20 мс
								)
{
	uint8_t i;
	Tim2_InitSet_t pres;
	stepreg->v_max=v_max;
	stepreg->t_accel_ms=t_accel_ms;
	stepreg->t_break_ms=t_break_ms;
	stepreg->stepTurn=stepTurn;
	stepreg->dt=dt;
	stepreg->cur_accel=0;
	stepreg->cur_break=0;
	stepreg->cur_step=0;
//	stepreg->half_step=0;
	for (i=0;i<max_ar;i++)
	{
		stepreg->F_accel[i]= (uint32_t)v_max*(uint32_t)stepTurn*(uint32_t)dt*(uint32_t)(i+1)/((uint32_t)60*(uint32_t)t_accel_ms);
		stepreg->F_break[i]= (uint32_t)v_max*(uint32_t)stepTurn*(uint32_t)dt*(uint32_t)(i+1)/((uint32_t)60*(uint32_t)t_break_ms);
		stepreg->Step_accel[i]=(uint32_t)stepreg->F_accel[i]*dt/1000*2;
		stepreg->Step_break[i]=(uint32_t)stepreg->F_break[i]*dt/1000*2;
		pres=CalcSetTim(Fcpu, stepreg->F_accel[i]*2);
		stepreg->Cnt_accel[i]=pres.cnt;	// значение счетчика для перезагрузки
		stepreg->Pres_accel[i]=pres.prescaler;	// значение предделителя
		pres=CalcSetTim(Fcpu, stepreg->F_break[i]*2);
		stepreg->Cnt_break[i]=pres.cnt;
		stepreg->Pres_break[i]=pres.prescaler;
	}
}





void StepReg_Calc(stepreg_t *stepreg, 
									uint32_t stepSet			// настройка данных
									)
{
	uint32_t s=0,s_accel=0,s_break=0;
	uint8_t i=0,j=0;
	uint32_t last_step,dStep,dStepAccel,dStepBreak, Fmax;
	stateDrive_t stateDrive;
	stepSet=stepSet*2;
	//Расчитываем максимальную скорость
	Fmax=(uint32_t)stepreg->v_max*stepreg->stepTurn/60 ;
	while(1)
	{
		// Ускоряемся
		if (stepreg->F_accel[i] <Fmax )
		{
			// Добавляем ускорение
			i++;
			s=s+stepreg->Step_accel [i-1];
			s_accel+=stepreg->Step_accel [i-1];
			// Добавляем торможение если скорость торможения меньше ускорения
			if (stepreg->F_break[j]< stepreg->F_accel[i-1])
			{
				s_break+=stepreg->Step_break [j];
				s=s+stepreg->Step_break [j];
				j++;
			}
			
			// если превысли максимальное кол-во шагов
			if (s>=stepSet )
			{
				dStep=s-stepSet;
				
				if (stepreg->Step_accel[i-1]>dStep)
				{
					stepreg->dStepAccel=dStep;
					stepreg->dStepBreak=0;
				}
				else
				if (stepreg->Step_accel[i-1]==dStep)
				{
					stepreg->dStepAccel=0;
					stepreg->dStepBreak=0;
					i--;
				}
				else
				{
					dStep-=stepreg->Step_accel[i-1];
					i--;
					stepreg->dStepAccel=0;
					stepreg->dStepBreak=dStep;
				}
				stepreg->num_accel=i;
				stepreg->num_break=j;
				stepreg->stateDrive=stateV_minus;
				break;
			}
		}
		else
		{
			// Вышли на максимальную скорость
			// расчитываем оставшиеся шаги
			stepreg->stateDrive=stateVmax_plus;
			stepreg->dStepAccel=stepSet-s;
			stepreg->dStepBreak=0;
			stepreg->num_accel=i;
			stepreg->num_break=j;
			break;
		}		
		// Тормозимся
	}
	
}

Tim2_InitSet_t tim2_Set;

#ifdef  __OSA__

void Task_2(void)
{
	

	OS_Delay(20);
}




void Task(void)
{
	uint8_t i,temp;
	// настройка прототипа
	StepReg_Init(&stepreg, 
									1000, 	// максимальные обороты 
									2000, 	// время разгона
									1500, 	// время торможения
									800, 		// шагов на оборот
									16000000, // частота тактирования таймера
									20 			// дискретное время расчета 10-20 мс
								);
	StepReg_Calc(&stepreg, 
							 1000// настройка данных
									);
	// настройка тактирования
	//tim2_Set=CalcSetTim(16000000, 13333);
	
	//TIM2_TimeBaseInit(TIM2_Prescaler_TypeDef TIM2_Prescaler, uint16_t TIM2_Period);
	
	//GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_FAST);
	// Настройка пинов
//	OS_Task_Create(7, Task_2); // создаем задачу
	Init_Delay();
	GPIO_Init(enStep1, GPIO_MODE_OUT_PP_HIGH_FAST); // Включен Step
	GPIO_Init(clkStep1, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(dirStep1, GPIO_MODE_OUT_PP_LOW_FAST);
	
	//GPIO_WriteHigh(enStep1);
	//GPIO_WriteLow(enStep1);
	for(i=0;i<10;i++)
	{
		GPIO_WriteHigh(clkStep1); // делает шаг
		delay_ms(10);
	  GPIO_WriteLow(clkStep1);
	}
	
	
	GPIO_Init(Buzz, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteHigh(Buzz);
	GPIO_WriteLow(Buzz);
	
	GPIO_Init(Buzz, GPIO_MODE_OUT_PP_LOW_FAST);
	
	
	TM1637_Init(&disp1, tm1637_1,Brightness0);
	TM1637_Init(&disp2, tm1637_2,Brightness2);
	TM1637_Init(&disp3, tm1637_3,Brightness4);
	TM1637_Init(&disp4, tm1637_4,Brightness6);
	ADC2_SingleOne_Init(	ADC2_PRESSEL_FCPU_D10, 
													ADC2_Handler_NoIT);
	timer1=0;	
	while(1)
	{
		//GPIO_WriteReverse(GPIOB, GPIO_PIN_1);
		//ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_12);
		sprintf(st,"%4d",timer1++);
		
		ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_12);
		adc2= ADC2_SingleOne_GetConversion();
		sprintf(st,"%4d",adc2);
		TM1637_TextOutput(&disp1,st);
		
		ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_11);
		adc2= ADC2_SingleOne_GetConversion();
		sprintf(st,"%4d",adc2);
		TM1637_TextOutput(&disp2,st);
		
		ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_10);
		adc2= ADC2_SingleOne_GetConversion();
		sprintf(st,"%4d",adc2);
		TM1637_TextOutput(&disp3,st);
		ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_7);
		adc2= ADC2_SingleOne_GetConversion();
		sprintf(st,"%4d",adc2);
		TM1637_TextOutput(&disp4,st);
		
		
		
		OS_Delay(500);
	}
}
#endif


uint16_t nmb=0,adc0;
uint32_t clks;
//-----

i2cStatus_t res;

FlagStatus adcStatus;

Tim2_InitSet_t tim2_Set;
void clockStep(void)
{
	// Проверка на наличие шагов для ускорения
	if (stepreg.cur_accel<stepreg.num_accel)
	{
		//---------------------------
		//Отработка шагов ускорения 
		//---------------------------
		// если шаг ускорения не является последним
		if (stepreg.cur_accel<stepreg.num_accel-1)
		{
			//---------------------------
			//Отработка шагов ускорения предпоследних шагов
			//---------------------------
			// Если это не последний такт в площадке, то 
			// просто производим тактирование
			if (stepreg.cur_step<stepreg.Step_accel[stepreg.cur_accel] )
			{
				//---------------------------
				//Отработка тактирования двигателя
				//---------------------------
				// Производим тактирование
				if (stepreg.cur_step%2==0)
				{
					GPIO_WriteHigh(clkStep1); // делает шаг
					if (stepreg.dirStep==dirStepFar)
					{
						stepreg.stepAbs++;
					}
					else
					{
						stepreg.stepAbs--;
					}
				}
				else
				{
					GPIO_WriteLow(clkStep1);	// возврат назад
				}
				// Увеличиваем счетчик тактов
				stepreg.cur_step++;
				// Отслеживаем переполнение счетчика шагов
				if (stepreg.cur_step>=stepreg.Step_accel[stepreg.cur_accel] )
				{
					//-------------------
					// Отработали последний шаг тактирования 
					//--------------------
					// Загрузка параметров для нового шага
					stepreg.cur_step=0;
					stepreg.cur_accel++;
					
					//Загрузка нового значения для таймера
					TIM2_TimeBaseInit(stepreg.Cnt_accel[stepreg.cur_accel], 
														stepreg.Pres_accel[stepreg.cur_accel]);
				}
				
			}
			else
			{
				//---------------------------------
				// Если в шаге ускорения нет шагов тактирования 
				//----------------------------------
				// Загрузка параметров для нового шага
					stepreg.cur_step=0;
					stepreg.cur_accel++;
					
					//Загрузка нового значения для таймера
					TIM2_TimeBaseInit(stepreg.Cnt_accel[stepreg.cur_accel], 
														stepreg.Pres_accel[stepreg.cur_accel]);
			}
			
		}
		else
		{
			//----------------------------------
			// Отработка последего шага ускорения
			//-----------------------------------
			
			//---------------------------
			//Отработка тактирования двигателя
			//---------------------------
			// Производим тактирование
			if (stepreg.cur_step%2==0)
			{
				GPIO_WriteHigh(clkStep1); // делает шаг
			}
			else
			{
				GPIO_WriteLow(clkStep1);	// возврат назад
			}
			// Увеличиваем счетчик тактов
			stepreg.cur_step++;	
			
				
			// Проверяем состояние 	
			if (stepreg.stateDrive==stateVmax_plus)
			{
				//----------------------------------
				// Достигли макс. скорости
				//-----------------------------------
				if (stepreg.cur_step>=stepreg.Step_accel[stepreg.cur_accel]+stepreg.dStepAccel)
				{
					//------------------------
					// Переходим в область Торможения
					//------------------------
					
					
					//-------------------
					// Отработали последний шаг тактирования 
					//--------------------
					// Загрузка параметров для нового шага
					stepreg.cur_step=0;
					stepreg.cur_accel++;
					
					//Загрузка нового значения для таймера
					TIM2_TimeBaseInit(stepreg.Cnt_accel[stepreg.num_break-stepreg.cur_break-1], 
														stepreg.Pres_accel[stepreg.num_break-stepreg.cur_break-1]);
				}
			}
			else
			{
				//----------------------------------
				// Макс скорости не достигли
				//-----------------------------------
				// stateV_minus
				if (stepreg.cur_step+stepreg.dStepAccel>=stepreg.Step_accel[stepreg.cur_accel])
				{
					//------------------------
					// Переходим в область Торможения
					//------------------------
					//-------------------
					// Отработали последний шаг тактирования 
					//--------------------
					// Загрузка параметров для нового шага
					stepreg.cur_step=0;
					stepreg.cur_accel++;
					
					//Загрузка нового значения для таймера
					TIM2_TimeBaseInit(stepreg.Cnt_accel[stepreg.num_break-stepreg.cur_break-1], 
														stepreg.Pres_accel[stepreg.num_break-stepreg.cur_break-1]);
				}
				
			}
			
		}
	}
	else
	{
		//---------------------------
		//Отработка шагов торможения 
		//---------------------------
		
		// Отработка режима шагов на торможения
		//
		
		if (stepreg.cur_break<stepreg.num_break )
		{
			if (stepreg.cur_break==0)
			{
				//------------------
				// Первый шаг
				//------------------
				if (stepreg.cur_step%2==0)
				{
					GPIO_WriteHigh(clkStep1); // делает шаг
				}
				else
				{
					GPIO_WriteLow(clkStep1);	// возврат назад
				}
				// Увеличиваем счетчик тактов
				stepreg.cur_step++;
				if (stepreg.num_break==1)
				{
					//--------
					// Последний шаг, небходимо заканчивать движение
					//----------------
					
				}
				else
				{
					//-----------------------
					// Есть другие шаги, переходим на них
					//----------------

				}
				
			}
			else
			if (stepreg.cur_break<stepreg.num_break-1 )
			{
				// Предпоследние Шаги торможения
				
				
				//---------------------------
				//Отработка тактирования двигателя
				//---------------------------
				// Производим тактирование
				if (stepreg.cur_step%2==0)
				{
					GPIO_WriteHigh(clkStep1); // делает шаг
				}
				else
				{
					GPIO_WriteLow(clkStep1);	// возврат назад
				}
				// Увеличиваем счетчик тактов
				stepreg.cur_step++;
				// Проверка на окончание тактов	
				if (stepreg.cur_step>=stepreg.Step_accel[stepreg.cur_accel] )
				{
					//-------------------
					// Отработали последний шаг тактирования 
					//--------------------
					// Загрузка параметров для нового шага
					stepreg.cur_step=0;
					stepreg.cur_accel++;
					
					//Загрузка нового значения для таймера
					TIM2_TimeBaseInit(stepreg.Cnt_accel[stepreg.cur_accel], 
														stepreg.Pres_accel[stepreg.cur_accel]);
				}	
					
			}
			else
			{
				// Последний шаг торможения
				
				
				//---------------------------
				//Отработка тактирования двигателя
				//---------------------------
				// Производим тактирование
				if (stepreg.cur_step%2==0)
				{
					GPIO_WriteHigh(clkStep1); // делает шаг
				}
				else
				{
					GPIO_WriteLow(clkStep1);	// возврат назад
				}
				// Увеличиваем счетчик тактов
				stepreg.cur_step++;	
				
			}
			
		}
		else
		{
			// остановка
			// защита от ошибок
			
		}
	}
	
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(1000); //Настраиваем прерывание 500мкс
//	OS_Task_Create(7, Task); // создаем задачу
	OS_EI();   // Разрешить все прерывания
	OS_Run(); // Запуск ядра RTOS OSA
#else
	/* Infinite loop */
//	cf_u=&cf;
	uint8_t i;
	Init_Delay();
	
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
	StepReg_Init(&stepreg, 
									1000, 			// максимальные обороты 
									2000, 	// время разгона
									1500, 	// время торможения
									800, 		// шагов на оборот
									20 					// дискретное время расчета 10-20 мс
								);
	StepReg_Calc(&stepreg, 
							 1000// настройка данных
									);
	
	//GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	//GPIO_Init(enStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_Init(clkStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_Init(dirStep, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	
	
	//TIM2_Cmd(ENABLE);
	enableInterrupts()
	while (1)
  {
		
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
