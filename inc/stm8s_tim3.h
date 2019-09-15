/**
  ******************************************************************************
  * @file    stm8s_tim3.h
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief  This file contains all functions prototype and macros for the TIM3 peripheral.
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
#ifndef __STM8S_TIM3_H
#define __STM8S_TIM3_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/** @addtogroup TIM3_Exported_Types
  * @{
  */

/** TIM3 Forced Action */
typedef enum
{
  TIM3_FORCEDACTION_ACTIVE           = ((uint8_t)0x50),
  TIM3_FORCEDACTION_INACTIVE         = ((uint8_t)0x40)
} TIM3_ForcedAction_TypeDef;

#define IS_TIM3_FORCED_ACTION_OK(ACTION) (((ACTION) == TIM3_FORCEDACTION_ACTIVE) || \
    ((ACTION) == TIM3_FORCEDACTION_INACTIVE))

/** TIM3 Prescaler */
typedef enum
{
  TIM3_PRESCALER_1  = ((uint8_t)0x00),
  TIM3_PRESCALER_2    = ((uint8_t)0x01),
  TIM3_PRESCALER_4    = ((uint8_t)0x02),
  TIM3_PRESCALER_8     = ((uint8_t)0x03),
  TIM3_PRESCALER_16   = ((uint8_t)0x04),
  TIM3_PRESCALER_32     = ((uint8_t)0x05),
  TIM3_PRESCALER_64    = ((uint8_t)0x06),
  TIM3_PRESCALER_128   = ((uint8_t)0x07),
  TIM3_PRESCALER_256   = ((uint8_t)0x08),
  TIM3_PRESCALER_512   = ((uint8_t)0x09),
  TIM3_PRESCALER_1024  = ((uint8_t)0x0A),
  TIM3_PRESCALER_2048 = ((uint8_t)0x0B),
  TIM3_PRESCALER_4096   = ((uint8_t)0x0C),
  TIM3_PRESCALER_8192 = ((uint8_t)0x0D),
  TIM3_PRESCALER_16384 = ((uint8_t)0x0E),
  TIM3_PRESCALER_32768 = ((uint8_t)0x0F)
} TIM3_Prescaler_TypeDef;

#define IS_TIM3_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM3_PRESCALER_1  ) || \
    ((PRESCALER) == TIM3_PRESCALER_2    ) || \
    ((PRESCALER) == TIM3_PRESCALER_4    ) || \
    ((PRESCALER) == TIM3_PRESCALER_8  ) || \
    ((PRESCALER) == TIM3_PRESCALER_16   ) || \
    ((PRESCALER) == TIM3_PRESCALER_32     ) || \
    ((PRESCALER) == TIM3_PRESCALER_64    ) || \
    ((PRESCALER) == TIM3_PRESCALER_128   ) || \
    ((PRESCALER) == TIM3_PRESCALER_256   ) || \
    ((PRESCALER) == TIM3_PRESCALER_512   ) || \
    ((PRESCALER) == TIM3_PRESCALER_1024  ) || \
    ((PRESCALER) == TIM3_PRESCALER_2048 ) || \
    ((PRESCALER) == TIM3_PRESCALER_4096 ) || \
    ((PRESCALER) == TIM3_PRESCALER_8192 ) || \
    ((PRESCALER) == TIM3_PRESCALER_16384 ) || \
    ((PRESCALER) == TIM3_PRESCALER_32768 ))

/** TIM3 Output Compare and PWM modes */
typedef enum
{
  TIM3_OCMODE_TIMING     = ((uint8_t)0x00),
  TIM3_OCMODE_ACTIVE     = ((uint8_t)0x10),
  TIM3_OCMODE_INACTIVE   = ((uint8_t)0x20),
  TIM3_OCMODE_TOGGLE     = ((uint8_t)0x30),
  TIM3_OCMODE_PWM1       = ((uint8_t)0x60),
  TIM3_OCMODE_PWM2       = ((uint8_t)0x70)
} TIM3_OCMode_TypeDef;

#define IS_TIM3_OC_MODE_OK(MODE) (((MODE) ==  TIM3_OCMODE_TIMING) || \
                                  ((MODE) == TIM3_OCMODE_ACTIVE) || \
                                  ((MODE) == TIM3_OCMODE_INACTIVE) || \
                                  ((MODE) == TIM3_OCMODE_TOGGLE)|| \
                                  ((MODE) == TIM3_OCMODE_PWM1) || \
                                  ((MODE) == TIM3_OCMODE_PWM2))

#define IS_TIM3_OCM_OK(MODE)(((MODE) ==  TIM3_OCMODE_TIMING) || \
                             ((MODE) == TIM3_OCMODE_ACTIVE) || \
                             ((MODE) == TIM3_OCMODE_INACTIVE) || \
                             ((MODE) == TIM3_OCMODE_TOGGLE)|| \
                             ((MODE) == TIM3_OCMODE_PWM1) || \
                             ((MODE) == TIM3_OCMODE_PWM2) || \
                             ((MODE) == (uint8_t)TIM3_FORCEDACTION_ACTIVE) || \
                             ((MODE) == (uint8_t)TIM3_FORCEDACTION_INACTIVE))

/** TIM3 One Pulse Mode */
typedef enum
{
  TIM3_OPMODE_SINGLE                 = ((uint8_t)0x01),
  TIM3_OPMODE_REPETITIVE             = ((uint8_t)0x00)
} TIM3_OPMode_TypeDef;

#define IS_TIM3_OPM_MODE_OK(MODE) (((MODE) == TIM3_OPMODE_SINGLE) || \
                                   ((MODE) == TIM3_OPMODE_REPETITIVE))

/** TIM3 Channel */

typedef enum
{
  TIM3_CHANNEL_1                     = ((uint8_t)0x00),
  TIM3_CHANNEL_2                     = ((uint8_t)0x01)
} TIM3_Channel_TypeDef;

#define IS_TIM3_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM3_CHANNEL_1) || \
                                     ((CHANNEL) == TIM3_CHANNEL_2))

#define IS_TIM3_PWMI_CHANNEL_OK(CHANNEL) (((CHANNEL) == TIM3_CHANNEL_1) || \
    ((CHANNEL) == TIM3_CHANNEL_2))

/** TIM3 Output Compare Polarity */
typedef enum
{
  TIM3_OCPOLARITY_HIGH               = ((uint8_t)0x00),
  TIM3_OCPOLARITY_LOW                = ((uint8_t)0x22)
} TIM3_OCPolarity_TypeDef;

#define IS_TIM3_OC_POLARITY_OK(POLARITY) (((POLARITY) == TIM3_OCPOLARITY_HIGH) || \
    ((POLARITY) == TIM3_OCPOLARITY_LOW))

/** TIM3 Output Compare states */
typedef enum
{
  TIM3_OUTPUTSTATE_DISABLE           = ((uint8_t)0x00),
  TIM3_OUTPUTSTATE_ENABLE            = ((uint8_t)0x11)
} TIM3_OutputState_TypeDef;

#define IS_TIM3_OUTPUT_STATE_OK(STATE) (((STATE) == TIM3_OUTPUTSTATE_DISABLE) || \
                                        ((STATE) == TIM3_OUTPUTSTATE_ENABLE))

/** TIM3 Input Capture Polarity */
typedef enum
{
  TIM3_ICPOLARITY_RISING            = ((uint8_t)0x00),
  TIM3_ICPOLARITY_FALLING           = ((uint8_t)0x44)
} TIM3_ICPolarity_TypeDef;

#define IS_TIM3_IC_POLARITY_OK(POLARITY) (((POLARITY) == TIM3_ICPOLARITY_RISING) || \
    ((POLARITY) == TIM3_ICPOLARITY_FALLING))

/** TIM3 Input Capture Selection */
typedef enum
{
  TIM3_ICSELECTION_DIRECTTI          = ((uint8_t)0x01),
  TIM3_ICSELECTION_INDIRECTTI        = ((uint8_t)0x02),
  TIM3_ICSELECTION_TRGI              = ((uint8_t)0x03)
} TIM3_ICSelection_TypeDef;

#define IS_TIM3_IC_SELECTION_OK(SELECTION) (((SELECTION) == TIM3_ICSELECTION_DIRECTTI) || \
    ((SELECTION) == TIM3_ICSELECTION_INDIRECTTI) || \
    ((SELECTION) == TIM3_ICSELECTION_TRGI))

/** TIM3 Input Capture Prescaler */
typedef enum
{
  TIM3_ICPSC_DIV1                    = ((uint8_t)0x00),
  TIM3_ICPSC_DIV2                    = ((uint8_t)0x04),
  TIM3_ICPSC_DIV4                    = ((uint8_t)0x08),
  TIM3_ICPSC_DIV8                    = ((uint8_t)0x0C)
} TIM3_ICPSC_TypeDef;

#define IS_TIM3_IC_PRESCALER_OK(PRESCALER) (((PRESCALER) == TIM3_ICPSC_DIV1) || \
    ((PRESCALER) == TIM3_ICPSC_DIV2) || \
    ((PRESCALER) == TIM3_ICPSC_DIV4) || \
    ((PRESCALER) == TIM3_ICPSC_DIV8))

/** TIM3 Input Capture Filer Value */
#define IS_TIM3_IC_FILTER_OK(ICFILTER) ((ICFILTER) <= 0x0F)

/** TIM3 interrupt sources */
typedef enum
{
  TIM3_IT_UPDATE                     = ((uint8_t)0x01),
  TIM3_IT_CC1                        = ((uint8_t)0x02),
  TIM3_IT_CC2                        = ((uint8_t)0x04)
} TIM3_IT_TypeDef;

#define IS_TIM3_IT_OK(IT) (((IT) != 0x00) && ((IT) <= 0x07))

#define IS_TIM3_GET_IT_OK(IT) (((IT) == TIM3_IT_UPDATE) || \
                               ((IT) == TIM3_IT_CC1) || \
                               ((IT) == TIM3_IT_CC2))

/** TIM3 Prescaler Reload Mode */
typedef enum
{
  TIM3_PSCRELOADMODE_UPDATE          = ((uint8_t)0x00),
  TIM3_PSCRELOADMODE_IMMEDIATE       = ((uint8_t)0x01)
} TIM3_PSCReloadMode_TypeDef;

#define IS_TIM3_PRESCALER_RELOAD_OK(RELOAD) (((RELOAD) == TIM3_PSCRELOADMODE_UPDATE) || \
    ((RELOAD) == TIM3_PSCRELOADMODE_IMMEDIATE))

/** TIM3 Event Source */
typedef enum
{
  TIM3_EVENTSOURCE_UPDATE            = ((uint8_t)0x01),
  TIM3_EVENTSOURCE_CC1               = ((uint8_t)0x02),
  TIM3_EVENTSOURCE_CC2               = ((uint8_t)0x04)
} TIM3_EventSource_TypeDef;

#define IS_TIM3_EVENT_SOURCE_OK(SOURCE) (((SOURCE) != 0x00))

/** TIM3 Update Source */
typedef enum
{
  TIM3_UPDATESOURCE_GLOBAL           = ((uint8_t)0x00),
  TIM3_UPDATESOURCE_REGULAR          = ((uint8_t)0x01)
} TIM3_UpdateSource_TypeDef;

#define IS_TIM3_UPDATE_SOURCE_OK(SOURCE) (((SOURCE) == TIM3_UPDATESOURCE_GLOBAL) || \
    ((SOURCE) == TIM3_UPDATESOURCE_REGULAR))

/** TIM3 Flags */
typedef enum
{
  TIM3_FLAG_UPDATE                   = ((uint16_t)0x0001),
  TIM3_FLAG_CC1                      = ((uint16_t)0x0002),
  TIM3_FLAG_CC2                      = ((uint16_t)0x0004),
  TIM3_FLAG_CC1OF                    = ((uint16_t)0x0200),
  TIM3_FLAG_CC2OF                    = ((uint16_t)0x0400)
} TIM3_FLAG_TypeDef;

#define IS_TIM3_GET_FLAG_OK(FLAG) (((FLAG) == TIM3_FLAG_UPDATE) || \
                                   ((FLAG) == TIM3_FLAG_CC1) || \
                                   ((FLAG) == TIM3_FLAG_CC2) || \
                                   ((FLAG) == TIM3_FLAG_CC1OF) || \
                                   ((FLAG) == TIM3_FLAG_CC2OF) )

#define IS_TIM3_CLEAR_FLAG_OK(FLAG) ((((uint16_t)(FLAG) & 0xF9F8) == 0x0000) && ((uint16_t)(FLAG)!= 0x0000))

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM3_Exported_Functions
  * @{
  */
//#define TIM3_DeInit_Def
#ifdef TIM3_DeInit_Def
void TIM3_DeInit(void);
#endif

#define TIM3_TimeBaseInit_Def
#ifdef TIM3_TimeBaseInit_Def
void TIM3_TimeBaseInit(TIM3_Prescaler_TypeDef TIM3_Prescaler, uint16_t TIM3_Period);
#endif

#define TIM3_OC1Init_Def
#ifdef TIM3_OC1Init_Def
void TIM3_OC1Init(TIM3_OCMode_TypeDef TIM3_OCMode, TIM3_OutputState_TypeDef TIM3_OutputState, uint16_t TIM3_Pulse, TIM3_OCPolarity_TypeDef TIM3_OCPolarity);
#endif


#define TIM3_OC2Init_Def
#ifdef TIM3_OC2Init_Def
void TIM3_OC2Init(TIM3_OCMode_TypeDef TIM3_OCMode, TIM3_OutputState_TypeDef TIM3_OutputState, uint16_t TIM3_Pulse, TIM3_OCPolarity_TypeDef TIM3_OCPolarity);
#endif

#define TIM3_ICInit_Def
#ifdef TIM3_ICInit_Def
void TIM3_ICInit(TIM3_Channel_TypeDef TIM3_Channel, TIM3_ICPolarity_TypeDef TIM3_ICPolarity, TIM3_ICSelection_TypeDef TIM3_ICSelection,  TIM3_ICPSC_TypeDef TIM3_ICPrescaler, uint8_t TIM3_ICFilter);
#endif


#define TIM3_PWMIConfig_Def
#ifdef TIM3_PWMIConfig_Def
void TIM3_PWMIConfig(TIM3_Channel_TypeDef TIM3_Channel, TIM3_ICPolarity_TypeDef TIM3_ICPolarity, TIM3_ICSelection_TypeDef TIM3_ICSelection,  TIM3_ICPSC_TypeDef TIM3_ICPrescaler, uint8_t TIM3_ICFilter);
#endif

#define TIM3_Cmd_Def
#ifdef TIM3_Cmd_Def
void TIM3_Cmd(FunctionalState NewState);
#endif

#define TIM3_ITConfig_Def
#ifdef TIM3_ITConfig_Def
void TIM3_ITConfig(TIM3_IT_TypeDef TIM3_IT, FunctionalState NewState);
#endif

#define TIM3_InternalClockConfig_Def
#ifdef TIM3_InternalClockConfig_Def
void TIM3_InternalClockConfig(void);
#endif

#define TIM3_UpdateDisableConfig_Def
#ifdef TIM3_UpdateDisableConfig_Def
void TIM3_UpdateDisableConfig(FunctionalState NewState);
#endif

#define TIM3_UpdateRequestConfig_Def
#ifdef TIM3_UpdateRequestConfig_Def
void TIM3_UpdateRequestConfig(TIM3_UpdateSource_TypeDef TIM3_UpdateSource);
#endif

#define TIM3_SelectOnePulseMode_Def
#ifdef TIM3_SelectOnePulseMode_Def
void TIM3_SelectOnePulseMode(TIM3_OPMode_TypeDef TIM3_OPMode);
#endif

#define TIM3_PrescalerConfig_Def
#ifdef TIM3_PrescalerConfig_Def
void TIM3_PrescalerConfig(TIM3_Prescaler_TypeDef Prescaler, TIM3_PSCReloadMode_TypeDef TIM3_PSCReloadMode);
#endif

#define TIM3_ForcedOC1Config_Def
#ifdef TIM3_ForcedOC1Config_Def
void TIM3_ForcedOC1Config(TIM3_ForcedAction_TypeDef TIM3_ForcedAction);
#endif

#define TIM3_ForcedOC2Config_Def
#ifdef TIM3_ForcedOC2Config_Def
void TIM3_ForcedOC2Config(TIM3_ForcedAction_TypeDef TIM3_ForcedAction);
#endif

#define TIM3_ARRPreloadConfig_Def
#ifdef TIM3_ARRPreloadConfig_Def
void TIM3_ARRPreloadConfig(FunctionalState NewState);
#endif

#define TIM3_CCPreloadControl_Def
#ifdef TIM3_CCPreloadControl_Def
void TIM3_CCPreloadControl(FunctionalState NewState);
#endif

//#define TIM3_OC1PreloadConfig_Def
#ifdef TIM3_OC1PreloadConfig_Def
void TIM3_OC1PreloadConfig(FunctionalState NewState);
#endif

#define TIM3_OC2PreloadConfig_Def
#ifdef TIM3_OC2PreloadConfig_Def
void TIM3_OC2PreloadConfig(FunctionalState NewState);
#endif

//#define TIM3_GenerateEvent_Def
#ifdef TIM3_GenerateEvent_Def
void TIM3_GenerateEvent(TIM3_EventSource_TypeDef TIM3_EventSource);
#endif

//#define TIM3_OC1PolarityConfig_Def
#ifdef TIM3_OC1PolarityConfig_Def
void TIM3_OC1PolarityConfig(TIM3_OCPolarity_TypeDef TIM3_OCPolarity);
#endif

//#define TIM3_OC2PolarityConfig_Def
#ifdef TIM3_OC2PolarityConfig_Def
void TIM3_OC2PolarityConfig(TIM3_OCPolarity_TypeDef TIM3_OCPolarity);
#endif

//#define TIM3_CCxCmd_Def
#ifdef TIM3_CCxCmd_Def
void TIM3_CCxCmd(TIM3_Channel_TypeDef TIM3_Channel, FunctionalState NewState);
#endif

//#define TIM3_SelectOCxM_Def
#ifdef TIM3_SelectOCxM_Def
void TIM3_SelectOCxM(TIM3_Channel_TypeDef TIM3_Channel, TIM3_OCMode_TypeDef TIM3_OCMode);
#endif

#define TIM3_SetCounter_Def
#ifdef TIM3_SetCounter_Def
void TIM3_SetCounter(uint16_t Counter);
#endif

#define TIM3_SetAutoreload_Def
#ifdef TIM3_SetAutoreload_Def
void TIM3_SetAutoreload(uint16_t Autoreload);
#endif

//#define TIM3_SetCompare1_Def
#ifdef TIM3_SetCompare1_Def
void TIM3_SetCompare1(uint16_t Compare1);
#endif

//#define TIM3_SetCompare2_Def
#ifdef TIM3_SetCompare2_Def
void TIM3_SetCompare2(uint16_t Compare2);
#endif

//#define TIM3_SetIC1Prescaler_Def
#if defined(TIM3_SetIC1Prescaler_Def) ||defined(TIM3_ICInit_Def)
void TIM3_SetIC1Prescaler(TIM3_ICPSC_TypeDef TIM3_IC1Prescaler);
#endif

//#define TIM3_SetIC2Prescaler_Def
#if defined(TIM3_SetIC2Prescaler_Def) ||defined(TIM3_ICInit_Def)
void TIM3_SetIC2Prescaler(TIM3_ICPSC_TypeDef TIM3_IC2Prescaler);
#endif

//#define TIM3_GetCapture1_Def
#ifdef TIM3_GetCapture1_Def
uint16_t TIM3_GetCapture1(void);
#endif

//#define TIM3_GetCapture2_Def
#ifdef TIM3_GetCapture2_Def
uint16_t TIM3_GetCapture2(void);
#endif

//#define TIM3_GetCounter_Def
#ifdef TIM3_GetCounter_Def
uint16_t TIM3_GetCounter(void);
#endif

//#define TIM3_GetPrescaler_Def
#ifdef TIM3_GetPrescaler_Def
TIM3_Prescaler_TypeDef TIM3_GetPrescaler(void);
#endif

#define TIM3_GetFlagStatus_Def
#ifdef TIM3_GetFlagStatus_Def
FlagStatus TIM3_GetFlagStatus(TIM3_FLAG_TypeDef TIM3_FLAG);
#endif

#define TIM3_ClearFlag_Def
#ifdef TIM3_ClearFlag_Def
void TIM3_ClearFlag(TIM3_FLAG_TypeDef TIM3_FLAG);
#endif

//#define TIM3_GetITStatus_Def
#ifdef TIM3_GetITStatus_Def
ITStatus TIM3_GetITStatus(TIM3_IT_TypeDef TIM3_IT);
#endif

#define TIM3_ClearITPendingBit_Def
#ifdef TIM3_ClearITPendingBit_Def
void TIM3_ClearITPendingBit(TIM3_IT_TypeDef TIM3_IT);
#endif
/**
  * @}
  */

#endif /* __STM8S_TIM3_H */

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
