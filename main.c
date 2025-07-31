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
#include <math.h>

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
//uint8_t a[]={0x71,0x74,0x07,0x33,0x02};

char st[50];
//char simvol;		



tm1637_t disp1, disp2, disp3, disp4 ;


// NACK - 1
// ACK  - 0


char st1[]=" FFF";
uint16_t timer1,adc2;
//int16_t a,b;
// регулятор оборотов

/*
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
*/

/*
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
		stepreg->Cnt_accel[i]=pres.cnt;						// значение счетчика для перезагрузки
		stepreg->Pres_accel[i]=pres.prescaler;		// значение предделителя
		pres=CalcSetTim(Fcpu, stepreg->F_break[i]*2);
		stepreg->Cnt_break[i]=pres.cnt;
		stepreg->Pres_break[i]=pres.prescaler;
	}
}

*/


/*
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
*/

typedef enum
{
	stepUp,
	stepDown
}stepDir_t;

typedef enum
{
	stepTact=0, // Шаг тактирования
	stepIdle=1 // шаг ожидания
}stepTitTak_t;


typedef struct
{
	FunctionalState EnableTimer; // включение/отключение таймера
	uint16_t cnt;
	TIM2_Prescaler_TypeDef prescaler;
}PID_Tact_Timer2_t;

#define tabSize 200


typedef enum
{
	gen_idle=0,
	gen_idle_right,
	gen_idle_left,
	gen_run,
	gen_auto,
	gen_stop,
	gen_man
	
}state_gen_t;

typedef enum
{
	single_off,
	single_on
}single_stage_t;


typedef enum
{
	v_up,
	v_down
}v_state_t;

typedef struct
{
	uint16_t autoreload;			//  автозагрузка таймера
	uint8_t divBin:4;   // Делитель таймера
	v_state_t state:1; 	// тип взаимодействия
	uint16_t step;			//  кол-во шагов
}v_comm_t;

typedef struct
{
	// Настройка ПИД-регулятора
	int32_t stepPV; // текущее значение
	int32_t stepSP;	// задание
	float Kp;				// коэффициент пропорциональности
	float Ki;				// коэффициент интергрирования
	float Kd;				// коэффициент дифференцирования
	float wOP;	// скорость необходимая
	float wMax;  // максимальные обороты в минуту в прямом направлении
	//float wMin; // максимальные обороты в противоположном направлении
	int32_t e[3];		// ошибки, последняя ошибка e[2]
	//-------------------------------
	// Расчет частоты тактирования скорости
	uint32_t f_tact; // частота тактирования таймера
	uint16_t microStep; // кол-во микрошагов в обороте
	stepDir_t stepDir; // направление вращения
	stepTitTak_t stepTI; // определяем шаг
	uint16_t cnt;			// значение перезагрузки
	TIM2_Prescaler_TypeDef prescaler; // предделитель
	FunctionalState EnableTimer; // флаг работы таймера
	GPIO_TypeDef* Port_Clk; // Пин 
	GPIO_Pin_TypeDef Pin_Clk;
	GPIO_TypeDef* Port_Dir;
	GPIO_Pin_TypeDef Pin_Dir;
	//------------------
	// Таблица 
	uint16_t stepNum; // номер шага итерации 
	uint16_t t_accel; // время ускорения /торможения для максимальной скорости
	uint16_t t_break; // время ускорения /торможения для максимальной скорости
	uint16_t v_accel; // время ускорения /торможения для максимальной скорости
	uint8_t dt; 			// период расчета таблицы в ms
	uint16_t stepV_Max; // максимальное кол-во шагов изменения задания
	v_comm_t v_comm[tabSize]; // общая таблица ускорения/ торможения
	uint16_t v_comm_size; // размер расчитанной таблицы
	//------------------
	state_gen_t state_gen; // состояние генерирования сигнала
  int32_t snapPV; // Виртуальная PV на момент запуска
	int32_t snapSP; // Виртуальное SP на момент запусказадание
	stepDir_t snapDir; // Направление движения
	
	//------------
	int32_t 				snap_PV;
	int32_t 				snap_SP;
	int32_t 				snap_dSP;
	
	uint16_t 				snap_numSymm; 		// кол-во шаг для разгона, не включая 
	//single_stage_t 	snap_singleStage; // Необходимость последнего шага
	uint16_t 				snap_uniqStep;			// Номер шага для включения точного шага
	uint32_t 				snap_uniqCnt; 		// Кол-во тактов для включения точного шага
	uint16_t				snap_accelStep; 	// Номер максимального шага для ускорения
	uint16_t				snap_breakStep;	// Номер максимального шага для торможения
	uint16_t 				snap_numCur;			// номер шага в таблице
	uint32_t				snap_cntCut;			// кол-во отработанных шагов 
	//---------------------------------
	
	
	
}pidW_t;
// структура 
pidW_t pidW;

#define noStep 0xFFFF



void PID_getStep(pidW_t *pidW)
{
	if (pidW->snap_dSP)
	{
		if ( pidW->snap_numCur < pidW->v_comm_size-1 )
		{
			find_up:
			// не достигли точки максимальной скорости
			if ( pidW->snap_dSP - pidW->v_comm[pidW->snap_numCur].step > 0   )
			{
				// вычитаем значение из задания
				pidW->snap_dSP  =	pidW->snap_dSP - pidW->v_comm[pidW->snap_numCur].step;
				if (pidW->v_comm[pidW->snap_numCur].state==v_up)
				{
					// делать такт
				}
				else
				{
					// поиск нового такта
					goto find_up; 
				}
			}
		}
		else
		{
			// достигли точки постоянной скорости
			
		}
		
		
		if ( pidW->snap_dSP - pidW->v_comm[pidW->snap_numCur].step >=0 )
		{
			if (pidW->v_comm[pidW->snap_numCur].state  )
			{
			}
		}
	}
}


void PID_stepGpio(pidW_t *pidW )
{
	// режим авто
	uint16_t i, temp;
	if (pidW->state_gen  == gen_auto)
	{
		//repeat_stepGpio:
		run_man:
		if (pidW->snap_accelStep < noStep)
		{
			GPIO_WriteHigh(pidW->Port_Clk, pidW->Pin_Clk);
			// Фиксация изменения координаты
			if (pidW->snapDir == stepUp)
			{
				pidW->snap_PV++;
			}
			else
			{
				pidW->snap_PV--;
			}
			
			//--------------------------
			
			if (pidW->snap_cntCut <  pidW->v_comm[pidW->snap_numCur].step -1 )
			{
				pidW->snap_cntCut++;
			}
			else
			{
				// последний такт на шаге
				pidW->snap_cntCut=0;
				// Поиск направления
				if ( pidW->snap_numCur < pidW->snap_accelStep)
				{
					// Поиск нового шага
					for(i=pidW->snap_numCur+1; i<=pidW->snap_accelStep;  i++)
					{
						if(pidW->v_comm[i].state==v_up)
						{
							pidW->snap_numCur=i;
							TIM2_TimeBaseInit(pidW->v_comm[i].divBin, 
																pidW->v_comm[i].autoreload);
							break;
						}
					}
				}
				else
				if (pidW->snap_numCur == pidW->snap_accelStep)
				{
					pidW->snap_accelStep = noStep;
					if ( pidW->snap_uniqStep < noStep )
					{
						pidW->snap_numCur =pidW->snap_uniqStep;
						TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_numCur].divBin, 
															pidW->v_comm[pidW->snap_numCur].autoreload);
					}
					else
					if (pidW->snap_breakStep<noStep)
					{
						pidW->snap_numCur =pidW->snap_breakStep;
						TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_numCur].divBin, 
															pidW->v_comm[pidW->snap_numCur].autoreload);
					}
					else
					{
						pidW->state_gen  = gen_stop;
						pidW->snap_numCur=noStep;
						TIM2_Cmd(DISABLE);
					}
					
				}
			}
		}
		else
		if (pidW->snap_uniqStep < noStep)
		{
			if (pidW->snap_cntCut < pidW->snap_uniqCnt-1 )
			{
				pidW->snap_cntCut++; 
			}
			else
			{
				pidW->snap_cntCut=0;
				pidW->snap_uniqStep = noStep;
				if (pidW->snap_breakStep<noStep)
				{
					pidW->snap_numCur =pidW->snap_breakStep;
					TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_numCur].divBin, 
														pidW->v_comm[pidW->snap_numCur].autoreload);
				}
				else
				{
					pidW->state_gen  = gen_stop;
					pidW->snap_numCur=noStep;
					TIM2_Cmd(DISABLE);
				}
			}
		}
		else
		if (pidW->snap_breakStep  < noStep)
		{
			break_gen_stop:
			if (pidW->snap_cntCut <  pidW->v_comm[pidW->snap_numCur].step -1  )
			{
				pidW->snap_cntCut++; 
			}
			else
			{
				pidW->snap_cntCut=0;
				
				while(pidW->snap_numCur)
				{
					pidW->snap_numCur--;
					if(pidW->v_comm[pidW->snap_numCur].state==v_down)
					{
						TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_numCur].divBin, 
															pidW->v_comm[pidW->snap_numCur].autoreload);
						goto last_snapStep;
						break;
					}
				}
				
				if (pidW->snap_numCur==0)
				{
					pidW->snap_breakStep  = noStep;
					pidW->snap_numCur=noStep;
					pidW->state_gen  = gen_stop;
					TIM2_Cmd(DISABLE);
				}
				
				last_snapStep:
				nop();
				//last_snapStep:
			}
		}
		else
		{
			pidW->snap_numCur=noStep;
			pidW->state_gen  = gen_stop;
			TIM2_Cmd(DISABLE);
		}
	}
	else
	//-------------------------------------------
	// остановка
	if (pidW->state_gen  == gen_stop  )
	{
		if (pidW->snap_numCur < noStep)
		{
			GPIO_WriteHigh(pidW->Port_Clk, pidW->Pin_Clk);
			// фиксация 
			if (pidW->snapDir == stepUp)
			{
				pidW->snap_PV++;
			}
			else
			{
				pidW->snap_PV--;
			}
			//---------------
			if (pidW->v_comm[pidW->snap_numCur].state==v_up)
			{
				// поиск нового шага для торможения
				if (pidW->snap_numCur>0)
				{
					for(i=0;i<pidW->snap_numCur-1;i++)
					{
						temp=pidW->snap_numCur-i-1;
						if(pidW->v_comm[temp].state==v_down && pidW->v_comm[temp].step>0)
						{
							TIM2_TimeBaseInit(pidW->v_comm[temp].divBin, 
																pidW->v_comm[temp].autoreload);
							pidW->snap_cntCut=0;
							pidW->snap_numCur=temp;
							goto run_gen_stop;
						}
					}
					// не нашли шагов для остановки
					goto stop_gen_stop;
				}
				else
				{
					stop_gen_stop:
					pidW->snap_numCur=noStep;
					pidW->state_gen  = gen_stop;
					TIM2_Cmd(DISABLE);
				}
				// нашли шаг продолжаем остановку
				run_gen_stop:
				nop();
			}
			else
			{
				// находимся на треке падения скорости
				goto break_gen_stop;
			}
			
			//pidW->snap_numCur=noStep;
			//pidW->state_gen  = gen_stop;
			//TIM2_Cmd(DISABLE);
		}
		else
		{
			pidW->snap_numCur=noStep;
			pidW->state_gen  = gen_stop;
			TIM2_Cmd(DISABLE);
		}
	}
	else
	if (pidW->state_gen  == gen_man )
	{
		
//		if (pidW->snap_numCur < pidW-> )
		GPIO_WriteHigh(pidW->Port_Clk, pidW->Pin_Clk);
		// фиксация 
		if (pidW->snapDir == stepUp)
		{
			pidW->snap_PV++;
		}
		else
		{
			pidW->snap_PV--;
		}
		// переход между шагами
		if ( pidW->snap_numCur < noStep)
		{
			// двигатель вращается 
		}
		else
		{
			// двигатель стоит
			pidW->snap_numCur=0;
			
			if (pidW->snap_cntCut <  pidW->v_comm[pidW->snap_numCur].step -1  )
			{
				pidW->snap_cntCut++; 
			}
			else
			{
				pidW->snap_cntCut=0;
			}
		}
	}
	//exit_stepGpio:
	
	GPIO_WriteLow(pidW->Port_Clk, pidW->Pin_Clk);
}
//--------------------------
// запуск таймера
void PID_StartAuto(pidW_t *pidW)
{
	uint16_t i;
	// настраиваем направление
	if (pidW->snapDir ==  stepUp)
	{
		GPIO_WriteHigh(pidW->Port_Dir, pidW->Pin_Dir);
	}
	else
	{
		GPIO_WriteLow(pidW->Port_Dir, pidW->Pin_Dir);
	}
	// загрузка данных в таймер
	repeat_StartAuto:
	if (pidW->snap_accelStep <noStep)
	{
		// загрузить настройки таймер
		for(i=0;i<=pidW->snap_accelStep;i++)
		{
			if(pidW->v_comm[i].state==v_up )
			{
				TIM2_TimeBaseInit(pidW->v_comm[i].divBin, pidW->v_comm[i].autoreload);
				pidW->snap_cntCut=0;
				pidW->snap_numCur=i;
				pidW->state_gen  = gen_auto;
				TIM2_Cmd(ENABLE);
				return;
				//break;
			}
		}
		pidW->snap_accelStep =noStep;
		goto repeat_StartAuto;
	}
	else
	if(pidW->snap_uniqStep < noStep)
	{
		// загрузить настройки таймер
		TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_uniqStep].divBin, 
											pidW->v_comm[pidW->snap_uniqStep].autoreload);
		pidW->snap_cntCut=0;
		pidW->snap_numCur=pidW->snap_uniqStep;
		pidW->state_gen  = gen_auto;
		TIM2_Cmd(ENABLE);
	}
	else
	if (pidW->snap_breakStep <noStep)
	{
		// загрузить настройки таймер
		for(i=0;i<=pidW->snap_breakStep;i++)
		{
			if(pidW->v_comm[pidW->snap_breakStep-i].state==v_down )
			{
				TIM2_TimeBaseInit(pidW->v_comm[pidW->snap_breakStep-i].divBin, 
													pidW->v_comm[pidW->snap_breakStep-i].autoreload);
				pidW->snap_cntCut=0;
				pidW->state_gen  = gen_auto;
				pidW->snap_numCur=pidW->snap_breakStep-i;
				TIM2_Cmd(ENABLE);
				return;
			}
		}
		
	}
}




void PID_TabCalc(pidW_t *pidW, 
								int32_t snap_PV, // текущее значение 
								int32_t snap_SP  // задание
								)
{
	uint16_t i;
	int32_t s=0;
	pidW->snap_numCur=noStep;
	pidW->snap_cntCut=0;
	pidW->snap_PV=snap_PV;
	pidW->snap_SP=snap_SP;
	
	pidW->snap_accelStep=noStep;
	pidW->snap_breakStep=noStep;
	pidW->snap_uniqStep=noStep;
	
	if (snap_SP > snap_PV)
	{
		pidW->snap_dSP=snap_SP-snap_PV;
		pidW->snapDir=stepUp;
	}
	else
	{
		pidW->snap_dSP = snap_PV-snap_SP;
		pidW->snapDir = stepDown;
	}
	
	
	// если нет изменений, то запускать нечего
	if (pidW->snap_dSP==0)
	{
		return;
	}
	for(i=0; i<pidW->v_comm_size-1;i++)
	{
		if ( (s + pidW->v_comm[i].step) <=  pidW->snap_dSP  )
		{
			s =s + pidW->v_comm[i].step;
			// кол-во шагов для отработки
			if (pidW->v_comm[i].state ==  v_up)
			{
				pidW->snap_accelStep=i;
				
			}
			else
			{
				pidW->snap_breakStep=i;
			}
		}
		else
		{
			break;
		}
	}
	
	if (s <  pidW->snap_dSP)
	{
		pidW->snap_uniqStep=i;
		pidW->snap_uniqCnt= pidW->snap_dSP - s;
	}
	else
	{
		pidW->snap_uniqStep=noStep;
	}
	
	
	// i - содержит шаг на котором останавливается расчет
	
	
	
	/*
	uint32_t relSP, dSP;
	volatile
	uint32_t relPV,
	stepV_Max_num=0, // Кол-во повторений последнего шага
	value_last // кол-во повторений шагов последнего номер
	;
	volatile single_stage_t single_stage;
	volatile uint16_t i,
	num_symm=0, 		//счетчик для  симметричный номер для шага
	num_single=0xFFFF, // номер уникального шага
	
	uniq_num=0xFFFF, 		// уникальный номер шага 
	uniq_cnt=0 		// кол-во тактов уникального шага
	;
	single_stage=single_off;
	pidW->snap_PV=snapPV;
	pidW->snap_SP=snapSP;
	pidW->snap_numCur=0;
	if (snapSP>snapPV)
	{
		dSP=snapSP-snapPV;
		pidW->snapDir=stepUp;
	}
	else
	{
		dSP=snapPV-snapSP;
		pidW->snapDir=stepDown;
	}
	
	// расчет точек переходов
	relPV=0;
	i=0;
	num_symm=0; // кол-во симметричных шагов
	for(;i<10;i++)
	{
	}
	while(relPV != dSP)
	{
		if (  pidW->snap_numSymm  < (pidW->stepV_Max) )
		{
			
			// расчет восходящей+низходящей траектории
			if ( relPV+2*pidW->stepV[ pidW->snap_numSymm ] <= dSP )
			{
				relPV+=2*pidW->stepV[ pidW->snap_numSymm++ ];
			}
			else
			{
				
				// расчитываем последнюю точку
				if ( relPV+pidW->stepV[pidW->snap_numSymm]<=dSP )
				{
					relPV+=pidW->stepV[pidW->snap_numSymm];
					pidW->snap_VmaxNum=1;
					
				}
				else
				{
					pidW->snap_VmaxNum=0;
				}
				// проверка окончание расчета 
				uniq:
				if (relPV == dSP)
				{
					return;
				}
				// рассчитываем уникальную точку 
				for(i=0;i<pidW->stepV_Max;i++)
				{
					if (relPV + pidW->stepV[i]>=dSP  )
					{
						pidW->snap_uniqNum=i;
						pidW->snap_uniqCnt=dSP-relPV;
						relPV+=pidW->snap_uniqCnt;
						break;
					}
				}	
			}
		}
		else
		{
			// вышли на максимальную скорость
			// кол-во оставшихся шагов
			pidW->snap_VmaxNum=(dSP-relPV)/pidW->stepV[pidW->snap_numSymm];
			relPV+=pidW->snap_VmaxNum*pidW->stepV[pidW->snap_numSymm];
			goto uniq;
		}
	}
	return;
	*/
}

void PID_initTab(pidW_t *pidW,
									uint8_t dt,
									uint16_t t_accel,
									uint16_t t_break,
									uint16_t v_accel
									)
{
	uint16_t i,  v ;
	uint8_t divBin; // предделитель таймера бинарный
	uint8_t FlagWork=1; // Выход из цикла
	uint32_t stepV, v_last=0,v_cur=0,  
	f_temp; // предделитель
	uint32_t v_avg;
	uint32_t  v_accel_ms=0, 	v_break_ms=0;
	uint32_t 	v_accel_cur=0, 	v_break_cur=0,
						v_accel_last=0, v_break_last=0  ;
	pidW->dt=dt;
	pidW->t_accel=t_accel;
	pidW->t_break=t_break;
	pidW->v_accel=v_accel;
	
	// расчет общей таблицы
	i=0;
	v_accel_cur= v_accel* dt*(v_accel_ms+1)/t_accel;
	v_break_cur= v_accel* dt*(v_break_ms+1)/t_break;
	while(FlagWork)
	{
		if (v_accel_cur >= v_break_cur)
		{
			
			pidW->v_comm[v_break_ms+v_accel_ms].step=(v_break_cur+v_break_last)*dt*pidW->microStep/120000;
			if (pidW->v_comm[v_break_ms+v_accel_ms].step==0)
			{
				pidW->v_comm[v_break_ms+v_accel_ms].step=1;
			}
			pidW->v_comm[v_break_ms+v_accel_ms].state=v_down;
			v_avg=(v_break_cur+v_break_last)/2;
			v_break_ms++;
			v_break_last=v_break_cur;
			v_break_cur= v_accel* dt*(v_break_ms+1)/t_break;
			//stepV= stepV*;
			//stepV=stepV*pidW->microStep/120000;
			//pidW->stepV[i]=stepV;
			
		}
		else
		{
			pidW->v_comm[v_break_ms+v_accel_ms].step=(v_accel_cur+v_accel_last)*dt*pidW->microStep/120000;
			if (pidW->v_comm[v_break_ms+v_accel_ms].step==0)
			{
				pidW->v_comm[v_break_ms+v_accel_ms].step=1;
			}
			pidW->v_comm[v_break_ms+v_accel_ms].state=v_up;
			v_avg=(v_accel_cur+v_accel_last)/2;
			v_accel_ms++;
			
			if (v_accel_cur< v_accel)
			{			
				v_accel_last=v_accel_cur;
				v_accel_cur= v_accel* dt*(v_accel_ms+1)/t_accel;
			}
			else
			{
				if (v_accel_last == v_accel_cur)
				{
					FlagWork=0;
					//break;
				}
				else
				{
					v_accel_last=v_accel_cur;
				}
			}
		}
		// Защита от 0
		if (v_avg==0)
		{
			v_avg=1;
		}
		// расчет предделителя
		
		divBin=0;
		//f_temp= (pidW->f_tact*30)/(v_avg*pidW->microStep);
		f_temp= (pidW->f_tact*30);
		v_avg= v_avg*(uint32_t)pidW->microStep;
		f_temp /=v_avg;
		
		while(f_temp>0xFFFF)
		{
			f_temp/=2;
			divBin++;
		}
		pidW->v_comm[v_break_ms+v_accel_ms-1].autoreload =f_temp;
		pidW->v_comm[v_break_ms+v_accel_ms-1].divBin=divBin;
	}
	pidW->v_comm_size=v_break_ms+v_accel_ms;
}
// последние данные таймера 
Tim2_InitSet_t ret;
// расчет коэффициентов для таймера
/*
Tim2_InitSet_t CalcTim2OfOborot(uint32_t f_tact, // частота тактирования таймера
													uint16_t microStep, // кол-во микрошагов в обороте
													uint16_t w_min // скорость в об/минуту
													) 
{
	uint32_t f_temp;
	uint8_t div_b=0;
	
	if (w_min==0)
	{
		TIM2_Cmd(DISABLE);
		//ret.cnt=0xFFFF;
		//ret.prescaler=0x0F;
		return ret;
	}
	// расчет предделителя
	f_temp= (f_tact*30)/(w_min*microStep);
	while(f_temp>0xFFFF)
	{
		f_temp/=2;
		div_b++;
	}
	ret.cnt=f_temp;
	ret.prescaler=div_b;
	
	TIM2_TimeBaseInit(ret.prescaler, ret.cnt);
	TIM2_Cmd(ENABLE);
	
	
	return ret;
	
}
*/

// настройка блока расчета скорости движения
void PID_Calc_Init(pidW_t *pidW,		// указатель на ПИД
							int32_t stepPV, // текущее значение
							int32_t stepSP,	// задание первоначальное
							float Kp,				// коэффициент пропорциональности
							float Ki,				// коэффициент пропорциональности
							float Kd,				// коэффициент пропорциональности
							float wOP,      // текущие обороты в минуту
							//float wMin,      // максимальные обороты в минуту	
							float wMax      // максимальные обороты в минуту								
							)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		pidW->e[i]=0;
	}
	pidW->stepSP=stepSP;
	pidW->stepPV=stepPV;
	pidW->Kp=Kp;
	pidW->Ki=Ki;
	pidW->Kd=Kd;
	pidW->wMax=wMax;
//	pidW->wMin=wMin;
	pidW->wOP=wOP;
	
}
// настройка блока тактирования двигателя
void PID_Tact_Init(pidW_t *pidW,		// указатель на ПИД
									uint32_t f_tact, // частота тактирования таймера
									uint16_t microStep, // кол-во тактов на оборот
									GPIO_TypeDef* Port_Clk,
									GPIO_Pin_TypeDef Pin_Clk,
									GPIO_TypeDef* Port_Dir,
									GPIO_Pin_TypeDef Pin_Dir
									)
{
	GPIO_Init(Port_Clk, Pin_Clk, GPIO_MODE_OUT_PP_LOW_FAST);
	//GPIO_Init(pidW->Port_Dir,  pidW->Pin_Dir, GPIO_MODE_OUT_PP_LOW_FAST);
	if (pidW->stepDir==stepUp)
 {
	 GPIO_Init(Port_Clk, Pin_Clk, GPIO_MODE_OUT_PP_HIGH_FAST);
 }
 else
 {
	 GPIO_Init(Port_Dir,  Pin_Dir, GPIO_MODE_OUT_PP_LOW_FAST);
 }
	
	pidW->f_tact=f_tact;
	pidW->Port_Clk=Port_Clk;
	pidW->Pin_Clk=Pin_Clk;
	pidW->Port_Dir=Port_Dir;
	pidW->Pin_Dir=Pin_Dir;
	pidW->stepTI=0;
	pidW->microStep=microStep;
}


// Блок расчета значений

void PID_calc(pidW_t *pidW, int32_t stepSP)
{
	int32_t e_temp;
	uint8_t i;
	
	pidW->stepSP=stepSP;
	// сдвиг ошибки
	for(i=0;i<2;i++)
	{
		pidW->e[i]=pidW->e[i+1];
	}
	// расчет последней ошибки
	pidW->e[2]=pidW->stepSP - pidW->stepPV;
	// вычисление выходного сигнала
	pidW->wOP +=
							pidW->Kp*(pidW->e[2] - pidW->e[1])+
							pidW->Ki* pidW->e[2] +
							pidW->Kd*(pidW->e[2] - 2*pidW->e[1] + pidW->e[0]);
	
	

	if (pidW->wOP < -pidW->wMax)
	{
		
		pidW->wOP=-pidW->wMax;
	}
	else
	if (pidW->wMax<pidW->wOP)
	{
		pidW->wOP=pidW->wMax;
	}
}
// ----------
// функция расчета частоты вращения
// Перевод из OP в тактирование таймера

PID_Tact_Timer2_t PID_Tact(pidW_t *pidW ) 
{
	uint32_t f_temp;
	uint8_t div_b=0;
	uint16_t wOP_abs;
	stepDir_t dir;
	PID_Tact_Timer2_t ret;
	
	//wOP_abs=(int32_t)fabs(pidW->wOP);
	// удалил fabs
	wOP_abs=(int32_t)(pidW->wOP);
	/*
	if (pidW->wOP>0)
	{
		wOP_abs=(int32_t)pidW->wOP;
	}
	else
	{
		wOP_abs=(int32_t)(pidW->wOP*-1);
	}
	*/
	if (pidW->wOP>=0)
	{
		dir=stepUp;
	}
	else
	{
		dir=stepDown;
	}
	
	if (wOP_abs ==0 )
	{
		pidW->EnableTimer=DISABLE;
		ret.EnableTimer=DISABLE;
		ret.cnt=pidW->cnt;
		ret.prescaler=pidW->prescaler;
		
		//TIM2_Cmd(DISABLE);
		//ret.cnt=0xFFFF;
		//ret.prescaler=0x0F;
		return ret;
	}
	// расчет предделителя
	f_temp= (pidW->f_tact*30);
	f_temp=f_temp	/((uint32_t)wOP_abs*(uint32_t)pidW->microStep);
	while(f_temp>0xFFFF)
	{
		f_temp/=2;
		div_b++;
	}
	//disableInterrupts();
	pidW->cnt=f_temp;
	pidW->prescaler=div_b;
	pidW->EnableTimer=ENABLE;
	pidW->stepDir  =dir;
	//enableInterrupts();
	
	ret.cnt=f_temp;
	ret.prescaler=div_b;
	ret.EnableTimer=ENABLE;
	
	
	//TIM2_TimeBaseInit(ret.prescaler, ret.cnt);
	//TIM2_Cmd(ENABLE);
	
	
	return ret;
	
}


void clockStep(void)
{
	//pidW.stepPV++;
	PID_stepGpio(&pidW );
	
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}



#ifdef  __OSA__
int32_t k;
PID_Tact_Timer2_t setTim2;

void Task_2(void)
{
	PID_Calc_Init(&pidW,		// указатель на ПИД
							10000,	// текущее значение
							0, // задание первоначальное
							0.1,				// коэффициент пропорциональности
							0.0001,				// коэффициент интегрирования
							0,				// коэффициент дифференцирования	
							0, // текущие обороты в минуту
							//-1000,   // минимальные обороты в минуту
							1000   // максимальные обороты в минуту
							);
							
	PID_Tact_Init(&pidW,		// указатель на ПИД
									16000000, // частота тактирования таймера
									800, // кол-во тактов на оборот
									clkStep1,
									dirStep1
									);
	PID_initTab(&pidW,
									20, // Шаг интергирования ms 
									3000, // время раскрутки
									200, // время торможения
									1000 // скорость вращения
									);
	PID_TabCalc(&pidW , 
								0, // текущее значение 
								1773  // задание
								);
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	
	PID_StartAuto(&pidW);
	while(1)
	{
		nop();
	}
	//TIM2_TimeBaseInit(setTim2.prescaler, s);
	while(1)
	{
		PID_calc(&pidW, 100);
		// расчет оборотов
		setTim2=PID_Tact(&pidW);
		
		if (setTim2.EnableTimer==ENABLE)
		{
			disableInterrupts();
			TIM2_TimeBaseInit(setTim2.prescaler, setTim2.cnt);
			TIM2_Cmd(ENABLE);
			enableInterrupts();
		}
		else
		{
		//	disableInterrupts();
			TIM2_Cmd(DISABLE);
		//	enableInterrupts();
		}
		if (pidW.stepDir==stepUp)
		{
			nop();
		}
		if (pidW.stepDir==stepDown)
		{
			nop();
		}
		
		if (pidW.EnableTimer==DISABLE)
		{
			nop();
		}
		if (pidW.stepPV>5000)
		{
			nop();
		}
		if (pidW.stepPV<110)
		{
			nop();
		}
		// расчет настройки таймера 


		//
		//
		//
	/*
	if (pidW.stepSP==0)
	{
		k=-20;
	}
	if (pidW.stepSP==-100)
	{
		k=+20;
	}
	*/
	//pidW.stepSP+=k;

	
	//PID_calc(&pidW);	
	
	OS_Delay(20);
	}
}




void Task(void)
{
	uint8_t i,temp;
	
	/*
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
	*/
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

//Tim2_InitSet_t tim2_Set;


void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(1000); //Настраиваем прерывание 500мкс
//	OS_Task_Create(7, Task); // создаем задачу
	OS_Task_Create(7, Task_2); // создаем задачу
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
