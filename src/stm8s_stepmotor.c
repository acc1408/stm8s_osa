/**
  ******************************************************************************
  * @file    stm8s_adc2.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the functions/macros for the ADC2 peripheral.
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
#include "inc/stm8s_stepmotor.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/


//------------------------------------
// настройка портов
void PID_Init_Gpio(	pidW_t *pidW,
										// настройка таймеров
										timer_TimeBaseInit_t TimeBaseInit, // настройка предделителей таймера
										timer_cmd_t timer_cmd,// настройка включения/отключения таймера
										
										GPIO_TypeDef* Port_Clk, // Пин 
										GPIO_Pin_TypeDef Pin_Clk,
										GPIO_TypeDef* Port_Dir,
										GPIO_Pin_TypeDef Pin_Dir,	
										GPIO_TypeDef* Port_En, // Пин 
										GPIO_Pin_TypeDef Pin_En,
										
										limitSW_Level_t limitSW_Up_Level,
										GPIO_TypeDef* Port_limitSW_Up,
										GPIO_Pin_TypeDef Pin_limitSW_Up,
										
										limitSW_Level_t limitSW_Down_Level,
										GPIO_TypeDef* Port_limitSW_Down,
										GPIO_Pin_TypeDef Pin_limitSW_Down
									)
{
	// отключаем драйвер

	GPIO_Init(Port_En, Pin_En, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(Port_Dir, Pin_Dir, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(Port_Clk, Pin_Clk, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_WriteHigh(Port_En, Pin_En);	
	GPIO_Init(Port_limitSW_Up,   Pin_limitSW_Up,   GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(Port_limitSW_Down, Pin_limitSW_Down, GPIO_MODE_IN_PU_NO_IT);
	
//	GPIO_Init(Port_Calib, Pin_Calib, GPIO_MODE_IN_PU_NO_IT);
	
	pidW->TimeBaseInit=TimeBaseInit;
	pidW->timer_cmd=timer_cmd;
	
	pidW->Port_Clk=Port_Clk;
	pidW->Pin_Clk=Pin_Clk;
	
	pidW->Port_Dir=Port_Dir;
	pidW->Pin_Dir=Pin_Dir;
	
	pidW->Port_En=Port_En;
	pidW->Pin_En=Pin_En;
	
	
	pidW->Port_limitSW_Up=Port_limitSW_Up;
	pidW->Pin_limitSW_Up=Pin_limitSW_Up;
	
	pidW->Port_limitSW_Down=Port_limitSW_Down;
	pidW->Pin_limitSW_Down=Pin_limitSW_Down;
	
	pidW->limitSW_Up_Level=limitSW_Up_Level;
	pidW->limitSW_Down_Level=limitSW_Down_Level;

}

uint16_t PID_initTab(pidW_t *pidW,
									int32_t PV, // текущее значение PV
								
									uint32_t f_tact,
									uint16_t microStep,									
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
	
	pidW->clk=ClkNextRise;
	
	
	// расчет общей таблицы
	i=0;
	v_accel_cur= v_accel* dt*(v_accel_num+1)/t_accel;
	v_break_cur= v_accel* dt*(v_break_num+1)/t_break;
	
	while(FlagWork)
	{
		if (i>=tabSize)
		{
			// превышение таблицы данных по разгону и ускорению
			while(1);
		}
			
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
	return i;
}

/*
void PID_calibStartOne(pidW_t *pidW)
{
	pidW->PV_calibFlag=PV_calibStartOne;
}

void PID_calibOne(pidW_t *pidW)
{
	pidW->PV_calibFlag=PV_calibOne;
}

PV_calibFlag_t PID_getCalibState(pidW_t *pidW)
{
	return pidW->PV_calibFlag;
}

void PID_setFlagCalib(pidW_t *pidW)
{
	pidW->PV_calibFlag=1;
}

*/
void PID_setPV(pidW_t *pidW,
									int32_t PV)
{
	pidW->PV=PV;
}

int32_t PID_getPV(pidW_t *pidW)
{
	return pidW->PV;
}

void PID_limitSwitch_Up(pidW_t *pidW, FunctionalState State)
{
	if (State)
	{
		pidW ->stopUp=flag_rot_stop_on;
	}else
	{
		pidW ->stopUp=flag_rot_stop_off;
	}
}

void PID_limitSwitch_Down(pidW_t *pidW, FunctionalState State)
{
	if (State)
	{
		pidW ->stopDown=flag_rot_stop_on;
	}else
	{
		pidW ->stopDown=flag_rot_stop_off;
	}
}
void PID_limitSW_test(pidW_t *pidW)
{
	
		switch(pidW->limitSW_Up_Level)
		{
			case limitSW_Level_Low:
				if ( !GPIO_ReadInputPin(pidW->Port_limitSW_Up  , pidW->Pin_limitSW_Up  ))  
				{
					pidW->stopUp=flag_rot_stop_on;
				}
				else
				{
					pidW->stopUp=flag_rot_stop_off;
				}
				break;
			case limitSW_Level_High:
				if ( GPIO_ReadInputPin(pidW->Port_limitSW_Up  , pidW->Pin_limitSW_Up ))  
				{
					pidW->stopUp=flag_rot_stop_on;
				}
				else
				{
					pidW->stopUp=flag_rot_stop_off;
				}
				break;
		}
		//----------------------
			switch(pidW->limitSW_Down_Level)
			{
				case limitSW_Level_Low:
					if ( !GPIO_ReadInputPin(pidW->Port_limitSW_Down  , pidW->Pin_limitSW_Down  ))  
					{
						pidW->stopDown=flag_rot_stop_on;
					}
					else
					{
						pidW->stopDown=flag_rot_stop_off;
					}
					break;
				case limitSW_Level_High:
					if ( GPIO_ReadInputPin(pidW->Port_limitSW_Down  , pidW->Pin_limitSW_Down ))  
					{
						pidW->stopDown=flag_rot_stop_on;
					}
					else
					{
						pidW->stopDown=flag_rot_stop_off;
					}
					break;
			}
}

mode_rotation_t PID_getStateRot(pidW_t *pidW)
{
	return pidW->rot;
}
// Режим включения
mode_power_t PID_getStatePower(pidW_t *pidW)
{
	return pidW->power;
}

stepDir_t PID_getStateDir(pidW_t *pidW)
{
	return pidW->Dir;
}
limitSW_Level_t PID_getCalibState(pidW_t *pidW)
{
	return pidW->PV_calibLevel;
}

void PID_stepGpio(pidW_t *pidW )
{
	
	if (pidW ->clk == ClkNextRise)
	{
		
		if (pidW -> rot == rot_stop)
		{
			if (pidW->Dir)
			{
				GPIO_WriteHigh(pidW->Port_Dir, pidW->Pin_Dir);
			}
			else
			{
				GPIO_WriteLow(pidW->Port_Dir, pidW->Pin_Dir);
			}
			// сохраняем режим запуска после остановки
			pidW -> rot = pidW -> power;
		}
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
		//----------------------
		// начала калибровки
		// флаг калибровки
		if (pidW -> PV_calibLevel != limitSW_Level_None)
		{
			switch(pidW -> PV_calibLevel)
			{
				case limitSW_Level_Low:
					if ( !GPIO_ReadInputPin(pidW ->Port_Calib, pidW ->Pin_Calib))  
					{
						lbl_stop_calib:
						pidW ->PV=pidW ->PV_calibValue;
						pidW ->PV_calibLevel=limitSW_Level_None;
						pidW->power  =  power_off;
					}
					break;
				case limitSW_Level_High:	
					if (GPIO_ReadInputPin(pidW ->Port_Calib, pidW ->Pin_Calib))  
					{
						goto lbl_stop_calib;
					}
					break;
			}
		}
		// конец передвижения калибровки
		//------------
		PID_limitSW_test(pidW);
		
		// переключение на другой фронт
		pidW ->clk = ClkNextFall;
	}
	else
	{
			GPIO_WriteLow(pidW->Port_Clk, pidW->Pin_Clk);
			//-----------------------------------
			
			// если есть флаг общего стопа
			if ( (pidW->power  ==  power_off )
			|| (pidW->power  == power_man &&  pidW -> rot == rot_auto) )
		
			{
				pidW->power  =  power_off;
				//pidW->stopCom  = flag_rot_stop_off;
				stop_rot_any:
				if (pidW -> v_comm[pidW -> cur_V].state == v_up)
					{
						goto stop_accel;
					}
					else
					{
						goto stop_break;
					}
			}
			// верхний концевик
		if (pidW->stopUp  == flag_rot_stop_on	 )
		{
			
			if (pidW->Dir == stepUp)
			{
				goto stop_rot_any;
			}
		}
		// нижний концевик
		if (pidW->stopDown  == flag_rot_stop_on	 )
		{
			if (pidW->Dir == stepDown)
			{
				goto stop_rot_any;
			}
		}
		
		// проверка на вращение
		if (pidW -> rot == rot_stop)
			{
				// если вызвали прерывание,
				// а задания нет
				nop();
				goto stop_rot_exit;
			}
		//--------------------
		
		
		
		// проверка на максимальную скорость
		if (pidW -> cur_V == pidW ->max_V)
		{
			if (pidW -> cur_Step  <pidW ->max_Step-1
			|| pidW -> power == power_man		) //  ручной режим
			
			{
				pidW -> cur_Step ++;
			}
			else
			{
				pidW -> cur_Step=0;
				if (pidW -> v_comm[pidW -> cur_V].v_down_offset )
				{
					pidW -> cur_V=pidW -> cur_V - pidW -> v_comm[pidW -> cur_V].v_down_offset;
					pidW->TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
																pidW->v_comm[pidW -> cur_V].autoreload);
					 
				}
				else
				{
					pidW -> rot=rot_stop;
				}
			}
		}
		else
		// проверка на разгон
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
				pidW->TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
															pidW->v_comm[pidW -> cur_V].autoreload);
			}
			
		}
		else
		{
			// возобновление вращения в ручном режиме
			if ( pidW->power  == power_man )
			{
				pidW -> cur_Step=0;
				while( pidW -> v_comm[++pidW -> cur_V].state == v_down);
				goto stop_rot_exit;
			}
			
			// отработка торможения
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
				pidW->TimeBaseInit(pidW->v_comm[pidW -> cur_V].divBin, 
															pidW->v_comm[pidW -> cur_V].autoreload);
				}
				else
				{
					pidW -> rot=rot_stop;
				}
			}
		}
		stop_rot_exit:
		//-----------------------------------------
		if (pidW -> rot == rot_stop)
		{
			pidW->timer_cmd(DISABLE);
			pidW -> power =power_off;
			
			//pidW->stopCom  = flag_rot_stop_off;
		}
		pidW ->clk = ClkNextRise;
	}
}

void PID_calibInit( pidW_t 						*pidW,
										GPIO_TypeDef* 		Port_Calib,
										GPIO_Pin_TypeDef 	Pin_Calib,
										limitSW_Level_t		PV_calibLevel,		
										int32_t						PV_calibValue 
										)
{
	GPIO_Init(Port_Calib, Pin_Calib, GPIO_MODE_IN_PU_NO_IT);
	pidW -> Port_Calib=Port_Calib;
	pidW -> Pin_Calib=Pin_Calib;
	pidW -> PV_calibLevel=PV_calibLevel;
	pidW -> PV_calibValue=PV_calibValue;
	
}


void PID_stop(pidW_t *pidW)
{
	pidW->power  = power_off;
}

void PID_stop_auto(pidW_t *pidW)
{
	if ( pidW->rot == rot_auto)
	{
		pidW->power  = power_off;
	}
}

void PID_stop_man(pidW_t *pidW)
{
	if ( pidW->rot == rot_man)
	{
		pidW->power  = power_off;
	}
}

void PID_stop_man_dir(pidW_t *pidW, stepDir_t dir)
{
	if ( pidW->rot == rot_man
			&& pidW->Dir == dir )
	{
		pidW->power  = power_off;
	}
}

void PID_start_man(pidW_t *pidW, stepDir_t dir)
{
	
	PID_limitSW_test(pidW);
	
	switch(pidW->rot)
	{
		case rot_stop:
			
			if (dir ==stepUp )
			{
				if (pidW ->stopUp==flag_rot_stop_off)
				{
					pidW->power  = power_man;
					pidW->Dir=stepUp;
				}
				else
				{
					pidW->power  = power_off;
					return;
				}
			}
			else
			{
				if (pidW ->stopDown==flag_rot_stop_off)
				{
					pidW->power  = power_man;
					pidW->Dir = stepDown;
				}
				else
				{
					pidW->power  = power_off;
					return;
				}
			}
			pidW->accel_V0=pidW->accel_V0_man;
			pidW ->cur_V=pidW ->accel_V0;
			pidW->	max_V=pidW->v_comm_size-1;
			pidW->timer_cmd(ENABLE);
			break;
			
		case rot_auto:
			pidW->power  = power_off;
			break;
			
		case rot_man:
			if (pidW->Dir ==dir)
			{
				//pidW->state_gen  = gen_stop;
				if (dir == stepUp  )
				{
					if (pidW->stopUp == flag_rot_stop_off)
					{
						pidW->power  = power_man;
						pidW->timer_cmd(ENABLE);
					}
					else
					{
						pidW->power  = power_off;
					}
				}
				else
				{
					if (pidW->stopDown == flag_rot_stop_off)
					{
						pidW->power  = power_man;
						pidW->timer_cmd(ENABLE);
					}
					else
					{
						pidW->power  = power_off;
					}
				}
			}
			else
			{
				pidW->power  = power_off;
			}
			break;
	}
}

void PID_start_auto(pidW_t *pidW, 
								int32_t SP ) // задание
{
	uint32_t dSP,S;
	uint16_t i;
	//uint16_t break_V0;
	// защита от перерасчета во время работы
	PID_limitSW_test(pidW);
	
	
	if (pidW->rot  != rot_stop
	|| SP == pidW->PV	
	||  pidW->power !=power_off
	)
	{
		return;
	}
	
	
	pidW->SP=SP;
	
	
	if (SP > pidW->PV)
	{
		if (pidW->stopUp == flag_rot_stop_on)
		{
			pidW->power  = power_off;
			return;
		}
		dSP=SP - pidW->PV;
		pidW->Dir=stepUp;
		GPIO_WriteHigh(pidW->Port_Dir, pidW->Pin_Dir);
	}
	else
	{
		if (pidW->stopDown == flag_rot_stop_on)
		{
			pidW->power  = power_off;
			return;
		}
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
				//значит существует точка начала движения с трека ускорения
				// далее находим ближайщую точку на треке ускорений
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
				// начинаем ускорение с последней точки торможения 
				pidW->accel_V0=i;
			}
			// расчитываем точку максимальной скорости
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
	pidW ->power = power_auto;
	pidW ->cur_V=pidW ->accel_V0;
	pidW ->TimeBaseInit(pidW->v_comm[pidW->accel_V0].divBin, 
																	pidW->v_comm[pidW->accel_V0].autoreload);
	
	pidW->timer_cmd(ENABLE);
}







/**
  * @}
  */
  
/**
  * @}
  */
  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
