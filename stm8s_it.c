/**
  ******************************************************************************
  * @file    stm8s_it.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service 
  *          routine.
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
//#include "stm8s.h"
#include "stm8s_it.h"

/** @addtogroup Template_Project
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __OSA__ 
#include "osa.c"
#endif

#ifdef __STM8S_ADC1_H 
#include "src/stm8s_adc1.c"
#endif
#ifdef __STM8S_CLK_H
#include "src/stm8s_clk.c" // ������� ������������
#endif
#ifdef __STM8S_EXTI_H
#include "src/stm8s_exti.c" // ������� ������� ����������
#endif
#ifdef __STM8S_FLASH_H
#include "src/stm8s_flash.c" // ������� ������ �� ����
#endif
#ifdef __STM8S_GPIO_H
#include "src/stm8s_gpio.c" // ������� ��� GPIO
#endif
#ifdef __STM8S_I2C_H
#include "src/stm8s_i2c.c" // ������� ��� ���� I2C
#endif
#ifdef __STM8S_ITC_H
#include "src/stm8s_itc.c"  //
#endif
#ifdef __STM8S_IWDG_H
#include "src/stm8s_iwdg.c"
#endif
#ifdef __STM8S_RST_H
#include "src/stm8s_rst.c"
#endif
#ifdef __STM8S_SPI_H
#include "src/stm8s_spi.c"  
#endif
#ifdef __STM8S_TIM1_H
#include "src/stm8s_tim1.c"
#endif
#ifdef __STM8S_ADC2_H
#include "src/stm8s_adc2.c" // ������� ��� ���2
#endif
#ifdef __STM8S_AWU_H
#include "src/stm8s_awu.c"
#endif
#ifdef __STM8S_BEEP_H
#include "src/stm8s_beep.c"
#endif
#ifdef __STM8S_CAN_H
#include "src/stm8s_can.c"
#endif
#ifdef __STM8S_TIM2_H
#include "src/stm8s_tim2.c"
#endif
#ifdef __STM8S_TIM3_H
#include "src/stm8s_tim3.c"
#endif
#ifdef __STM8S_TIM4_H
#include "src/stm8s_tim4.c"
#endif
#ifdef __STM8S_TIM5_H
#include "src/stm8s_tim5.c"
#endif
#ifdef __STM8S_TIM6_H
#include "src/stm8s_tim6.c"
#endif
#ifdef __STM8S_UART1_H
#include "src/stm8s_uart1.c"
#endif
#ifdef __STM8S_UART2_H
#include "src/stm8s_uart2.c"
#endif
#ifdef __STM8S_UART3_H
#include "src/stm8s_uart3.c"
#endif
#ifdef __STM8S_WWDG_H
#include "src/stm8s_wwdg.c"
#endif
#ifdef __STM8S_DELAY_H
#include "src/stm8s_delay.c"
#endif
#ifdef __STM8S_I2CLCD1602_H
#include "src/stm8s_i2clcd1602.c"
#endif
#ifdef __STM8S_ENCODER_H
#include "src/stm8s_encoder.c"
#endif
#ifdef __STM8S_BUTTON_H
#include "src/stm8s_button.c"
#endif
#ifdef __STM8S_BME280_H
#include "src/stm8s_bme280.c"
#endif
#ifdef __STM8S_BUFFER_H
#include "src/stm8s_buffer.c"
#endif
#ifdef __STM8S_CMDLINE_H
#include "src/stm8s_cmdline.c"
#endif
#ifdef __STM8S_STDIOINIT_H
#include "src/stm8s_stdioinit.c"
#endif


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void baraban_tim3 (void);
void privodlineTim2(void);
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy Interrupt routine
  * @par Parameters:
  * None
  * @retval
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt, 25)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}
#endif /*_COSMIC_*/

/**
  * @brief TRAP Interrupt routine
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Top Level Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TLI_IRQHandler, 0)

{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Auto Wake Up Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(AWU_IRQHandler, 1)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Clock Controller Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(CLK_IRQHandler, 2)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTA Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTA_IRQHandler, 3)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTB Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTB_IRQHandler, 4)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTC_IRQHandler, 5)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief External Interrupt PORTE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI_PORTE_IRQHandler, 7)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x) 
/**
  * @brief External Interrupt PORTF Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(EXTI_PORTF_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
/**
  * @brief CAN RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_RX_IRQHandler, 8)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

/**
  * @brief CAN TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(CAN_TX_IRQHandler, 9)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8AF52Ax) */

/**
  * @brief SPI Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI_IRQHandler, 10)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @brief Timer1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CAP_COM_IRQHandler, 12)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer5 Update/Overflow/Break/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_UPD_OVF_BRK_TRG_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
 
/**
  * @brief Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM5_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }

#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
/**
  * @brief Timer2 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
	
 @svlreg INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
	privodlineTim2();
	
	 
 }

/**
  * @brief Timer2 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM2_CAP_COM_IRQHandler, 14)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8AF626x)
/**
  * @brief Timer3 Update/Overflow/Break Interrupt routine.
  * @param  None
  * @retval None
  */
 

 INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
	baraban_tim3 ();
	
 }

/**
  * @brief Timer3 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(TIM3_CAP_COM_IRQHandler, 16)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined (STM8S003) || defined(STM8S001) || defined (STM8AF62Ax) || defined (STM8AF52Ax) || defined (STM8S903)
/**
  * @brief UART1 TX Interrupt routine.
  * @param  None
  * @retval None
  */
	
	
 INTERRUPT_HANDLER(UART1_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART1 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S001) || (STM8S903) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8AF622x)
/**
  * @brief UART4 TX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_TX_IRQHandler, 17)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART4 RX Interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART4_RX_IRQHandler, 18)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
		
		
 }
#endif /* (STM8AF622x) */

/**
  * @brief I2C Interrupt routine.
  * @param  None
  * @retval None
  */
	//volatile uint8_t data, address=0b01111110;
	
	//I2CEventBit_t event;
//	uint8_t d;
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
	#ifdef __STM8S_I2C_H 
	byte16_t evt;
	evt.bytelow = I2C->SR1;
	evt.bytehigh = I2C->SR3;
	i2c_Task.ItEvent=evt.byte16;
	i2c_Task.Error=I2C->SR2;
	switch(i2c_Task.Error)
	{
		case 0:
	
	switch(i2c_Task.ItEvent)
	{
		//case I2C_START_FLAG:
		case I2C_EVENT_MASTER_MODE_SELECT: /*!< BUSY, MSL and SB flag */ 
		{	
				switch(i2c_Task.Dir)
				{
					case I2C_DIRECTION_TX:
					{	
						I2C->DR=i2c_Task.DeviceAddrRW;
					}
					break;
					case I2C_DIRECTION_RX:
					{
						switch(i2c_Task.NumSendReceive)
						{
							case 0: 
								i2c_Task.Dir=0;
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->stop=1;
								break;
							case 1:
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->ack=0;
								break;
							default:
								I2C->DR=i2c_Task.DeviceAddrRW;
								I2C->ack=1;
								break;
						}
					}
					break;
				}
		}
		break;
		
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: //!< BUSY, MSL, TRA, ADDR, TXE  				flags 
		//
		case I2C_EVENT_MASTER_BYTE_TRANSMITTING:	//!< 				BUSY, MSL, TRA, 			TXE 				flags 
		//
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:	//!< EV8_2: 	BUSY, MSL, TRA, 			TXE and BTF flags 
		{
			switch(i2c_Task.Func)
			{
				case i2cSendSend:
				{	
					switch(i2c_Task.NumSend)
					{
						case 0:
						{	
							switch(i2c_Task.NumSendReceive)
							{
								case 0:
								{
									I2C->stop=1;
									i2c_Task.Func=i2cIdle;
									i2c_Task.CurrentIndex=0;
								}
								break;
								case 1:
								{
									I2C->DR=i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++];
									i2c_Task.NumSendReceive--;
									i2c_Task.CurrentIndex=0;
								}
								break;
								default:
								{
									I2C->DR=i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++];
									i2c_Task.NumSendReceive--;
								}
								break;
							}
						}
						break;
						case 1:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
							i2c_Task.CurrentIndex=0;
						}
						break;
						default:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
						}
						break;
					}
				}
				break;
				// ��������� ������� ��������� � �������
				case i2cSendReceive:
				{
					switch(i2c_Task.NumSend)
					{
						case 0:
						{
							switch(i2c_Task.NumSendReceive)
							{
								case 0:
								{
									I2C->stop=1;
									i2c_Task.Func=i2cIdle;
									i2c_Task.CurrentIndex=0;
								}
								break;
								default:
								{
									i2c_Task.Dir=I2C_DIRECTION_RX;
									I2C->start=1;  // ��������� ������
								}
							}
						}
						break;
						case 1:
						{
							I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
							i2c_Task.NumSend--;
							i2c_Task.CurrentIndex=0;
						}
						break;
						default:
						I2C->DR=i2c_Task.ArraySend[i2c_Task.CurrentIndex++];
						i2c_Task.NumSend--;
					}
				}
				break;
			};
		}
		break;
		
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:	/*!< BUSY, MSL and ADDR						flags */
		{
			switch(i2c_Task.NumSendReceive)
			{
				case 1:
				I2C->ack=0;
				I2C->stop=1;
				break;
				default:
				I2C->ack=1;
			}
		}
		break;
		
		case I2C_EVENT_MASTER_BYTE_RECEIVED:					/*!< BUSY, MSL and 			RXNE 			flags */
		case I2C_EVENT_MASTER_2_BYTE_RECEIVED:				/*!< BUSY, MSL and 			RXNE BTF 	flags */
		{
			switch(i2c_Task.NumSendReceive)
			{
				
				case 1:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex]=I2C->DR;
					i2c_Task.NumSendReceive--;
					i2c_Task.Func=i2cIdle;
					i2c_Task.CurrentIndex=0;
				}
				break;
				case 2:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++]=I2C->DR;
					i2c_Task.NumSendReceive--;
					I2C->ack=0;
					I2C->stop=1;
				}
				break;
				default:
				{
					i2c_Task.ArrSendReceive[i2c_Task.CurrentIndex++]=I2C->DR;
					i2c_Task.NumSendReceive--;
					I2C->ack=1;
				}
				break;
			}
		}
		break;
		//case I2C_EVENT_SLAVE_ACK_FAILURE:
		case I2C_RXNE_FLAG: // rxne flag
			I2C->rxne=0;
		break;
		//case I2C_START_FLAG: // start flag
		//	I2C->sb=0;
		//break;
		
		
		case 0x300:	/*!< BUSY, MSL 	flags */
		{ 
			I2C->stop=1;
			i2c_Task.Func=i2cIdle;
			i2c_Task.Error=I2C->SR2; 
			I2C->SR2=0;
		}
		
		break;
	}	
	break;
	case 0b001: // ERROR BUS
	case 0b100: // ERROR ASK lost
			I2C->stop=1;
			i2c_Task.Func=i2cIdle;
			i2c_Task.Error=I2C->SR2; 
			I2C->SR2=0;
	break;
	}	
	#endif
}

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
/**
  * @brief UART2 TX interrupt routine.
  * @param  None
  * @retval None
  */
void	uart2_GetFromBuffer(void);
 INTERRUPT_HANDLER(UART2_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
		#ifdef __STM8S_UART2_H
		uart2_GetFromBuffer();
		#endif
 }

/**
  * @brief UART2 RX interrupt routine.
  * @param  None
  * @retval None
  */
	#ifdef __STM8S_UART2_H
extern inputchar_t UART2_RxFunc; // ������ �� ���������� � ����� uart2.c
#endif
 INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
	#ifdef __STM8S_UART2_H
		if (UART2_RxFunc) 
			UART2_RxFunc((char)UART2->DR);
		else 
		{	
			UART2_ClearITPendingBit(UART2_IT_RXNE); 
		}
	#endif
 }
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief UART3 TX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_TX_IRQHandler, 20)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }

/**
  * @brief UART3 RX interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(UART3_RX_IRQHandler, 21)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
/**
  * @brief ADC2 interrupt routine.
  * @param  None
  * @retval None
  */
 INTERRUPT_HANDLER(ADC2_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#else /* STM8S105 or STM8S103 or STM8S903 or STM8AF626x or STM8AF622x */
/**
  * @brief ADC1 interrupt routine.
  * @par Parameters:
  * None
  * @retval 
  * None
  */
 INTERRUPT_HANDLER(ADC1_IRQHandler, 22)
 {
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
 }
#endif /* (STM8S208) || (STM8S207) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8S903) || defined (STM8AF622x)
/**
  * @brief Timer6 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM6_UPD_OVF_TRG_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }
#else /* STM8S208 or STM8S207 or STM8S105 or STM8S103 or STM8AF52Ax or STM8AF62Ax or STM8AF626x */
/**
  * @brief Timer4 Update/Overflow Interrupt routine.
  * @param  None
  * @retval None
  */


 INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
	//------


	#ifdef __OSA__ 
	OS_Timer();
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	#endif
	
 }
#endif /* (STM8S903) || (STM8AF622x)*/

/**
  * @brief Eeprom EEC Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EEPROM_EEC_IRQHandler, 24)
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
}

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/