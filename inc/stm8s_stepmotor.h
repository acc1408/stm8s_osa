/**
  ******************************************************************************
  * @file    stm8s_adc2.h
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the prototypes/macros for the ADC2 peripheral.
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_STEPMOTOR_H
#define __STM8S_STEPMOTOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_gpio.h"

/* Exported types ------------------------------------------------------------*/


#define no_V 0xFFFF

#define tabSize 	200 // размер таблицы для сохранения шагов разгона и торможения



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

/*
typedef enum
{
	gen_idle=0,
	gen_lastStep,
	gen_auto,
	gen_stop,
	gen_man
	
}state_gen_t;
*/


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

typedef enum
{
  TIM_PRESCALER_1  = ((uint8_t)0x00),
  TIM_PRESCALER_2    = ((uint8_t)0x01),
  TIM_PRESCALER_4    = ((uint8_t)0x02),
  TIM_PRESCALER_8     = ((uint8_t)0x03),
  TIM_PRESCALER_16   = ((uint8_t)0x04),
  TIM_PRESCALER_32     = ((uint8_t)0x05),
  TIM_PRESCALER_64    = ((uint8_t)0x06),
  TIM_PRESCALER_128   = ((uint8_t)0x07),
  TIM_PRESCALER_256   = ((uint8_t)0x08),
  TIM_PRESCALER_512   = ((uint8_t)0x09),
  TIM_PRESCALER_1024  = ((uint8_t)0x0A),
  TIM_PRESCALER_2048 = ((uint8_t)0x0B),
  TIM_PRESCALER_4096   = ((uint8_t)0x0C),
  TIM_PRESCALER_8192 = ((uint8_t)0x0D),
  TIM_PRESCALER_16384 = ((uint8_t)0x0E),
  TIM_PRESCALER_32768 = ((uint8_t)0x0F)
}TIM_Prescaler_TypeDef;

typedef struct
{
	// настройка скорости тактирования
	
	uint16_t step;			//  кол-во шагов
	v_state_t state:1; 	// тип взаимодействия // ускорение торможение
	TIM_Prescaler_TypeDef divBin:4;   // Делитель таймера
	uint16_t autoreload;			//  автозагрузка таймера
	uint8_t v_up_offset:4;	// смещение при ускорении
	uint8_t v_down_offset:4;	// смещение при торможении
}v_comm_t;


// Уровень срабатывания концевика
typedef enum
{
	limitSW_Level_None=0,
	limitSW_Level_Low,
	limitSW_Level_High
}limitSW_Level_t;

typedef enum
{
	flag_rot_stop_off=0,
	flag_rot_stop_on=1
}flag_rot_stop_t;

// режим вращения вала
typedef enum
{
	rot_stop=0,
	rot_man=1,
	rot_auto=2
}mode_rotation_t;
// режим включения
typedef enum
{
	power_off =0,
	power_man=1,
	power_auto=2
}mode_power_t;


typedef void (*timer_cmd_t)(FunctionalState NewState);
typedef void (*timer_TimeBaseInit_t)(TIM_Prescaler_TypeDef TIM_Prescaler, uint16_t TIM_Period);

typedef enum
{
	PV_calibNo=0,
	PV_calibTwo,
	PV_calibStartTwo,
	PV_calibOne,
	PV_calibStartOne
}
PV_calibFlag_t;

typedef struct
{
	//-------------------------------
	// Расчет частоты тактирования скорости
	uint32_t f_tact; // частота тактирования таймера
	uint16_t microStep; // кол-во микрошагов в обороте
	// настройка функций таймеров
	// настройка предделителей таймера
	timer_TimeBaseInit_t TimeBaseInit;
	// настройка включения/отключения таймера
	timer_cmd_t timer_cmd;
	
	GPIO_TypeDef* 		Port_Clk; // Пин 
	GPIO_Pin_TypeDef 	Pin_Clk;
	GPIO_TypeDef* 		Port_Dir;
	GPIO_Pin_TypeDef 	Pin_Dir;
	GPIO_TypeDef* 		Port_En;
	GPIO_Pin_TypeDef 	Pin_En;
	
	//-------------------
	limitSW_Level_t 	limitSW_Up_Level;
	GPIO_TypeDef* 		Port_limitSW_Up;
	GPIO_Pin_TypeDef 	Pin_limitSW_Up;
	
	limitSW_Level_t  	limitSW_Down_Level;
	GPIO_TypeDef*    	Port_limitSW_Down;
	GPIO_Pin_TypeDef 	Pin_limitSW_Down;
	
	//------------------
	// Таблица 
	uint16_t t_accel; // время ускорения /торможения для максимальной скорости
	uint16_t t_break; // время ускорения /торможения для максимальной скорости
	uint16_t v_accel; // время ускорения /торможения для максимальной скорости
	uint8_t dt; 			// период расчета таблицы в ms
	
	v_comm_t v_comm[tabSize]; // общая таблица ускорения/ торможения
	uint16_t v_comm_size; // размер расчитанной таблицы
	//------------------
	// стоп максимальный
	flag_rot_stop_t stopUp;
	// стоп минимальный
	flag_rot_stop_t stopDown;
	
	// режим включения двигателя после остановки 
	mode_rotation_t rot;
	
	// режим управления скоростями
	mode_power_t power;
 
	stepDir_t Dir; // Направление движения
	
	//-------------------
	// Калибровка
	GPIO_TypeDef* 		Port_Calib;
	GPIO_Pin_TypeDef 	Pin_Calib;
	int32_t						PV_calibValue;
	limitSW_Level_t		PV_calibLevel;
	//------------
	int32_t 				PV;
	int32_t 				SP;
//	uint32_t 				dSP;
	uint16_t 				max_V;			// Номер уровня для включения точного шага
	uint32_t 				max_Step; 		// Кол-во тактов для включения точного шага

	uint16_t				accel_V0;	// Номер первого уровня ускорения 
	uint16_t				accel_V0_man;	// Номер первого уровня ускорения при ручном ускорении
	uint16_t 				cur_V;			// номер уровня в таблице
	uint32_t				cur_Step;			// кол-во отработанных шагов 
	stepTitTak_t		clk;				// Такт CLK
	//endpointFlag_t 	flagUp;			// концевик максимального перемещения
	//endpointFlag_t 	flagDown;	// концевик минимального перемещения
	
	//uint16_t				accel_V; 	// Номер максимального шага для ускорения
	//uint16_t				break_V;	// Номер максимального шага для торможения
	//---------------------------------
}pidW_t;







//----------------


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
										//GPIO_TypeDef* 		Port_Calib,
										//GPIO_Pin_TypeDef 	Pin_Calib,
										
										limitSW_Level_t limitSW_Up_Level,
										GPIO_TypeDef* Port_limitSW_Up,
										GPIO_Pin_TypeDef Pin_limitSW_Up,
										
										limitSW_Level_t limitSW_Down_Level,
										GPIO_TypeDef* Port_limitSW_Down,
										GPIO_Pin_TypeDef Pin_limitSW_Down
									);
									
uint16_t PID_initTab(pidW_t *pidW,
									int32_t PV, // текущее значение PV
									//int32_t PV_calibValue,
									uint32_t f_tact,// частота
									uint16_t microStep,									
									uint8_t dt,
									uint16_t t_accel,
									uint16_t t_break,
									uint16_t v_accel,
									uint8_t Last_break_V,		// кол-во последних уровней для добавления шагов
									uint8_t Last_break_Step	// кол-во дополнительных шагов 
									);									
									
void PID_setPV(pidW_t *pidW,
									int32_t PV);

int32_t PID_getPV(pidW_t *pidW);

void PID_start_auto(pidW_t *pidW, 
								int32_t SP );

void PID_start_man(pidW_t *pidW, stepDir_t dir);								

void PID_stop_man_dir(pidW_t *pidW, stepDir_t dir);


void PID_stop_man(pidW_t *pidW);


void PID_stop_auto(pidW_t *pidW);


void PID_stop(pidW_t *pidW);


void PID_stepGpio(pidW_t *pidW );


stepDir_t PID_getStateDir(pidW_t *pidW);

mode_power_t PID_getStatePower(pidW_t *pidW);


mode_rotation_t PID_getStateRot(pidW_t *pidW);


void PID_limitSwitch_Down(pidW_t *pidW, FunctionalState State);

void PID_limitSwitch_Up(pidW_t *pidW, FunctionalState State);
//void PID_setFlagCalib(pidW_t *pidW);
// Первоначальная подготовка к калибровке
//void PID_calibStartOne(pidW_t *pidW);
// Запуск первого этапа
//void PID_calibOne(pidW_t *pidW);
//PV_calibFlag_t PID_getCalibState(pidW_t *pidW);

void PID_calibInit( pidW_t 						*pidW,
										GPIO_TypeDef* 		Port_Calib,
										GPIO_Pin_TypeDef 	Pin_Calib,
										limitSW_Level_t		PV_calibLevel,		
										int32_t						PV_calibValue 
										);
limitSW_Level_t PID_getCalibState(pidW_t *pidW);
//PV_calibFlag_t PID_getCalibState(pidW_t *pidW);

/** @addtogroup ADC2_Exported_Types
  * @{
  */

/**
  * @brief  ADC2 clock prescaler selection
  */


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/* Exported macros ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/


#endif /* __STM8S_STEPMOTOR_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
