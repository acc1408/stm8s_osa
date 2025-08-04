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
#define enStep1  	GPIOF, GPIO_PIN_5
#define clkStep1 	GPIOH, GPIO_PIN_2
#define dirStep1 	GPIOF, GPIO_PIN_7
#define Buzz 	  	GPIOD, GPIO_PIN_2
// 
#define karetUp 	  GPIOG, GPIO_PIN_5

#define karetDown 	  GPIOI, GPIO_PIN_5

#define karetStart GPIOI, GPIO_PIN_3
#define karetKalib GPIOI, GPIO_PIN_4


#define tabSize 250

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


typedef enum
{
	stepUp,
	stepDown
}stepDir_t;

typedef enum
{
	ClkNextRise=0, // Шаг тактирования
	ClkNextFall=1 // шаг ожидания
}stepTitTak_t;


typedef struct
{
	FunctionalState EnableTimer; // включение/отключение таймера
	uint16_t cnt;
	TIM2_Prescaler_TypeDef prescaler;
}PID_Tact_Timer2_t;



typedef enum
{
	gen_idle=0,
	gen_lastStep,
	gen_auto,
	gen_stop,
	gen_man
	
}state_gen_t;

// режим вращения вала
typedef enum
{
	rot_stop=0,
	rot_up,
	rot_max,
	rot_down
}mode_rotation_t;
// режим включения
typedef enum
{
	power_off =0,
	power_man,
	power_auto
}mode_power_t;


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
	// настройка скорости тактирования
	uint16_t autoreload;			//  автозагрузка таймера
	uint8_t divBin:4;   // Делитель таймера
	v_state_t state:1; 	// тип взаимодействия // ускорение торможение
	uint16_t step;			//  кол-во шагов
	uint8_t v_up_offset;	// смещение при ускорении
	uint8_t v_down_offset;	// смещение при торможении
}v_comm_t;



typedef enum
{
	noContact,	// контакт не замкнут
	Contact	// контакт замкнут
}endpointFlag_t;


typedef struct
{
	// Настройка ПИД-регулятора
	//int32_t stepPV; // текущее значение
	//int32_t stepSP;	// задание
	//-------------------------------
	// Расчет частоты тактирования скорости
	uint32_t f_tact; // частота тактирования таймера
	uint16_t microStep; // кол-во микрошагов в обороте
	
	
	GPIO_TypeDef* Port_Clk; // Пин 
	GPIO_Pin_TypeDef Pin_Clk;
	GPIO_TypeDef* Port_Dir;
	GPIO_Pin_TypeDef Pin_Dir;
	//------------------
	// Таблица 
	uint16_t t_accel; // время ускорения /торможения для максимальной скорости
	uint16_t t_break; // время ускорения /торможения для максимальной скорости
	uint16_t v_accel; // время ускорения /торможения для максимальной скорости
	uint8_t dt; 			// период расчета таблицы в ms
	
	v_comm_t v_comm[tabSize]; // общая таблица ускорения/ торможения
	uint16_t v_comm_size; // размер расчитанной таблицы
	//------------------
	state_gen_t state_gen; // состояние генерирования сигнала
 
	stepDir_t Dir; // Направление движения
	
	//------------
	int32_t 				PV;
	int32_t 				SP;
	uint32_t 				dSP;
	
	
	uint16_t 				max_V;			// Номер уровня для включения точного шага
	uint32_t 				max_Step; 		// Кол-во тактов для включения точного шага

	uint16_t				accel_V0;	// Номер первого уровня ускорения 
	uint16_t				accel_V0_man;	// Номер первого уровня ускорения 
	uint16_t 				cur_V;			// номер уровня в таблице
	uint32_t				cur_Step;			// кол-во отработанных шагов 
	stepTitTak_t		clk;				// Такт CLK
	endpointFlag_t flagUp;			// концевик максимального перемещения
	endpointFlag_t flagDown;	// концевик минимального перемещения
	
	//uint16_t				accel_V; 	// Номер максимального шага для ускорения
	//uint16_t				break_V;	// Номер максимального шага для торможения
	//---------------------------------
}pidW_t;
// структура 
pidW_t pidW;

#define no_V 0xFFFF


void PID_stepGpio(pidW_t *pidW )
{
	
	if (pidW ->clk == ClkNextRise)
	{
		/*
			if (pidW->Dir == stepUp)
			{
				pidW->PV++;
			}
			*/
				
			if (pidW ->state_gen == gen_auto
					|| pidW->state_gen  == gen_man
			)
			{
				GPIO_WriteHigh(pidW->Port_Clk, pidW->Pin_Clk);
				// Фиксация изменения координаты
				if (pidW->Dir == stepUp)
				{
					pidW->PV++;
				}
				else
				{
					pidW->PV--;
				}
				
				// переход из трека торможения на ускорение
				if (pidW -> v_comm[pidW -> cur_V].state == v_down
				&&  pidW->state_gen  == gen_man	)
				{
					pidW -> cur_Step=0;
						pidW -> cur_V=pidW -> cur_V + pidW -> v_comm[pidW -> cur_V].v_up_offset;
						TIM2_TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
																	pidW->v_comm[pidW -> cur_V].autoreload);
				}
				else
				// проверка на максимальную скорость
				if (pidW -> cur_V == pidW ->max_V)
				{
					if (pidW -> cur_Step  <pidW ->max_Step-1
					|| pidW -> state_gen == gen_man		) //  ручной режим
					
					{
						pidW -> cur_Step ++;
					}
					else
					{
						pidW -> cur_Step=0;
						if (pidW -> v_comm[pidW -> cur_V].v_down_offset )
						{
							pidW -> cur_V=pidW -> cur_V - pidW -> v_comm[pidW -> cur_V].v_down_offset;
							TIM2_TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
																		pidW->v_comm[pidW -> cur_V].autoreload);
							 
						}
						else
						{
							pidW -> state_gen=gen_lastStep;
						}
					}
				}
				else
				{
					if (pidW -> v_comm[pidW -> cur_V].state == v_up)
					{
						// трек ускорения
						if (pidW -> cur_Step  < pidW -> v_comm[pidW -> cur_V].step-1)
						{
							pidW -> cur_Step ++;
						}
						else
						{
							pidW -> cur_Step=0;
							pidW -> cur_V=pidW -> cur_V + pidW -> v_comm[pidW -> cur_V].v_up_offset;
							TIM2_TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
																		pidW->v_comm[pidW -> cur_V].autoreload);
						}
						
					}
					else
					{
						stop_break:
						// трек торможения
						if (pidW -> cur_Step  < pidW -> v_comm[pidW -> cur_V].step-1)
						{
							pidW -> cur_Step ++;
						}
						else
						{
							stop_accel:
							pidW -> cur_Step=0;
							if (pidW -> v_comm[pidW -> cur_V].v_down_offset)
							{
							pidW -> cur_V=pidW -> cur_V - pidW -> v_comm[pidW -> cur_V].v_down_offset;
							TIM2_TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
																		pidW->v_comm[pidW -> cur_V].autoreload);
							}
							else
							{
								pidW -> state_gen=gen_lastStep;
							}
						}
					}
				}
			}
			else
			if (pidW ->state_gen == gen_stop)
			{
				GPIO_WriteHigh(pidW->Port_Clk, pidW->Pin_Clk);
				// Фиксация изменения координаты
				if (pidW->Dir == stepUp)
				{
					pidW->PV++;
				}
				else
				{
					pidW->PV--;
				}
				
				if (pidW -> v_comm[pidW -> cur_V].state == v_up)
				{
					goto stop_accel;
				}
				else
				{
					goto stop_break;
				}
			}
			pidW ->clk = ClkNextFall;
	}
	else
	{
		
		if (pidW -> state_gen == gen_lastStep)
		{
			TIM2_Cmd(DISABLE);
			pidW -> state_gen =gen_idle;
		}
		GPIO_WriteLow(pidW->Port_Clk, pidW->Pin_Clk);
		pidW ->clk = ClkNextRise;
	}
}

void PID_stop(pidW_t *pidW)
{
	if (pidW->state_gen == gen_auto
			|| pidW->state_gen  == gen_man	)
	{
		pidW->state_gen  = gen_stop;
	}
}

void PID_man(pidW_t *pidW, stepDir_t dir)
{
	switch(pidW->state_gen)
	{
		case gen_idle:
			pidW->state_gen  = gen_man;
			if (dir ==stepUp )
			{
			pidW->Dir=stepUp;
			GPIO_WriteHigh(pidW->Port_Dir, pidW->Pin_Dir);
			}
			else
			{
				pidW->Dir = stepDown;
				GPIO_WriteLow(pidW->Port_Dir, pidW->Pin_Dir);
			}
			pidW->accel_V0=pidW->accel_V0_man;
			pidW ->cur_V=pidW ->accel_V0;
			pidW->	max_V=pidW->v_comm_size-1;
			TIM2_Cmd(ENABLE);
			break;
			
		case gen_auto:
			pidW->state_gen  = gen_stop;
			break;
			
		case gen_stop:
		{
			if (pidW->Dir ==dir)
			{
				//pidW->state_gen  = gen_stop;
				pidW->state_gen  = gen_man;
			}
			else
			{
				pidW->state_gen  = gen_stop;
			}
			
		}
		case gen_man:
			if (pidW->Dir !=dir)
			{
				pidW->state_gen  = gen_stop;
				//pidW->state_gen  = gen_man;
			}
			break;
	}
}


void PID_start(pidW_t *pidW)
{
	if (pidW->state_gen  !=gen_idle)
	{
		return;
	}
	if (pidW->SP != pidW->PV)
	{
		pidW ->state_gen = gen_auto;
		pidW ->cur_V=pidW ->accel_V0;
		TIM2_TimeBaseInit(pidW->v_comm[pidW->accel_V0].divBin, 
																		pidW->v_comm[pidW->accel_V0].autoreload);
		
		TIM2_Cmd(ENABLE);
	}
}

void PID_TabCalc2(pidW_t *pidW, 
								int32_t SP ) // задание
{
	uint32_t dSP,S;
	uint16_t i;
	//uint16_t break_V0;
	// защита от перерасчета во время работы
	if (pidW->state_gen  !=gen_idle)
	{
		return;
	}
	if (SP == pidW->PV)
	{
		return;
	}
	
	pidW->SP=SP;
	
	
	if (SP > pidW->PV)
	{
		dSP=SP - pidW->PV;
		pidW->Dir=stepUp;
		GPIO_WriteHigh(pidW->Port_Dir, pidW->Pin_Dir);
		
	}
	else
	{
		dSP = pidW->PV-SP;
		pidW->Dir = stepDown;
		GPIO_WriteLow(pidW->Port_Dir, pidW->Pin_Dir);
	}

	pidW->max_V=no_V;
	pidW->accel_V0=no_V;
	S=0;
	for(i=0;i<pidW->v_comm_size;i++)
	{
		S+=pidW->v_comm[i].step;
		if (S>=dSP || i==pidW->v_comm_size-1)
		{
			// оконачание подсчета
			if (pidW->accel_V0 !=no_V)
			{
				// существует точка начала движения с трека ускорения
				if (pidW->v_comm[i].state == v_down)
				{
						do
						{
							S-=pidW->v_comm[i].step;
							i--;
						}while(pidW->v_comm[i].state == v_down );	
				}
			}
			else
			{
				// начинаем с последней точки 
				pidW->accel_V0=i;
			}
			S-=pidW->v_comm[i].step;
			pidW->max_Step=dSP-S;
			pidW->max_V=i;
			
			break;
		}
		else
		{
			// еще идет подсчет шагов
			// определяем начальную точку запуска
			if (pidW->accel_V0 == no_V)
			{
				if (pidW->v_comm[i].state == v_up)
				{
					pidW->accel_V0 =i;
					
				}
				
			}
		}
		
		
	}
	// запуск двигателя
	pidW ->state_gen = gen_auto;
	pidW ->cur_V=pidW ->accel_V0;
	TIM2_TimeBaseInit(pidW->v_comm[pidW->accel_V0].divBin, 
																	pidW->v_comm[pidW->accel_V0].autoreload);
	
	TIM2_Cmd(ENABLE);
}

//------------------------------------
void PID_initTab(pidW_t *pidW,
									int32_t PV, // текущее значение PV
									uint32_t f_tact,
									uint16_t microStep,
									GPIO_TypeDef* Port_Clk, // Пин 
									GPIO_Pin_TypeDef Pin_Clk,
									GPIO_TypeDef* Port_Dir,
									GPIO_Pin_TypeDef Pin_Dir,
									uint8_t dt,
									uint16_t t_accel,
									uint16_t t_break,
									uint16_t v_accel,
									uint8_t Last_break_V,		// кол-во последних уровней для добавления шагов
									uint8_t Last_break_Step	// кол-во дополнительных шагов 
									)
{
	uint16_t i=0, // текущий номер в записи  
						j,
						k,
						v,
						v_accel_ind=0xFFFF, // индекс ускорения последнего
						v_break_ind=0xFFFF // индекс торможения последнего
							;
	uint8_t divBin; // предделитель таймера бинарный
	uint8_t FlagWork=1; // Выход из цикла
	uint32_t stepV, v_last=0,v_cur=0,  
	f_temp; // предделитель
	uint32_t v_avg;
	
	
	uint32_t  v_accel_num=0, 	v_break_num=0; // кол-во записей ускорения и торможения
	uint32_t 	v_accel_cur=0, 	v_break_cur=0, // скорость текущей скорости
						v_accel_last=0, v_break_last=0;// скорость предыдущая
	pidW->dt=dt;
	pidW->PV=PV;
	pidW->t_accel=t_accel;
	pidW->t_break=t_break;
	pidW->v_accel=v_accel;
	pidW->f_tact=f_tact;
	pidW->microStep=microStep;
	pidW->accel_V0=no_V;
	pidW->Port_Clk=Port_Clk;
	pidW->Pin_Clk=Pin_Clk;
	pidW->Port_Dir=Port_Dir;
	pidW->Pin_Dir=Pin_Dir;
	pidW->clk=ClkNextRise;
	GPIO_Init(Port_Clk, Pin_Clk, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(Port_Dir, Pin_Dir, GPIO_MODE_OUT_PP_LOW_FAST);
	
	// расчет общей таблицы
	i=0;
	v_accel_cur= v_accel* dt*(v_accel_num+1)/t_accel;
	v_break_cur= v_accel* dt*(v_break_num+1)/t_break;
	
	while(FlagWork)
	{
		if (v_accel_cur >= v_break_cur)
		{
			// расчет торможения
			stepV=(v_break_cur+v_break_last)*dt*pidW->microStep/120000;
			// минимум должен быть один шаг
			if (stepV)
			{
				pidW->v_comm[i].step=stepV;
			}
			else
			{
				pidW->v_comm[i].step=1;
			}
			
			pidW->v_comm[i].state=v_down;
			v_avg=(v_break_cur+v_break_last)/2;
			if (v_avg==0)
			{
				v_avg=1;
			}
			
			
			pidW->v_comm[i].v_up_offset=0;
			pidW->v_comm[i].v_down_offset=0;
			if (v_break_num)
			{
				pidW->v_comm[i].v_down_offset=i-v_break_ind;
			}
			
			if (v_break_num<Last_break_V)
			{
				pidW->v_comm[i].step+=Last_break_Step;
			}
			
			
			
			v_break_ind=i;
			// расчет скорости следующей точки
			v_break_num++;
			v_break_last=v_break_cur;
			v_break_cur= v_accel* dt*(v_break_num+1)/t_break;
		}
		else
		{
			
			// расчет ускорения
			stepV=(v_accel_cur+v_accel_last)*dt*pidW->microStep/120000;
			if (stepV)
			{
				pidW->v_comm[i].step=stepV;
			}
			else
			{
				pidW->v_comm[i].step=1;
			}
			
			pidW->v_comm[i].state=v_up;
			v_avg=(v_accel_cur+v_accel_last)/2;
			
			// расчет относительного перехода
			pidW->v_comm[i].v_up_offset=0;
			if (v_accel_num)
			{
				pidW->v_comm[v_accel_ind].v_up_offset=i-v_accel_ind;
			}
			else
			{
				// первая точка для ручной подачи
				pidW->accel_V0_man=i;
			}
			
			
			if (v_break_num)
			{
				pidW->v_comm[i].v_down_offset=i-v_break_ind;
				// настройка ускорения с трека торможения
				j=v_break_ind;
				k=v_break_num;
				while(k>0 && pidW->v_comm[j].v_up_offset==0)
				{
					pidW->v_comm[j].v_up_offset=i-j;
					j--;
					k--;
				}
				
				
			}
			else
			{
				pidW->v_comm[i].v_down_offset=0;
			}
			
			
			
			v_accel_num++;
			// номер последнего ускорения
			v_accel_ind=i;
			
			
			if (v_accel_cur< v_accel)
			{
				v_accel_last=v_accel_cur;
				v_accel_cur= v_accel* dt*(v_accel_num+1)/t_accel;
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
		pidW->v_comm[i].autoreload =f_temp;
		pidW->v_comm[i].divBin=divBin;
		i++;// увеличиваем кол-во записей
	}
	//i++; 
	pidW->v_comm_size=i;
}
// последние данные таймера 
Tim2_InitSet_t ret;


void clockStep(void)
{
	//pidW.stepPV++;
	PID_stepGpio(&pidW );
	
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}



#ifdef  __OSA__
int32_t k;
PID_Tact_Timer2_t setTim2;

typedef enum
{
	karet_idle=0,
	karet_up=1,
	karet_down=2,
	karet_stop=3
	
} butKaret_t;

butKaret_t butKaret;

void Task_2(void)
{
	GPIO_Init(enStep1, GPIO_MODE_OUT_PP_HIGH_FAST); // Включен Step
	GPIO_Init(clkStep1, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(dirStep1, GPIO_MODE_OUT_PP_LOW_FAST);
	
	GPIO_Init(karetUp, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(karetDown, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(karetStart, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(karetKalib, GPIO_MODE_IN_PU_NO_IT);
	
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	
	//GPIO_Init(dirStep1, GPIO_MODE_OUT_PP_HIGH_FAST);
	PID_initTab(&pidW,
									0, // текущее значение PV
									16000000, // частота
									800, // микрошагов
									clkStep1,
									dirStep1,
									20, // Шаг дифференцирования ms 
									2000, // время раскрутки
									1500, // время торможения
									1000, // скорость вращения
									5, // кол-во последних уровней для добавления шагов
									10 // кол-во шагов перед остановкой
									);
	/*
	PID_TabCalc2(&pidW , 			
								-800 // задание)
								); 
	PID_start(&pidW);
	*/
	
	
	/*
	PID_TabCalc(&pidW , 
								0000, // текущее значение 
								16000  // задание
								);
	*/
	
	//PID_StartAuto(&pidW);
	//-------------
	
	/*
	while(	pidW.state_gen  != gen_stop)
	{
		if (pidW.PV==2000)
		{
			
			pidW.state_gen  = gen_stop;
		}
	}
	*/
	while(	1)
	{
		
		if ( !GPIO_ReadInputPin(karetStart) )
		{
			PID_TabCalc2(&pidW, 
								pidW.PV+800 ); // задание
			//PID_stop(&pidW);
		}
		// ручное перемещение
		butKaret=karet_idle;		
		if ( GPIO_ReadInputPin(karetUp) )
		{
			nop();
			//PID_stop(&pidW);
		}
		else
		{
			nop();
			butKaret|=karet_up;
			//PID_man(&pidW, stepUp);
		}
		if ( GPIO_ReadInputPin(karetDown) )
		{
			nop();
			//PID_stop(&pidW);
		}
		else
		{
			nop();
			butKaret|=karet_down;
			//PID_man(&pidW, stepDown);
		}
		
		if (pidW.state_gen == gen_idle
		|| pidW.state_gen == gen_man 	)
		{
			switch(butKaret)
			{
				case karet_idle:	
				case karet_stop:
					PID_stop(&pidW);
					break;
				
					break;
				case karet_up:
					PID_man(&pidW, stepUp);
					break;
				case karet_down:
					PID_man(&pidW, stepDown);
					break;
					
				
			}
		}
		else
		if (pidW.state_gen == gen_auto)
		{
			switch(butKaret)
			{
				case karet_idle:
						break;
				case karet_stop:
				case karet_up:
				case karet_down:
					PID_stop(&pidW);
					break;
			}
		}
	}
	//TIM2_TimeBaseInit(setTim2.prescaler, s);
	while(1)
	{
		
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
