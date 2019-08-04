/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_ENCODER_H
#define __STM8S_ENCODER_H
#include "stm8s.h"
#include "inc/stm8s_gpio.h"

typedef struct
{
	uint8_t p1_now:1; // уровень текущий дл€ линии 1
	uint8_t p1_last:1;// уровень линии предыдуший дл€ линии 1 
	uint8_t p1_front:1;// наличие фрона дл€ линии 1
	uint8_t p2_now:1;  // уровень текущий дл€ линии 1
	uint8_t p2_last:1;// уровень линии предыдуший дл€ линии 1 
	uint8_t p2_front:1;// уровень текущий дл€ линии 1
	GPIO_TypeDef* p1_gpio;
	GPIO_Pin_TypeDef p1_pin;
	GPIO_TypeDef* p2_gpio;
	GPIO_Pin_TypeDef p2_pin;
	int32_t cnt; // значение энкодера
	int32_t boost_max; // максимальное увеличение счетчика при периоде 1
	uint16_t period_max; // максимальный период уменьшени€ значени€ 
	uint16_t period; // прошедший период с прошлого изменени€ или щелчка
} encoder_t;
//-----------------------------------------
/**
  * @brief  »нициализаци€ энкодера
  * @param   ¬ыбор порта дл€ линии 1
  * @param   ¬ыбор пина дл€ линии 1
  * @param   TIM2_Pulse specifies the Pulse width  value.
  * @param   TIM2_OCPolarity specifies the Output Compare Polarity  from @ref TIM2_OCPolarity_TypeDef.
  * @retval None
  */
void EncoderInit(encoder_t* encod, 
										GPIO_TypeDef* p1_Gpio, GPIO_Pin_TypeDef p1_Pin,
										GPIO_TypeDef* p2_Gpio, GPIO_Pin_TypeDef p2_Pin,
										int32_t cnt,int32_t boost_max,uint16_t period_max );
int32_t EncoderRead(encoder_t* encod);										

#endif