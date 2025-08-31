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

// кол-во микрошагов на оборот
#define microStep1 800
#define maxDistCarSP 4000 // максимальное задание
#define ConvStepIntoDist 400 // конвертация шагов в мм
#define CalibLimitIn_mm 1000 // калибровочное расстояние

#define SrcConvStepIntoDist 400 // конвертация шагов в мм для источника
#define SrcDistDown				30		// расстояние опускания вниз для источника в верхней точке

#define alarm_btn_time_off 5 // время удержания кнопки пуск таймер для отключения
//=====================================
// описание входов шаговых двигатлей
// каретка
#define enStep1  	GPIOF, GPIO_PIN_5
#define clkStep1 	GPIOH, GPIO_PIN_2
#define dirStep1 	GPIOF, GPIO_PIN_7
//---------------------------------
// сигнал ошибки по току
#define AlmStep		GPIOA, GPIO_PIN_6
// Датчика концевиков
// концевика каретки
#define CarFarLimit 	GPIOF, GPIO_PIN_6
#define CarNearLimit 	GPIOH, GPIO_PIN_1
// Зона заслонки 1 м от источника
#define CarCalibLimit GPIOH, GPIO_PIN_3         
// Светодиод зоны заслонки
#define CarCalibLimitLed	GPIOE, GPIO_PIN_6

// Светодиоды ограничений
#define CarFarLimitLed 	GPIOE, GPIO_PIN_5
#define CarNearLimitLed	GPIOE, GPIO_PIN_7

//------------------------------------
// Кнопки
// ручной режим Дальше
#define CarFarBtn 	  GPIOG, GPIO_PIN_5
// ручной режим Ближе
#define CarNearBtn 	  GPIOI, GPIO_PIN_5
// авто старт
#define CarAutoStartBtn GPIOI, GPIO_PIN_3
// калибровка
#define CarCalibBtn GPIOI, GPIO_PIN_4
// Светодиоды кнопок
#define CarFarBtnLed	GPIOC, GPIO_PIN_4
#define CarNearBtnLed 	GPIOC, GPIO_PIN_3
#define CarAutoStartBtnLed	GPIOC, GPIO_PIN_2
#define CarCalibBtnLed	GPIOC, GPIO_PIN_1
// Настройка дистанции
#define CarAutoDistPlusBtn GPIOI, GPIO_PIN_1
#define CarAutoDistMinusBtn GPIOI, GPIO_PIN_2


//=========================================
// заслонка
#define enStep2  	GPIOH, GPIO_PIN_0
#define clkStep2 	GPIOA, GPIO_PIN_3
#define dirStep2 	GPIOA, GPIO_PIN_5
#define calibNoGpio	0,0
// Концевики для заслонки
// заслонка открыта
#define SliderOpenLimit GPIOA, GPIO_PIN_4
// заслонка закрыта
#define SliderCloseLimit 	GPIOA, GPIO_PIN_2


// Индикаторы светодиодов Концевиков
#define SliderOpenLimitLed 	GPIOH, GPIO_PIN_7
#define SliderCloseLimitLed GPIOH, GPIO_PIN_6

// Кнопки настройка Часов

#define SliderHourPlusBtn  	GPIOE, GPIO_PIN_3
#define SliderHourMinusBtn	GPIOE, GPIO_PIN_2

#define SliderMinutePlusBtn GPIOE, GPIO_PIN_1
#define SliderMinuteMinusBtn GPIOE, GPIO_PIN_0

#define SliderTimeStartBtn	GPIOE, GPIO_PIN_4

#define SliderOpenBtn				GPIOG, GPIO_PIN_7

#define SliderCloseBtn			GPIOG, GPIO_PIN_6

// Светодиоды
// Встроенный светодиоды Кнопки
#define SliderOpenBtnLed		GPIOC, GPIO_PIN_6
#define SliderCloseBtnLed		GPIOC, GPIO_PIN_5
#define SliderTimeStartBtnLed GPIOC, GPIO_PIN_7






//============================================
// Источник
#define enStepSrc_3  	GPIOA, GPIO_PIN_1
#define clkStepSrc_3 	GPIOD, GPIO_PIN_4
#define dirStepSrc_3 	GPIOD, GPIO_PIN_6
//----------------------------------------
// Концевики для источника
// источник поднят
#define SrcUpLimit 	GPIOD, GPIO_PIN_7
// источник опущен
#define SrcDownLimit 	GPIOD, GPIO_PIN_5
//----------------------------------------
// Светодиоды концевиков +
#define SrcUpLimitLed 	GPIOH, GPIO_PIN_5
// источник опущен
#define SrcDownLimitLed 	GPIOH, GPIO_PIN_4

// кнопки
// кнопка подъем источника+
#define SrcUpBtn GPIOI, GPIO_PIN_7
// опускание источника
#define SrcDownBtn GPIOI, GPIO_PIN_6
//--------------------------------------
// светодиоды кнопок
// кнопка источник поднят+
#define SrcUpBtnLed GPIOG, GPIO_PIN_1
// источник опущен
#define SrcDownBtnLed GPIOG, GPIO_PIN_0



//========================================
//--------------------------------
// пищалка двери+
#define Buzz 	  	GPIOD, GPIO_PIN_2

// Концевик двери+
#define SwDoorLimit 	GPIOD, GPIO_PIN_3
// Включение обмоток двери +
#define SwDoorCoil   	GPIOG, GPIO_PIN_3
// кнопка общей остановки
//#define StopBtn 		GPIOD, GPIO_PIN_0
#define ResetBtn		GPIOG, GPIO_PIN_4
#define ResetBtnLed GPIOC, GPIO_PIN_0
#define OnOffBtn		GPIOI, GPIO_PIN_0

#define StopBtn		GPIOD, GPIO_PIN_0


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
									//	*
//***************************************************************
// Описание пинов
#define tm1637_1	GPIOB, GPIO_PIN_3, GPIOB, GPIO_PIN_6
#define tm1637_2	GPIOB, GPIO_PIN_2, GPIOB, GPIO_PIN_6
#define tm1637_3	GPIOB, GPIO_PIN_1, GPIOB, GPIO_PIN_6
#define tm1637_4	GPIOB, GPIO_PIN_0, GPIOB, GPIO_PIN_6
//==================================================
// Пины I2C

#define GPIO_SDA	GPIOB, GPIO_PIN_5
#define GPIO_SCL	GPIOB, GPIO_PIN_4
#define maxRepeatWrite	50



// Окончание пинов
//uint8_t a[]={0x71,0x74,0x07,0x33,0x02};



#define noHandler 0

// Порог быстрой перемотки
#define edgeFastHour 10
#define edgeFastMinute 5



//------------------------------------------------
// Состояние оси подъем источника 
typedef enum
{
	SrcMoveIdle=0,
	SrcMoveUp,
	SrcMoveDown
} ControlSrc_t;

typedef enum
{
	sliderTimerStop,
	sliderTimerRun,
	//sliderTimerReset,
	sliderTimerHold,			//При достижении флага происходит сброс таймера
	sliderTimerInit
} sliderTimerState_t;

// регулятор оборотов
typedef enum
{
	flagModeKalibOff=0,
	flagModeKalibOn,
	flagModeKalibDownRun,
	flagModeKalibUpRun
}flagModeKalib_t;


typedef enum
{
	karet_idle=0,
	karet_up=1,
	karet_down=2,
	karet_stop=3
	
} butKaret_t;
// типы данных для работы с часами
typedef union
{
	uint8_t reg[4];
	struct
	{
		uint8_t s1:4;
		uint8_t s10:4;
		
		uint8_t m1:4;
		uint8_t m10:4;
		
		uint8_t h1:4;
		uint8_t h10:4;
		
		uint8_t day1_7;
	};
} ds3231_reg_t;

//-----------------------------------
// Типы данных для каретки

//----------------------------------

//=======================================
//---------------------------------
// Модуль работы экраны
//---------------------------------
// объекты экрана
tm1637_t 	disp1, 
					disp2, 
					dispTimer, 
					dispTimerAlarm ;
//---------------------------------
//массив для экранов
char st[15];
//==================================

//----------------------------------
// Структура для работы с шаговыми двигателями
//---------------------------------------------
// структура 
pidW_t 	pidCar,			// Каретка 
				pidSlider, 	// Заслонка
				pidSrc ;		// Источник
//----------------------------------
//========================================
// автовыход из зоны задвижки
typedef enum
{
	CarAutoUpOff,
	CarAutoUpOn
}CarAutoUpMode_t;
//----------------------------------------
// Кнопки управления кареткой
button_t 	Handler_CarFarBtn,
					Handler_CarNearBtn,
					Handler_CarAutoStartBtn,
					Handler_CarCalibBtn,
					Handler_CarAutoDistPlusBtn,
					Handler_CarAutoDistMinusBtn
					;
// кнопки
/*
button_t 	Handler_SrcUpBtn, 
					Handler_SrcDownBtn;
*/
//----------------------------------------
//========================================
// текущие расстояние каретки
int16_t 	distCarSP=1000; 
uint8_t		flagDistCarSP=0; 	// флаг кол-ва 
int32_t 	PV_Cur; // текущая дистанция в шагах.
// номер ускорения для дистанции
int16_t		incDistCarSP=0,
					decDistCarSP=0
					;		
int16_t		distCarPV;
CarAutoUpMode_t CarAutoUpMode=CarAutoUpOff;
//------------------------------------------
// просто переменные
uint8_t temp_8u;
uint16_t 	temp_16u,
					temp2_16u	;

int32_t temp_32;

typedef enum
{
	stateCarSliderIdle,
	
	stateWaitIdle,
	//------------------
	// Одиночные задачи
	//--------------------
	stateCarManNear,
	//---------------------
	stateCarManFar,
	//----------------------
	stateCarCalib,
	//------------------------
	state_CarAutoStart,
	//------------------------
	state_SrcUp,
	//-------------------------
	state_SrcDown,
	//------------------------
	state_SrcDownDist, // отвод источник на определенное положение вниз
	//-----------------------
	// Многоцелевые задачи
	//----------------------
	state_SliderOpen,	// конец задачи
	state_SliderOpen_OutZone,
	state_SliderOpen_OutZone_SrcDownDist,
	//----------------------------
	state_SliderClose,
	state_SliderClose_OutZone,
	//-------------------
	state_AlarmStart,
	state_AlarmStart_SrcUp,
	state_AlarmStart_SrcUp_CarAuto,
	state_AlarmStart_SrcUp_CarAuto_SliderOpen,
	state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone,
	state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto, 

	// отдельные задачи
	// не имеют собственной реализации
	state_OutZone	// отвод каретки из зоны заслонки
	
	
}stateCarSlider_t;



//=========================================
// модуль работы с I2C
I2Csw_gpio_t I2Csw_Gpio;
#define maskTimeMin				0x3F	// маска минут
#define maskTimeFlagAlm		0x40	// Маска флага запуска таймера
#define AT24C32_devAdr		0x57
#define DS3231_devAdr			0x68
#define DS3231_stopTimer  0x9C
#define DS3231_startTimer 0x1C
#define DS3231_control    0x0E


typedef enum
{
	alarmOff=0,
	alarmOn
}alarm_flag_t;

typedef struct
{
	uint32_t s_alm;	// общее кол-во секунд
	int8_t	min_alm;	// задание на таймер
	int8_t	hour_alm;	// кол-во часов
	alarm_flag_t flag_alm;	// наличие флага запуска
	uint8_t cntBtnOff;				// счетчик кнопки запуска таймера
	uint8_t cntBtnOffFlag;		// флаг кнопки запуска таймера
}alarm_t;


uint8_t DS3231_regCntr=DS3231_startTimer;	// регистр управления 
uint8_t adrDS3231=0;
uint8_t DS3231_reg[4];
alarm_t alarm;
uint16_t j;
softTimer_t softTimerUpdate;

uint8_t timerUpdateDisplay=0;
// Состояние
stateCarSlider_t stateCarSlider=stateCarSliderIdle;
//---------------------------
typedef enum
{
	PV_ok,
	PV_error
}PV_flag_t;

typedef enum
{
	SP_ok,
	SP_error
}SP_flag_t;

typedef enum
{
	time_ok,
	time_error
}time_flag_t;

typedef struct
{
	uint16_t num:9; 			//	номер данных для чтения/записи
	PV_flag_t flagPV:1; 	// флаг ошибки загрузки шагов каретки 
	SP_flag_t flagSP:1;		// флаг ошибки загрузки  времени
	time_flag_t flagTime:1;// флаг ошибки загрузки данных времени
	uint8_t :4;						// резерв
	alarm_flag_t flag_alm;	// флаг активного таймера
	int32_t step;	// текущий сохраненный флаг
	int16_t dist; // установка длины
	int8_t hour;	//кол-во часов
	int8_t min;	// кол-во
	uint16_t v1;	// максимальная скорость каретки
	uint16_t a1;	// время ускорения 
	uint16_t v2;	// максимальная скорость задвижки
	uint16_t v3;	// максимальное скорость источника
}saveData_t;	
saveData_t save;


void handler_timerUpdateDisplay(void)
{
	switch(	timerUpdateDisplay)
	{
		case 1:
		case 3:
			timerUpdateDisplay++;
			timerUpdateDisplay=timerUpdateDisplay%4;
			break;
	}
}

//========================================
// Состояние таймера для сохранения и запуска
// При работе каретки
sliderTimerState_t sliderTimer;


uint16_t timer1,adc2;
//int16_t a,b;


softTimer_t softTimer, 
						softTimerLed, 
						softTimerBtn,
						softTimerDisp,
						softTimerBlinkError,
						softTimerSave;

ControlSrc_t SrcMoveCntr;

flagModeKalib_t flagKalib;




uint16_t nmb=0,adc0;
uint32_t clks;
//-----



FlagStatus adcStatus;

//Tim2_InitSet_t tim2_Set;

int32_t k;

//butKaret_t butKaret;

// флаг для опроса 
uint8_t flagPollingTimeBtn=0;
int8_t minute=0, 
hour=0, 
flagFastMinutePlus, // флаг быстрая перемотка минут
flagFastMinuteMinus,
flagFastHourPlus,		// флаг быстрая перемотка часов
flagFastHourMinus
;


uint8_t flagLed=0; // флаг мигания
//char st2[]="54:21";


//ds3231_reg_t

void DS3231_timeInNumber(	uint8_t *number, 
													uint8_t *time)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		number[i]=(time[i]>>4)*10+time[i]&0x0F;
	}
}

void DS3231_numberInTime(	uint8_t *time,
													uint8_t *number 
													)
{
	uint8_t i;
	for(i=0;i<3;i++)
	{
		time[i]=(number[i]/10)<<4+number[i]%10;
	}
}

uint32_t DS3231_getSecond(	uint8_t *time)
{
	uint32_t s=0;
	uint8_t i;
	const uint16_t k[]={1,60,3600};
	for(i=0;i<3;i++)
	{
		s=s+ ( (time[i]>>4)*10+ time[i] &0x0F )*k[i];
	}
	if (time[3]==0)
	{
		time[3]=1;
	}
	s=s+(uint32_t)(time[3]-1)*86400;
	return s;
}

uint32_t hourMinute_getSecond(	uint8_t hour, uint8_t minute )
{
	uint32_t s=0;
	
	return hour*3600+minute*60;
}

void TIM1_TimeBaseInit_Middle(TIM_Prescaler_TypeDef TIM_Prescaler, uint16_t TIM_Period)
{
	
	const uint16_t TIM1_Prescaler_tab[]={0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535  }; 
	TIM1_TimeBaseInit(TIM1_Prescaler_tab[TIM_Prescaler], 
                       TIM1_COUNTERMODE_UP,
                       TIM_Period, 
											 0);
}


void clockStep(void)
{
	//pidW.stepPV++;
	PID_stepGpio(&pidCar );
	
	TIM2_ClearITPendingBit(TIM2_IT_UPDATE);
}

void clockStepSlider(void)
{
	//pidW.stepPV++;
	PID_stepGpio(&pidSlider );
	
	TIM3_ClearITPendingBit(TIM3_IT_UPDATE);
}

void clockStepSrc(void)
{
	//pidW.stepPV++;
	PID_stepGpio(&pidSrc );
	
	TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
}

// обработчик программных таймеров
void clockBlinkLed(void)
{
	SoftTimer_ClockISR(&softTimerBlinkError);
	SoftTimer_ClockISR(&softTimerSave);
	SoftTimer_ClockISR(&softTimerDisp);
	SoftTimer_ClockISR(&softTimerUpdate);
	SoftTimer_ClockISR(&softTimerLed);
	SoftTimer_ClockISR(&softTimerBtn);
	TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
}

/*
void PID_initPV(pidW_t *pidW, int32_t PV)
{
	pidW->PV=PV;
}
*/
/*
void endKaretKalibPV(void)
{
	int32_t PV=1000;
	//disableInterrupts();
//	PID_initPV(&pidW, PV);
//	PID_stop(&pidW);
	//EXTI_SetExtIntSensitivity(  EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);
	//GPIO_Init(endKaretKalib, GPIO_MODE_IN_PU_NO_IT);
	//GPIO_Init(endKaretKalib, GPIO_MODE_IN_PU_IT);
	//enableInterrupts();
	
	
}
*/

uint8_t updateDispaly=0;
void Handler_display(void)
{
	updateDispaly=~updateDispaly;
}



void Handler_time_btn(void)
{
	nop();
	flagPollingTimeBtn=1;
	flagDistCarSP=1;
	alarm.cntBtnOffFlag=1;
}

// 
uint8_t flagSaveData=0;
void Handler_SaveData(void)
{
	flagSaveData=0xFF;
}

uint8_t blinkError=0;
void Handler_displayError(void)
{
	blinkError=~blinkError;
}





void blinkLed(void)
{
	flagLed=~flagLed;
}

void DS3231_timeInNamber()
{
	
}

void LedPolling(void)
{
	// индикация сигнала от каретки
		// Концевик максимум 
		if ( !GPIO_ReadInputPin( CarFarLimit))
		{
			GPIO_WriteHigh(CarFarLimitLed);
			
		}
		else
		{
			GPIO_WriteLow(CarFarLimitLed);
		}
		
		if ( !GPIO_ReadInputPin(CarNearLimit))
		{
			GPIO_WriteHigh(CarNearLimitLed);
			
		}
		else
		{
			GPIO_WriteLow(CarNearLimitLed);
		}
		// Зона заслонки
		if ( !GPIO_ReadInputPin(CarCalibLimit))
		{
			GPIO_WriteHigh(CarCalibLimitLed);
		}
		else
		{
			GPIO_WriteLow(CarCalibLimitLed);
		}
		// --------------------------------
		
		//====================================
		// Опрос концевиков
		if ( !GPIO_ReadInputPin( SliderOpenLimit))
		{
			GPIO_WriteHigh(SliderOpenLimitLed);
		}
		else
		{
			GPIO_WriteLow(SliderOpenLimitLed);
		}
		
		if ( !GPIO_ReadInputPin(SliderCloseLimit))
		{
			GPIO_WriteHigh(SliderCloseLimitLed);
		}
		else
		{
			GPIO_WriteLow(SliderCloseLimitLed);
		}
		
		//==============================
		// Алгоритм работы Источника
		//------------------------------
		// индикация сигнала от светодиода
		if ( !GPIO_ReadInputPin( SrcUpLimit))
		{
			GPIO_WriteHigh(SrcUpLimitLed);
		}
		else
		{
			GPIO_WriteLow(SrcUpLimitLed);
		}
		
		if ( !GPIO_ReadInputPin(SrcDownLimit))
		{
			GPIO_WriteHigh(SrcDownLimitLed);
		}
		else
		{
			GPIO_WriteLow(SrcDownLimitLed);
		}
		
		
		//==================================
		//  мигание светодиодами в зависимости 
		// 	от того какая ось работает
		//-----------------------------
		// Мигание при работе оси каретки
		if (PID_getStatePower(&pidCar) != power_off	)
		{
			if (PID_getStateDir(&pidCar) == stepUp	)
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(CarFarBtnLed);
				}
				else
				{
					GPIO_WriteLow(CarFarBtnLed);
				}
			}
			else
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(CarNearBtnLed);
				}
				else
				{
					GPIO_WriteLow(CarNearBtnLed);
				}
			}
		}
		else
		{
			GPIO_WriteLow(CarFarBtnLed);
			GPIO_WriteLow(CarNearBtnLed);
		}
		// Мигание при работе оси затвора	
		if (PID_getStatePower(&pidSlider) != power_off)
		{
			if (PID_getStateDir(&pidSlider) == stepUp	)
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(SliderOpenBtnLed);
				}
				else
				{
					GPIO_WriteLow(SliderOpenBtnLed);
				}
			}
			else
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(SliderCloseBtnLed);
				}
				else
				{
					GPIO_WriteLow(SliderCloseBtnLed);
				}
			}
		}
		else
		{
			GPIO_WriteLow(SliderOpenBtnLed);
			GPIO_WriteLow(SliderCloseBtnLed);
		}
		//----------------------------
		
		
		
		//----------------------------
		// Мигание при работе оси источника		
		if (PID_getStatePower(&pidSrc) != power_off)
		{
			if (PID_getStateDir(&pidSrc) == stepUp	)
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(SrcUpBtnLed);
				}
				else
				{
					GPIO_WriteLow(SrcUpBtnLed);
				}
			}
			else
			{
				if ( flagLed )
				{
					GPIO_WriteHigh(SrcDownBtnLed);
				}
				else
				{
					GPIO_WriteLow(SrcDownBtnLed);
				}
			}
		}
		else
		{
			GPIO_WriteLow(SrcUpBtnLed);
			GPIO_WriteLow(SrcDownBtnLed);
		}
		//--------------------------------
		// Проверка различных режимов работы
		if (	stateCarSlider==state_SliderOpen_OutZone
				||stateCarSlider==state_SliderOpen_OutZone_SrcDownDist)
		{
			GPIO_WriteHigh(SliderOpenBtnLed);
		}
		//-------------------------
		
		if (	stateCarSlider==state_SliderClose_OutZone)
		{
			GPIO_WriteHigh(SliderCloseBtnLed);
		}
		//---------------------------------------
		if (	stateCarSlider == stateCarCalib)
		{
			GPIO_WriteHigh(CarCalibBtnLed);
		}
		else
		{
			GPIO_WriteLow(CarCalibBtnLed);
		}
		//-----------------------------
		// 
		if (	stateCarSlider == state_CarAutoStart)
		{
			GPIO_WriteHigh(CarAutoStartBtnLed);
		}
		else
		{
			GPIO_WriteLow(CarAutoStartBtnLed);
		}
		
		//--------------------------------------
		// индикация включения режима таймера
		if (	 state_AlarmStart <= stateCarSlider 
				&& stateCarSlider<=state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto
				|| alarm.flag_alm== alarmOn )
		{
			GPIO_WriteHigh(SliderTimeStartBtnLed);
		}
		else
		{
			GPIO_WriteLow(SliderTimeStartBtnLed);
		}
		
}

void StepMotorCar_Init(void)
{
	// настройка оси каретки
	TIM2_ARRPreloadConfig(ENABLE);
	TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
	
	PID_Init_Gpio(	&pidCar,
									TIM2_TimeBaseInit,
									TIM2_Cmd,
									
									clkStep1,
									dirStep1,
									enStep1,
									//CarCalibLimit,
									
									
									limitSW_Level_Low,
									CarFarLimit,
									
									limitSW_Level_Low,
									CarNearLimit	);
	
	PID_initTab(&pidCar,
							save.step, // текущее значение PV
							16000000, // частота
							800, // микрошагов
							20, // Шаг дифференцирования ms 
							save.a1, // время раскрутки
							200, // время торможения
							save.v1, // скорость вращения
							3, // кол-во последних уровней для добавления шагов
							10 // кол-во шагов перед остановкой
							);
}

void StepMotorSlider_Init(void)
{
	// Затвор
	TIM3_ARRPreloadConfig(ENABLE);
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
	
	PID_Init_Gpio(	&pidSlider,
									TIM3_TimeBaseInit,
									TIM3_Cmd,
									
									clkStep2,
									dirStep2,
									enStep2,
									
									limitSW_Level_Low,
									SliderOpenLimit,
									
									limitSW_Level_Low,
									SliderCloseLimit	);
	
	PID_initTab(&pidSlider,
							0, // текущее значение PV
							16000000, // частота
							800, // микрошагов
							20, // Шаг дифференцирования ms 
							1500, // время раскрутки
							300, // время торможения
							save.v2, // скорость вращения
							3, // кол-во последних уровней для добавления шагов
							10 // кол-во шагов перед остановкой
							);
}

void StepMotorSrc_Init(void)
{
	
	//----------------------------------------
	
	//----------------------------------------
	TIM1_ARRPreloadConfig(ENABLE);
	TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
	TIM1_TimeBaseInit(0, 
                       TIM1_COUNTERMODE_UP,
                       0xFFFF,
											 0);
											 
	
	
	PID_Init_Gpio(	&pidSrc,
									TIM1_TimeBaseInit_Middle,
									TIM1_Cmd,
									
									clkStepSrc_3,
									dirStepSrc_3,
									enStepSrc_3,
									
									limitSW_Level_Low,
									SrcUpLimit,
									
									limitSW_Level_Low,
									SrcDownLimit	);
	
	PID_initTab(&pidSrc,
							0, // текущее значение PV
							16000000, // частота
							1000, // микрошагов
							20, // Шаг дифференцирования ms 
							1500, // время раскрутки
							1000, // время торможения
							save.v3, // скорость вращения
							3, // кол-во последних уровней для добавления шагов
							10 // кол-во шагов перед остановкой
							);
	// настройка зоны заслонки/ точки калибровки
}


void DownloadSetting( void)
{
	uint16_t i,j;
		//  Поиск данных для загрузки из памяти
	
	
	for (i=0;i<512;i++)
	{
		//save.num=i;
		j=0;
		while(
		I2Csw_readFromMemoryByte2( &I2Csw_Gpio,
																AT24C32_devAdr,
																i*8,
																(uint8_t*)&save.step,
																8)!=I2Csw_success && j++ <maxRepeatWrite );
																
		if (save.step>=0)
		{
			// координата существует и найдена
			if (save.dist<0)
			{
					save.dist=1000;
			}
			
			if (save.hour<0 || save.min<0)
			{
				save.hour=1;
				save.min=0; // будильник отключен
			}
			goto next_record;
			break;
		}
	}
	//если все блок просмотрен
	// но истинной координаты не найдено, 
	save.step=100000;
	save.dist=1000;
	save.hour=1;
	save.min=0; // будильник отключен
	// то включаем мигание ошибки
	SoftTimer_CMD(&softTimerBlinkError,ENABLE);
	//save.dist=1000;
	next_record:
	// запись данных в новую ячейку памяти
	save.num++;
	j=0;
	while(I2Csw_sendToMemoryByte2( &I2Csw_Gpio,
																	AT24C32_devAdr,
																	save.num*8,
																	(uint8_t*)&save.step,
																	8)!=I2Csw_success && j++ <maxRepeatWrite );
																	
	// очистка данных
	
	// стираем прошлые данные
	temp_16u=0xFFFF;
	//temp_32u=0xFFFFFFFFF;
	for (i=0;i<4;i++)
	{
		j=0;
		while(
		I2Csw_sendToMemoryByte2(&I2Csw_Gpio,
															AT24C32_devAdr,
															(save.num-1)*8+i*2,
															(uint8_t*)&temp_16u,
															2) !=I2Csw_success && j++ <maxRepeatWrite );
	}
	
	// извлекаем из минут сигнал будильника
	// 6 бит является сингналом будильника
	// 5-0 биты - значение минут
	if (save.min&maskTimeFlagAlm)
	{
		save.flag_alm=alarmOn;
		save.min&=maskTimeMin;
	}
	else
	{
		save.flag_alm=alarmOff;
	}
	//------------------
	//	
	// конец блока чтения данных из EEPROM
}

void GPIO_All_Init(void)
{
	GPIO_Init(CarCalibLimit, GPIO_MODE_IN_PU_IT);
	// Ошибка по превышению тока
	GPIO_Init(AlmStep, GPIO_MODE_IN_PU_NO_IT);
	// кнопки управления кареткой
	GPIO_Init(CarFarBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(CarNearBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(CarAutoStartBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(CarCalibBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(CarAutoDistPlusBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(CarAutoDistMinusBtn, GPIO_MODE_IN_PU_NO_IT);
	// светодиоды кнопок управления кареткой 
	// GPIO_MODE_OUT_PP_LOW_FAST
	GPIO_Init(CarFarBtnLed, 			GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(CarNearBtnLed, 			GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(CarAutoStartBtnLed, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(CarCalibBtnLed, 		GPIO_MODE_OUT_PP_LOW_FAST);
	// Светодиоды индикаторов каретки
	GPIO_Init(CarFarLimitLed, 		GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(CarNearLimitLed, 		GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(CarCalibLimitLed, 	GPIO_MODE_OUT_PP_LOW_FAST);
	//-------------------------------------------
	// кнопки управления затвором
	GPIO_Init(SliderHourPlusBtn, 		GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderHourMinusBtn, 	GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderMinutePlusBtn, 	GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderMinuteMinusBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderTimeStartBtn, 	GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderOpenBtn, 				GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SliderCloseBtn, 			GPIO_MODE_IN_PU_NO_IT);
	// светодиоды кнопок управления затвором
	// Светодиоды
// Встроенный светодиоды Кнопки
	GPIO_Init(SliderOpenBtnLed, 		GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SliderCloseBtnLed, 		GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SliderTimeStartBtnLed,GPIO_MODE_OUT_PP_LOW_FAST);
	// светодиоды индикации управления затвором
	
	GPIO_Init(SliderOpenLimitLed, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SliderCloseLimitLed, GPIO_MODE_OUT_PP_LOW_FAST);
	
	//------------------------------------------------
	//====================================================
	
	
	// кнопки управления источником
	GPIO_Init(SrcUpBtn, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SrcDownBtn, GPIO_MODE_IN_PU_NO_IT);
	// светодиоды кнопок управления источником
	GPIO_Init(SrcUpBtnLed, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SrcDownBtnLed, GPIO_MODE_OUT_PP_LOW_FAST);
	// светодиоды индикации управления источником
	GPIO_Init(SrcUpLimitLed, GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(SrcDownLimitLed, GPIO_MODE_OUT_PP_LOW_FAST);
	//======================================
	// кнопка сброс
	GPIO_Init(ResetBtn, GPIO_MODE_IN_PU_NO_IT);
	// светодиод кнопки сброс
	GPIO_Init(ResetBtnLed, GPIO_MODE_OUT_PP_LOW_FAST);
	//----------------
	// Кнопка Вкл/выкл
	GPIO_Init(OnOffBtn, GPIO_MODE_IN_PU_NO_IT);
	
	// Кнопка стоп
	GPIO_Init(StopBtn, GPIO_MODE_IN_PU_NO_IT);
	//--------------
	// концевик двери
	GPIO_Init(SwDoorLimit, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(SwDoorCoil, GPIO_MODE_OUT_PP_LOW_FAST);
	
}


void SoftTimerAll_Init(void)
{
	//-----------------------------------------
	// инициализация таймера светодиода
	//---------------------------
	SoftTimer_Init(&softTimer,// указатель на работу таймера
							1,			// значение автообновления
							0		// обработчик переполнения	
							);
	SoftTimer_CMD(&softTimer,ENABLE);
	
	
	//-----------------------------------
	
	SoftTimer_Init(&softTimerLed,// указатель на работу таймера
							125,			// значение автообновления
							blinkLed		// обработчик переполнения
							);
	SoftTimer_CMD(&softTimerLed,ENABLE);
	
	//-------------------------
	
	SoftTimer_Init(&softTimerBtn,// указатель на работу таймера
							250,			// значение автообновления
							Handler_time_btn		// обработчик переполнения
							);
	SoftTimer_CMD(&softTimerBtn,ENABLE);
	
	//-------------------------
	// таймер обновления экранчиков
	SoftTimer_Init(&softTimerDisp,// указатель на работу таймера
							20,			// значение автообновления
							Handler_display		// обработчик переполнения
							);
	SoftTimer_CMD(&softTimerDisp,ENABLE);
	
	// таймер для мигания ошибки каретки
	SoftTimer_Init(&softTimerBlinkError,// указатель на работу таймера
							500,			// значение автообновления
							Handler_displayError		// обработчик переполнения
							);
	//SoftTimer_CMD(&softTimerBlinkError,ENABLE);
	
	SoftTimer_Init(&softTimerSave,// указатель на работу таймера
							3000,			// значение автообновления
							Handler_SaveData		// обработчик переполнения
							);
	SoftTimer_CMD(&softTimerSave,ENABLE);
	
	
}

#ifdef  __OSA__
void Task_2(void)
{

	while(1)
	{
		
	OS_Delay(20);
	}
}




void Task(void)
{
	uint8_t i,temp;
	/*
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
	*/
	while(1)
	{
		//GPIO_WriteReverse(GPIOB, GPIO_PIN_1);
		//ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_12);
		/*
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
		*/
		
		
		OS_Delay(500);
	}
}
#endif

uint16_t k2,i;

uint8_t f;
//int32_t PV; // данные для считывания
I2Csw_result_t rt_i2c;
volatile uint16_t delay16, d16;

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
	// Первоначальная инициализация драйверов
	GPIO_Init(enStepSrc_3,GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(enStep1, 		GPIO_MODE_OUT_PP_LOW_FAST);
	GPIO_Init(enStep2, 		GPIO_MODE_OUT_PP_LOW_FAST);
	 
	 
	//Init_Delay();
	temp_32=0x4FFF;
	while(temp_32--);
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
	//-----------------------
	// Инициализация АЦП
	ADC2_SingleOne_Init(	ADC2_PRESSEL_FCPU_D10, 
													ADC2_Handler_NoIT);
	
	
		// чтение данных скорости каретки
	ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_12);
	save.v1= (ADC2_SingleOne_GetConversion()>>3)*8+1;
	
	// время ускорения каретки
	ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_11);
	save.a1= (ADC2_SingleOne_GetConversion()>>3)*24+1;
	
	// максимальная скорость задвижки
	ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_10);
	save.v2= (ADC2_SingleOne_GetConversion()>>3)*8+1;
	
	// максимальная скорость источника
	ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_7);
	save.v3= (ADC2_SingleOne_GetConversion()>>3)*8+1;
													
	
	/// Настройка I2C
	//I2C_Init_7bit(100000);
	f=I2Csoft_INIT(&I2Csw_Gpio,
											GPIO_SDA,
											GPIO_SCL,
											16,	// 16 МГц
											100, // 80 кГц
											100	// таймоут 100 мс
											);
	// Запуск таймер на всякий случай
	I2Csw_sendToMemoryByte1( &I2Csw_Gpio,
													DS3231_devAdr, // адрес часов
													DS3231_control, //адрес 
													&DS3231_regCntr, //значение регистра
													1);
	//--------------------
	// установка флагов ошибок
	//save.flagPV=PV_error;
	//save.flagSP=SP_error;
	//save.flagTime=time_error;
	//------------------------------
	// Загрузка данных из EEPROM 
	DownloadSetting();
	

	//------------------------------------------
	//-----------------------------------------
	// ----------------------------------------
	// инициализация экрана
	TM1637_Init(&disp1, 	tm1637_1,tm1637_bright_3);
	TM1637_Init(&disp2, 	tm1637_2,tm1637_bright_3);
	TM1637_Init(&dispTimer,tm1637_3,tm1637_bright_3);
	TM1637_Init(&dispTimerAlarm, 	tm1637_4,tm1637_bright_3);
	//-----------------------------------
	// Инициализация всех программых таймеров
	SoftTimerAll_Init();
	
	
	
	//----------------
	// настройка таймера
	// 1 мс
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
	//----------------------------------
	// Инициализация и расчет двигателей
	StepMotorCar_Init();
	StepMotorSlider_Init();
	StepMotorSrc_Init();
	// инициализация всех пинов
	GPIO_All_Init();
	
	
	
	
	
	// Инциализация источника
	SrcMoveCntr=SrcMoveIdle;
	//	ButtonInit(&Handler_SrcUpBtn,100, SrcUpBtn);
	
	//ButtonInit(&Handler_SrcDownBtn,100, SrcDownBtn);
	
	//-------------------------------------------------
	//=====================================================
	// Кнопки модуля каретки
	// кнопки 
	//==================================================
	//ButtonInit(&Handler_CarAutoDistPlusBtn,100, CarAutoDistPlusBtn);
	//ButtonInit(&Handler_CarAutoDistMinusBtn,100, CarAutoDistMinusBtn);
	//============================================
	// настройка состояния таймера для 
	sliderTimer=sliderTimerInit;
	// загрузка данных настройки таймера из памяти
	/*
	if(save.min&maskTimeFlagAlm)
	{
		alarm.flag_alm=alarmOn;
	}
	else
	{
		alarm.flag_alm=alarmOff;
	}
	*/
	alarm.min_alm= save.min&maskTimeMin;
	alarm.hour_alm=save.hour;
	alarm.flag_alm=save.flag_alm;
	alarm.s_alm= (uint32_t)alarm.hour_alm*3600+(uint32_t)alarm.min_alm*60;
	
	// загрузка данных в счетчик расстояния
	distCarSP=save.dist;
	
	SoftTimer_Init(&softTimerUpdate,// указатель на работу таймера
							500,			// значение автообновления
							handler_timerUpdateDisplay	// обработчик переполнения
							);
	SoftTimer_CMD(&softTimerUpdate,
									ENABLE);
	
	enableInterrupts();
	
	
	
	while (1)
  {
		//===========================================
		// Оповещение режимов работы
		//---------------------------------------
		LedPolling();
		//-----------------
		// режим закрытия
		
		
		//------------------------------
		// индикация будильника
		//====================================
		// модуль управления экранами
		// каретки
		if (updateDispaly)
		{
			updateDispaly=0;
			
			sprintf(st,"%4d", distCarSP);
			TM1637_6digit_TextOutput(&disp1,st);
			if (blinkError)
			{
				// если ошибка есть
				sprintf(st,"ErrL" );	
			}
			else
			{
				distCarPV=PID_getPV(&pidCar)/ (int16_t)ConvStepIntoDist;
				sprintf(st,"%4d", distCarPV);
			}
			TM1637_4digit_TextOutput(&disp2,st);
		}
		//------------------------------------
		// Проверку на необходимость сохранять данные
		// происходит каждые 3 секунды
		if (flagSaveData)
		{
			flagSaveData=0;
			// Проверка на необходимость сохранения данных
			// часов
			if (alarm.hour_alm != save.hour || 
					alarm.min_alm != save.min ||  
					alarm.flag_alm != save.flag_alm  )
			{
				save.hour=alarm.hour_alm;
				//save.min=alarm.flag_alm|alarm.min_alm;
				if (alarm.flag_alm)
				{
					save.min=alarm.min_alm|maskTimeFlagAlm;
					save.flag_alm=alarmOn;
				}
				else
				{
					save.min=alarm.min_alm;
					save.flag_alm=alarmOff;
				}
				j=0;
				while(
				I2Csw_sendToMemoryByte2(&I2Csw_Gpio,
																	AT24C32_devAdr,
																	save.num*8+6,
																	(uint8_t*)&save.hour,
																	2) !=I2Csw_success && j++ <maxRepeatWrite );
				save.min=alarm.min_alm;
			}
			//--------------
			// Проверка на необходимость сохранения данных дистанации
			if (distCarSP !=save.dist)
			{
				save.dist=distCarSP;
				j=0;
				while(
				I2Csw_sendToMemoryByte2(&I2Csw_Gpio,
																	AT24C32_devAdr,
																	save.num*8+4,
																	(uint8_t*)&save.dist,
																	2) !=I2Csw_success && j++ <maxRepeatWrite );
			}
			//--------------------
			// Проверка на необходимость перерасчета скорости
			if (stateCarSlider == stateCarSliderIdle) 
			{
				//----------------------------------
				// Проверка настройки для первой оси
				//максимальная скорость каретки
				ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_12);
				temp_16u= (ADC2_SingleOne_GetConversion()>>3)*8+1;
				
				// время ускорения каретки
				ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_11);
				temp2_16u= (ADC2_SingleOne_GetConversion()>>3)*24+1;
				
				
				if (save.v1 !=temp_16u  || save.a1!= temp2_16u)
				{
					save.v1 =temp_16u ;
					save.a1= temp2_16u;
						StepMotorCar_Init();
				}
				//---------------------------------
				//Проверка настройки для 2 оси
				// максимальная скорость задвижки
				ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_10);
				temp_16u = (ADC2_SingleOne_GetConversion()>>3)*8+1;
				if (save.v2 !=temp_16u  )
				{
					save.v2 =temp_16u ;
					StepMotorSlider_Init();
				}
				//----------------------------------
				// Проверка настройки для 3 оси
				// максимальная скорость источника
				ADC2_SingleAny_Start(ADC2_SOFT,ADC2_CHANNEL_7);
				temp_16u = (ADC2_SingleOne_GetConversion()>>3)*8+1;
				// Все оси не вращаются
				if (save.v3 !=temp_16u )
				{
					save.v3 =temp_16u ;
					StepMotorSrc_Init();
				}
			}
			
		}
		
		//------------------------------------
		// Модуль управления кареткой 
		
		// если затвор закрыт и достигли зоны затвора, то останавливаемся
		//if ( GPIO_ReadInputPin(SliderOpenLimit) && !GPIO_ReadInputPin(CarCalibLimit))
		//{
		//	PID_stop_auto(&pidCar);
			//PID_stop_man_dir(&pidCar, stepDown);
		//}
		//--------------------------------
		// модуль управления кареткой и затвором
		
		switch(stateCarSlider)
		{
			case stateCarSliderIdle:
				// обработчик кнопок
				if ( GPIO_ReadInputPin( OnOffBtn))
				{
						// ручное перемещение каретки назад
						if (!GPIO_ReadInputPin(CarNearBtn))
						{
							stateCarSlider=stateCarManNear;	
							goto lbl_stateCarManNear;					
						}
						
						// ручное перемещение каретки вперед
						if (!GPIO_ReadInputPin(CarFarBtn))
						{
							stateCarSlider=stateCarManFar;	
							goto lbl_stateCarManFar;					
						}
						
						// калибровка
						if (!GPIO_ReadInputPin(CarCalibBtn ))
						{
							if ( GPIO_ReadInputPin( CarCalibLimit) )
							{
								PID_calibInit( &pidCar,
																CarCalibLimit,
																limitSW_Level_Low,		
																(int32_t)CalibLimitIn_mm* (int32_t)ConvStepIntoDist
																);
								PID_start_man(&pidCar, stepDown);
								
							}
							else
							{
								PID_calibInit( &pidCar,
																CarCalibLimit,
																limitSW_Level_High,		
																(int32_t)CalibLimitIn_mm* (int32_t)ConvStepIntoDist
																);
								PID_start_man(&pidCar, stepUp);	
							}
							// Отключаем таймер
							SoftTimer_CMD(&softTimerBlinkError,DISABLE);
							blinkError=0; // отключаем мигание
							stateCarSlider=stateCarCalib;	
							goto lbl_stateCarCalib;	
						}
						//----------------------------------
						// Кнопка запуск задания
						if (!GPIO_ReadInputPin(CarAutoStartBtn ))
						{
							stateCarSlider=state_CarAutoStart;
							goto lbl_state_CarAutoStart;
						}
						//-----------------
						// открыть створку
						if ( !GPIO_ReadInputPin(SliderOpenBtn) && 
									GPIO_ReadInputPin(SliderOpenLimit))
						{
							stateCarSlider=state_SliderOpen_OutZone_SrcDownDist;
							goto lbl_state_SliderOpen_OutZone_SrcDownDist;
						
						}
						//----------------------------------------------------
						// закрыть створку
						if ( !GPIO_ReadInputPin(SliderCloseBtn) && 
									GPIO_ReadInputPin(SliderCloseLimit) )
						{
							stateCarSlider=state_SliderClose_OutZone;
							goto lbl_state_SliderClose_OutZone;
						}
						
						// поднять источник
						if ( !GPIO_ReadInputPin(SrcUpBtn) && 
									GPIO_ReadInputPin(SrcUpLimit))
						{
							stateCarSlider=state_SrcUp;
							goto lbl_state_SrcUp;
						}
						//----------------------------------------------------
						// опустить источник
						if ( !GPIO_ReadInputPin(SrcDownBtn) 
								&& GPIO_ReadInputPin(SrcDownLimit) 	)
						{
							if (!GPIO_ReadInputPin(SrcUpLimit) )
							{
								stateCarSlider=state_SrcDownDist;
								goto lbl_state_SrcDownDist;
							}
							else
							{
								stateCarSlider=state_SrcDown;
								goto lbl_state_SrcDown;
							}
							
						}
						//-------------------------------------------
						// запуск будильника
						if ( !GPIO_ReadInputPin(SliderTimeStartBtn) 
								&& alarm.cntBtnOff>=alarm_btn_time_off )
						{
							alarm.cntBtnOff=0;
							if (	alarm.flag_alm == alarmOff)
							{
								
								// алгоритм включения будильника
								if ( GPIO_ReadInputPin(SliderOpenLimit))
								{
									// если створка закрыта
									stateCarSlider =state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto;
									goto        lbl_state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto;
								}
								else
								{
									stateCarSlider =state_AlarmStart_SrcUp_CarAuto;
									goto        lbl_state_AlarmStart_SrcUp_CarAuto;
								}
							}
							else
							{
								// алгоритм выключения будильника
								alarm.flag_alm = alarmOff;
							}
						}
						else
						{
							// сброс счетчика нажатий
							if (alarm.cntBtnOffFlag)
								{
									alarm.cntBtnOffFlag=0;
									if (alarm.cntBtnOff<=alarm_btn_time_off)
									{
										alarm.cntBtnOff++;
									}
								}
						}
						//-------------------
						// отключаем замок двери
						GPIO_WriteLow(SwDoorCoil);
				}
				else
				{
					// контакты замкнуты
					// идет блокировка кнопок
					GPIO_WriteHigh(SwDoorCoil);
				}
				//-----------------------------------
				if (alarm.flag_alm == alarmOn)
				{
					if (alarm.s_alm<=DS3231_getSecond(	DS3231_reg) ) 
					{
						alarm.flag_alm = alarmOff;
						stateCarSlider =state_SliderClose_OutZone;
						goto        lbl_state_SliderClose_OutZone;
					}
				}
				//-----------------------
				// Открытие двери
				// Приводит к уборке источника
//				SwDoorLimit
				if ( !GPIO_ReadInputPin(SwDoorLimit) && 
							!GPIO_ReadInputPin(SrcUpLimit) )
				{
					stateCarSlider=state_SrcDownDist;
					goto lbl_state_SrcDownDist;
				}
				
				break;
			//------------------------
			// обработчик команд
			case stateCarManNear:
				//lbl_stateCarManNear:
				if (!GPIO_ReadInputPin(CarNearBtn))
				{
					lbl_stateCarManNear:
					if ( GPIO_ReadInputPin(CarCalibLimit))
					{
						PID_start_man(&pidCar, stepDown);
					}
					else
					if ( !GPIO_ReadInputPin(SliderOpenLimit))
					{
						PID_start_man(&pidCar, stepDown);
					}
					else
					{
						PID_stop(&pidCar);
						goto lbl_testIdle;
					}
				}
				else
				{
					PID_stop(&pidCar);
					goto lbl_testIdle;
				}
				break;
				//--------------------------------
			case stateCarManFar:
				lbl_stateCarManFar:
				if (!GPIO_ReadInputPin(CarFarBtn))
				{
					//lbl_stateCarManFar:
					PID_start_man(&pidCar, stepUp);								
				}
				else
				{
					PID_stop_man_dir(&pidCar, stepUp);
					goto lbl_testIdle;
				}
				break;
				//-----------------
			case stateCarCalib:
				lbl_stateCarCalib:
				if (	!GPIO_ReadInputPin(CarNearBtn)
						|| !GPIO_ReadInputPin(CarFarBtn)	)
				{
					PID_stop(&pidCar);
					stateCarSlider=stateWaitIdle;
				}
				goto lbl_testIdle;
				break;
			//-------------------------
			case state_CarAutoStart:
				lbl_state_CarAutoStart:
				//stateCarSliderIdle
				
		
				if (PID_getStatePower(&pidCar) == power_off	
						&& PID_getStateRot(&pidCar)==rot_stop)
				{
					if( PID_getPV(&pidCar)  == (int32_t)ConvStepIntoDist * (int32_t)distCarSP)
					{
						if(stateCarSlider == state_CarAutoStart)
						{
							stateCarSlider=stateWaitIdle;
							goto lbl_testIdle;
						}
						else
						{
							// переход в следующее состояние
							stateCarSlider--;
						}
						
					}
					else
					if ( !GPIO_ReadInputPin(SliderOpenLimit) || GPIO_ReadInputPin(CarCalibLimit))
					{
						PID_start_auto(&pidCar, 
									(int32_t)ConvStepIntoDist * (int32_t)distCarSP);
						
						if ( 		!GPIO_ReadInputPin(CarFarLimit)&& PID_getStateDir(&pidCar)==stepUp
								|| !GPIO_ReadInputPin(CarNearLimit)&& PID_getStateDir(&pidCar)==stepDown
						)
						{
							PID_stop(&pidCar);
							stateCarSlider=stateWaitIdle;
							goto lbl_testIdle;
						}
									
					}
					else
					{
						if ( PID_getPV(&pidCar) <(int32_t)ConvStepIntoDist * (int32_t)distCarSP)
						{
							PID_start_auto(&pidCar, 
									(int32_t)ConvStepIntoDist * (int32_t)distCarSP);
							if ( 		!GPIO_ReadInputPin(CarFarLimit)&& PID_getStateDir(&pidCar)==stepUp
								|| !GPIO_ReadInputPin(CarNearLimit)&& PID_getStateDir(&pidCar)==stepDown
							)
							{
								PID_stop(&pidCar);
								stateCarSlider=stateWaitIdle;
								goto lbl_testIdle;
							}
						}
						else
						{
							if (stateCarSlider==state_CarAutoStart )
							{
								stateCarSlider=stateWaitIdle;
								goto lbl_testIdle;
							}
							else
							{
								stateCarSlider--;
							}
						}
					}
				}
				else
				if (	!GPIO_ReadInputPin(CarFarBtn)
						||!GPIO_ReadInputPin(CarNearBtn))
				{
					PID_stop(&pidCar);
					stateCarSlider=stateWaitIdle;
					goto lbl_testIdle;
					//PID_start_man(&pidCar, stepUp);								
				}
				else
				if ( 		GPIO_ReadInputPin(SliderOpenLimit) 
						&& !GPIO_ReadInputPin(CarCalibLimit)
						&& PID_getStateDir(&pidCar)== stepDown )
				{
					PID_stop(&pidCar);
					
					if(stateCarSlider == state_CarAutoStart)
					{
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
					else
					{
						// переход в следующее состояние
						stateCarSlider--;
					}
				}
				
				break;
			//----------------------------------------
			
			//------------------------
	
			case state_SliderOpen:
				lbl_state_SliderOpen:
				if (GPIO_ReadInputPin(CarCalibLimit)	)
				{
					// каретка вне зоны заслонки
					if (!GPIO_ReadInputPin(SliderCloseBtn))
					{
						// нажали в другую сторону
						PID_stop(&pidSlider);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
						//PID_start_man(&pidCar, stepUp);								
					}
					else
					{
						PID_start_man(&pidSlider, stepUp);
						if ( !GPIO_ReadInputPin(SliderOpenLimit))
						{
							// достигли зоны концевика
							PID_stop(&pidSlider);
							if (stateCarSlider==state_SliderOpen)
							{
								PID_stop(&pidSlider);
								stateCarSlider=stateWaitIdle;
								goto lbl_testIdle;
							}
							else
							{
								stateCarSlider--;
							}
						}
					}
				}
				else
				{
					// Каретка в зоне заслонки
					PID_stop(&pidSlider);
					stateCarSlider++;
				}
				
				break;
			
			//----------------------------------	
			
			//------------------------
			case state_SliderClose:
				lbl_state_SliderClose:
				if (GPIO_ReadInputPin(CarCalibLimit)	)
				{
					// каретка вне зоны заслонки
					if (!GPIO_ReadInputPin(SliderOpenBtn))
					{
						// нажали в другую сторону
						PID_stop(&pidSlider);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
						//PID_start_man(&pidCar, stepUp);								
					}
					else
					{
						PID_start_man(&pidSlider, stepDown);
						if ( !GPIO_ReadInputPin(SliderCloseLimit))
						{
							// достигли зоны концевика
							PID_stop(&pidSlider);
							if (stateCarSlider==state_SliderClose)
							{
								stateCarSlider=stateWaitIdle;
								goto lbl_testIdle;
							}
							else
							{
								stateCarSlider--;
							}
						}
					}
				}
				else
				{
					// Каретка в зоне заслонки
					PID_stop(&pidSlider);
					stateCarSlider++;
				}
				
				break;
			
			
			case state_OutZone:
				lbl_state_OutZone:
				if (!GPIO_ReadInputPin(CarCalibLimit))
				{
					// запуск каретки для вывода из зоны заслонки
					PID_start_man(&pidCar, stepUp);
					
					if (	!GPIO_ReadInputPin(CarNearBtn))
					{
						PID_stop(&pidCar);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
						//PID_start_man(&pidCar, stepUp);								
					}
				}
				else
				{
					PID_stop_man(&pidCar);
					// переход на следующую стадию автомата
					if (stateCarSlider == state_OutZone)
					{
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
					else
					{
						stateCarSlider--; 
					}
					
				}
				break;
			//--------------------
			case state_SliderClose_OutZone:
				lbl_state_SliderClose_OutZone:
				goto lbl_state_OutZone;
				break;
			//-------------------------------------------------
			case state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto:
				lbl_state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone_CarAuto:
				
			case state_AlarmStart_SrcUp_CarAuto:
				lbl_state_AlarmStart_SrcUp_CarAuto:
				goto lbl_state_CarAutoStart;
				
				break;
				
			case state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone:
				lbl_state_AlarmStart_SrcUp_CarAuto_SliderOpen_OutZone:
				
				goto lbl_state_OutZone;
				
				break;
			case state_AlarmStart_SrcUp_CarAuto_SliderOpen:
				lbl_state_AlarmStart_SrcUp_CarAuto_SliderOpen:
				
				goto lbl_state_SliderOpen;
				
				break;
			
			
			case state_AlarmStart:
				alarm.flag_alm=alarmOn;
				stateCarSlider=stateWaitIdle;
				goto lbl_testIdle;
				break;
				
			case state_AlarmStart_SrcUp:
				lbl_state_AlarmStart_SrcUp:
				
			case state_SrcUp:	
				lbl_state_SrcUp:	
				
				if (GPIO_ReadInputPin(SrcUpLimit))
				{
					PID_start_man(&pidSrc, stepUp);	
					if (!GPIO_ReadInputPin(SrcDownBtn))
					{
						PID_stop(&pidSrc);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
				}
				else
				{
					PID_stop(&pidSrc);
					if (stateCarSlider == state_SrcUp)
					{
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
					else
					{
						stateCarSlider--;
					}
				}
				
				break;
			
			
			case state_SrcDownDist:	
				lbl_state_SrcDownDist:	
				
				if (  !GPIO_ReadInputPin(SrcUpLimit) &&
							PID_getStatePower(&pidSrc) == power_off && 
							PID_getStateRot(&pidSrc)==rot_stop 
						)
				{
					PID_setPV(&pidSrc,
									0);
									
					PID_start_auto(&pidSrc, 
							PID_getPV(&pidSrc)- (int32_t)SrcConvStepIntoDist*(int32_t)SrcDistDown);
					
					if (!GPIO_ReadInputPin(SrcUpBtn))
					{
						PID_stop(&pidSlider);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
				}
				else
				if (PID_getStatePower(&pidSrc) == power_off
					&& PID_getStateRot(&pidSrc)==rot_stop) 
				{
					
					if ( stateCarSlider == state_SrcDownDist)
					{
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
					else
					{
						stateCarSlider--;
					}
				}
				else
				if (!GPIO_ReadInputPin(SrcUpBtn))
				{
					PID_stop(&pidSlider);
					stateCarSlider=stateWaitIdle;
					goto lbl_testIdle;
				}
				
				break;
			
			case state_SliderOpen_OutZone_SrcDownDist:
				lbl_state_SliderOpen_OutZone_SrcDownDist:
				
			
				goto lbl_state_SrcDownDist;
				break;
			
			case state_SliderOpen_OutZone:
				lbl_state_SliderOpen_OutZone:
				
				goto lbl_state_OutZone;
				break;



			case state_SrcDown:	
				lbl_state_SrcDown:	
				
				if (GPIO_ReadInputPin(SrcDownLimit))
				{
					PID_start_man(&pidSrc, stepDown);	
					if (!GPIO_ReadInputPin(SrcUpBtn))
					{
						PID_stop(&pidSrc);
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
				}
				else
				{
					PID_stop(&pidSrc);
					if (stateCarSlider == state_SrcDown)
					{
						stateCarSlider=stateWaitIdle;
						goto lbl_testIdle;
					}
					else
					{
					stateCarSlider--;
					}
				}
				
				break;
			
			
			
			//--------------------------
			case stateWaitIdle:
				// остановка осей
			default:
				PID_stop(&pidCar);
				PID_stop(&pidSlider);
				PID_stop(&pidSrc);
				lbl_testIdle:
				if (PID_getStatePower(&pidCar) == power_off	
						&& PID_getStateRot(&pidCar)==rot_stop)	
				{
					// проверка сохраненных данных
					// Чтение данных из 
					I2Csw_readFromMemoryByte2( &I2Csw_Gpio,
																AT24C32_devAdr,
																save.num*8,
																(uint8_t*)&temp_32,
																4);
					PV_Cur=PID_getPV(&pidCar);
					if (temp_32!=PV_Cur)
					{
						// запись данных
						j=0;
						while(
						I2Csw_sendToMemoryByte2(&I2Csw_Gpio,
																			AT24C32_devAdr,
																			save.num*8,
																			(uint8_t*)&PV_Cur,
																			4) !=I2Csw_success && j++ <maxRepeatWrite );
					}
					// дальнейшая проверка на остановку
					if( PID_getStatePower(&pidSlider) == power_off	
							&& PID_getStateRot(&pidSlider)==rot_stop	
							&& PID_getStatePower(&pidSrc) == power_off	
							&& PID_getStateRot(&pidSrc)==rot_stop	
							)
					{
						stateCarSlider=stateCarSliderIdle;
					}
				}
		}
		
		
		
		
		
		
		//--------------------------------------
		//--------------------
		// кнопки увеличения дистанции
		if ( !GPIO_ReadInputPin(CarAutoDistPlusBtn)  )
		{
			if (flagDistCarSP)
			{
				if (incDistCarSP ==0 )
				{
					nop();
					incDistCarSP++;
				}
				else
				if (incDistCarSP <=10)
				{
					incDistCarSP++;
					distCarSP++;
				}
				else
				if (incDistCarSP <=100)
				{
					incDistCarSP+=10;
					distCarSP+=10;
				}
				else
				if (incDistCarSP <=1000)
				{
					incDistCarSP+=100;
					distCarSP+=100;
				}
				else
				{
					//incDistCarSP+=100;
					distCarSP+=500;
				}
				
				if (distCarSP>maxDistCarSP)
					{
						distCarSP=maxDistCarSP;
					}
					flagDistCarSP=0;
			}
		}
		else
		{
			incDistCarSP=0;
		}
		
		if ( !GPIO_ReadInputPin(CarAutoDistMinusBtn)  )
		{
			if (flagDistCarSP)
			{
				if (decDistCarSP ==0 )
				{
					decDistCarSP++;
				}
				else
				if (decDistCarSP <=10)
				{
					decDistCarSP++;
					distCarSP--;
				}
				else
				if (decDistCarSP <=100)
				{
					decDistCarSP+=10;
					distCarSP-=10;
				}
				else
				if (decDistCarSP <=1000)
				{
					decDistCarSP+=100;
					distCarSP-=100;
				}
				else
				{
					//incDistCarSP+=100;
					distCarSP-=500;
				}
				
				if (distCarSP<0)
					{
						distCarSP=0;
					}
				flagDistCarSP=0;
			}
		}
		else
		{
			decDistCarSP=0;
		}
		
		
		
		
		//-----------------------------------
		//===================================
		// Затвор
		
		//--------------------
		// опрос кнопок таймера
		if (flagPollingTimeBtn)
		{
			if ( !GPIO_ReadInputPin(SliderHourPlusBtn))
			{
				if ( flagFastHourPlus<edgeFastHour)
				{
				alarm.hour_alm++;
				flagFastHourPlus++;
				}
				else
				{
					alarm.hour_alm+=edgeFastHour;
				}
				
				if (hour>99)
				{
					alarm.hour_alm=alarm.hour_alm%100;
				}
				
				
			}
			else
			{
				flagFastHourPlus=0;
			}
			
			
			if ( !GPIO_ReadInputPin(SliderHourMinusBtn))
			{
				
				if ( flagFastHourMinus < edgeFastHour)
				{
					alarm.hour_alm--;
					flagFastHourMinus++;
				}
				else
				{
					alarm.hour_alm-=edgeFastHour;
				}
				
				
				if (hour<0)
				{
					alarm.hour_alm=100+alarm.hour_alm;
				}
			}
			else
			{
				flagFastHourMinus=0;
			}
			
			
			if ( !GPIO_ReadInputPin(SliderMinutePlusBtn))
			{
				
				
				
				if ( flagFastMinutePlus<edgeFastMinute)
				{
				alarm.min_alm++;
				flagFastMinutePlus++;
				}
				else
				{
					alarm.min_alm+=edgeFastMinute;
				}
				
				if (minute>59)
				{
					alarm.min_alm=alarm.min_alm%60;
				}
				
			}
			else
			{
				flagFastMinutePlus=0;
			}
			
			if ( !GPIO_ReadInputPin(SliderMinuteMinusBtn))
			{
				if ( flagFastMinuteMinus<edgeFastMinute)
				{
				alarm.min_alm--;
				flagFastMinuteMinus++;
				}
				else
				{
					alarm.min_alm-=edgeFastMinute;
				}
				
				if (alarm.min_alm<0)
				{
					alarm.min_alm=60+alarm.min_alm;
				}
			}
			else
			{
				flagFastMinuteMinus=0;
			}
			//---------
			// вывод данных на экран
			alarm.s_alm= (uint32_t)alarm.hour_alm*3600+(uint32_t)alarm.min_alm*60;
			sprintf(st,"%02d:%02d", (uint16_t)alarm.hour_alm, (uint16_t)alarm.min_alm);
			TM1637_4digit_TextOutput(&dispTimerAlarm,st);
			flagPollingTimeBtn=0;
		}
		// конец блока настройки будильника
		//--------------------------------------
		//  Считывание данных из DS3231
		//  и обновление экрана для мигания
		//--------------------------------------
		switch(timerUpdateDisplay)
		{
			case 0:
				sprintf(st,"%d%d:%d%d",DS3231_reg[1]>>4,DS3231_reg[1]&0x0F,DS3231_reg[0]>>4,DS3231_reg[0]&0x0F);
				goto lbl_printTimer;
				break;
			case 2:
				sprintf(st,"%d%d%d%d",DS3231_reg[1]>>4,DS3231_reg[1]&0x0F,DS3231_reg[0]>>4,DS3231_reg[0]&0x0F);
				lbl_printTimer:
				TM1637_4digit_TextOutput(&dispTimer,st);
				timerUpdateDisplay++;
				break;
		}
		
		
		
		if ( !GPIO_ReadInputPin(SliderTimeStartBtn))
		{
			nop();
		}
		//-----------------------
		// Запуск таймера и обновление экрана
		if ( !GPIO_ReadInputPin(SliderOpenLimit) )
		{
			switch(sliderTimer)
			{
				case sliderTimerStop:
					I2Csw_sendToMemoryByte1(&I2Csw_Gpio,
																		DS3231_devAdr,
																		0,
																		DS3231_reg,
																		4);
				case sliderTimerRun:
					lbl_timerRun:
					// обновление часов
					if ( !GPIO_ReadInputPin( SrcUpLimit))
					{
						I2Csw_readFromMemoryByte1(&I2Csw_Gpio,
																			DS3231_devAdr,
																			0,
																			DS3231_reg,
																			4);
						SoftTimer_CMD(&softTimerUpdate,
											ENABLE);
					}
					else
					{
						I2Csw_sendToMemoryByte1(&I2Csw_Gpio,
																		DS3231_devAdr,
																		0,
																		DS3231_reg,
																		4);
						//Отключаем обновление экрана
						SoftTimer_CMD(&softTimerUpdate,
										DISABLE);
						// Останавливаем монитор с символом :
						timerUpdateDisplay=0;				
					}
				
					sliderTimer=sliderTimerRun;
					break;
				case sliderTimerHold:
						lbl_timerHold:
						DS3231_reg[0]=0; // секунды
						DS3231_reg[1]=0; // минуты 
						DS3231_reg[2]=0; // часы
						DS3231_reg[3]=1; // дни 
						I2Csw_sendToMemoryByte1(&I2Csw_Gpio,
																		DS3231_devAdr,
																		0,
																		DS3231_reg,
																		4);
					
						sliderTimer=sliderTimerRun;
						break;
				case sliderTimerInit:
					// добавить переход для проверки состояния источника
					if ( !GPIO_ReadInputPin( SrcUpLimit))
					{
						goto lbl_timerRun;
					}
					else
					{
						goto lbl_timerHold;
					}					
					break;		
			}
		}
		else
		if ( !GPIO_ReadInputPin(SliderCloseLimit) )
		{
			switch(sliderTimer)
			{
				case sliderTimerHold:
					break;
				case sliderTimerStop:		
				case sliderTimerInit:
				case sliderTimerRun:
					sliderTimer=sliderTimerHold;
					SoftTimer_CMD(&softTimerUpdate,
										DISABLE);
					timerUpdateDisplay=0;	
					break;	
			}
		}
		else
		{
			switch(sliderTimer)
			{
				case sliderTimerStop:
					//break;	
				
				case sliderTimerHold:	
					break;
					
				case sliderTimerRun:
					sliderTimer=sliderTimerStop;
					SoftTimer_CMD(&softTimerUpdate,
										DISABLE);
					timerUpdateDisplay=0;	
					break;
				
				case sliderTimerInit:
					
					if ( !GPIO_ReadInputPin( SrcUpLimit))
					{
						sliderTimer=sliderTimerStop;
					}
					else
					{
						sliderTimer=sliderTimerHold;
					}
					SoftTimer_CMD(&softTimerUpdate,
										DISABLE);
					timerUpdateDisplay=0;	
					break;
			}
			
		}
		// Конец обновления таймера
		//-------------------------------
		//================================
		//	

		if ( !GPIO_ReadInputPin( StopBtn))
		{
			nop();
			
			GPIO_WriteLow(enStep1);
			GPIO_WriteLow(enStep2);
			GPIO_WriteLow(enStepSrc_3);
			while(1);
		}
	
		
		if ( !GPIO_ReadInputPin( AlmStep))
		{
			// состояние ошибки
			nop();
			// отключение двигателя
			GPIO_WriteLow(enStep1);
			SoftTimer_CMD(&softTimerBlinkError,ENABLE);
			// переключаемся в режим сброса координат
			
		}
		else
		{
			nop();
			// включение двигателя
			GPIO_WriteHigh(enStep1);
			//GPIO_WriteHigh(enStepSrc_3);
		}
		
		// Конец автомата управления источником
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
