/**
  ******************************************************************************
  * @file    stm8s_i2c.c
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   This file contains all the functions for the I2C peripheral.
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
#include "inc/stm8s_i2c.h"

/** @addtogroup STM8S_StdPeriph_Driver
  * @{
  */
/* Private typedef -----------------------------------------------------------*/

/** @addtogroup I2C_Private_Defines
  * @{
  */
/* I2C register mask */
#define REGISTER_Mask               ((uint16_t)0x3000)
#define REGISTER_SR1_Index          ((uint16_t)0x0100)
#define REGISTER_SR2_Index          ((uint16_t)0x0200)
/* I2C Interrupt Enable mask */
#define ITEN_Mask                   ((uint16_t)0x0700)
/* I2C FLAG mask */
#define FLAG_Mask                   ((uint16_t)0x00FF)
/**
  * @}
  */

/* Private macro -------------------------------------------------------------*/
//#define OwnAddress 50
/* Private variables ---------------------------------------------------------*/
 i2cTask_t i2c_Task={0,0,0,0,0,0,0,0};
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @addtogroup I2C_Public_Functions
  * @{
  */

/**
  * @brief  Deinitializes the I2C peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void I2C_DeInit(void)
{
  /*
	I2C->CR1 = I2C_CR1_RESET_VALUE;
  I2C->CR2 = I2C_CR2_RESET_VALUE;
  I2C->FREQR = I2C_FREQR_RESET_VALUE;
  I2C->OARL = I2C_OARL_RESET_VALUE;
  I2C->OARH = I2C_OARH_RESET_VALUE;
  I2C->ITR = I2C_ITR_RESET_VALUE;
  I2C->CCRL = I2C_CCRL_RESET_VALUE;
  I2C->CCRH = I2C_CCRH_RESET_VALUE;
  I2C->TRISER = I2C_TRISER_RESET_VALUE;
	*/
	I2C->swrst=1;
	nop();
	I2C->swrst=0;
}

/**
  * @brief  Initializes the I2C according to the specified parameters in standard
  *         or fast mode.
  * @param  OutputClockFrequencyHz : Specifies the output clock frequency in Hz.
  * @param  OwnAddress : Specifies the own address.
  * @param  I2C_DutyCycle : Specifies the duty cycle to apply in fast mode.
  *         This parameter can be any of the  @ref I2C_DutyCycle_TypeDef enumeration.
  * @note   This parameter don't have impact when the OutputClockFrequency lower
  *         than 100KHz.
  * @param  Ack : Specifies the acknowledge mode to apply.
  *         This parameter can be any of the  @ref I2C_Ack_TypeDef enumeration.
  * @param  AddMode : Specifies the acknowledge address to apply.
  *         This parameter can be any of the  @ref I2C_AddMode_TypeDef enumeration.
  * @param  InputClockFrequencyMHz : Specifies the input clock frequency in MHz.
  * @retval None
  */
	
void I2C_Init_7bit(uint32_t OutputClockFrequencyHz)
{
  byte16_t result3, result25;
  CCRH_t tmpCCRH;
	uint32_t Clock;
	uint8_t ClockMHz;
  /* Check the parameters */
  assert_param(IS_I2C_OUTPUT_CLOCK_FREQ_OK(OutputClockFrequencyHz));
	// Enable Peripheral
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	// Get Clock
	Clock= CLK_GetClockFreq();
	ClockMHz=Clock/1000000;
	// Reset I2C
	I2C->swrst=1;
	nop();
	I2C->swrst=0;
	/*
	/*						
	/*------------------------- I2C FREQ Configuration ------------------------*/
  /* Write new value */
  I2C->freq = (uint8_t)(ClockMHz);

  /*--------------------------- I2C CCR Configuration ------------------------*/
  /* Disable I2C to configure TRISER */
  I2C->pe= (uint8_t)0;
  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
	if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpCCRH.ccrh=1<<7;
    result3.byte16  = (uint16_t) ((Clock ) / (OutputClockFrequencyHz * 3));
		result25.byte16 = (uint16_t) ((Clock ) / (OutputClockFrequencyHz * 25));
		if (result3.byte16*3<result25.byte16*25)
		{
			// Set Tlow/Thigh=16/9 
			tmpCCRH.duty=1;
		}
		else
		{
			result25.byte16=result3.byte16;
		}
		// Verify and correct CCR value if below minimum value 
    if (result25.byte16 < (uint16_t)0x01)
    {
      // Set the minimum allowed value 
      result25.byte16 = (uint16_t)0x0001;
    }
		 /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
   I2C->TRISER = (((uint8_t)ClockMHz * 3) / 10) + 1;
	}
  else /* STANDARD MODE */
  {
    tmpCCRH.ccrh=0;
		/* Calculate standard mode speed */
    result25.byte16 = (uint16_t)((Clock ) / (OutputClockFrequencyHz << (uint8_t)1));
		
    /* Verify and correct CCR value if below minimum value */
    if (result25.byte16 < (uint16_t)0x0004)
    {
      /* Set the minimum allowed value */
      result25.byte16 = (uint16_t)0x0004;
    }
    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(ClockMHz + (uint8_t)1);
  }
  /* Write CCR with new calculated value */
  I2C->CCRL = result25.bytelow;
  tmpCCRH.ccr11_8=result25.bytehigh;
	I2C->CCRH =tmpCCRH.ccrh;
  
  /* Configure I2C acknowledgement */
	// I2C->ack=0;
	// I2C->pos=0;
  /*--------------------------- I2C OAR Configuration ------------------------*/
  //I2C->OARL = (uint8_t)(OwnAddress);
	I2C->OARH=1<<6; // enable 7bit Mode
	//I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	/* Enable I2C */
  I2C->pe=1;
}

/**
  * @brief  Enables or disables the I2C peripheral.
  * @param  NewState : Indicate the new I2C peripheral state.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
	
	
// Resrart I2C module
void I2C_restartInit(void)
{
	//uint8_t I2C_CR1,I2C_CR2,I2C_FREQR,	I2C_OARL,I2C_OARH,
	//I2C_ITR,I2C_CCRL,	I2C_CCRH,I2C_TRISER;
	uint8_t i,i2cSetting[9];
/* Disable I2C to configure TRISER */
  I2C->pe= (uint8_t)0;
	//-- Save config I2C
	for(i=0;i<5;i++)
	{
		i2cSetting[i]=*((uint8_t*)I2C+i);
	}
	for(i;i<9;i++)
	{
		i2cSetting[i]=*((uint8_t*)I2C+i+5);
	}
	//reset I2C
	I2C->swrst=1;
	nop();
	I2C->swrst=0;
	//-- Download config I2C
	for(i=0;i<5;i++)
	{
		*((uint8_t*)I2C+i)=i2cSetting[i];
	}
	for(i;i<9;i++)
	{
		*((uint8_t*)I2C+i+5)=i2cSetting[i];
	}
	// Enable I2C
	I2C->pe= (uint8_t)1;
}	
// Обработка ошибок
// Handle error I2C module
i2cStatus_t I2C_HandlerError(i2cStatusReg_t *sr)
{
	if (sr->berr)  //Error 
			{
			I2C->berr=0; // na?in oeaaa
			//i2cStack.busError=1;
			I2C_restartInit(); 
			//I2C->stop=1; // aaia?aoey STOP
			return i2cBusError;
			}
			if (sr->af)  //Error 
			{
			I2C->af=0;
			I2C->stop=1;
			return i2cNack;			
			}	
			if (sr->arlo)  //Error 
			{
			I2C->arlo=0;
			//I2C->stop=1;
			return i2cArbitLost;			
			}	
			if (sr->ovr)  //Error 
			{
				I2C->ovr=0;
				//I2C->stop=1;
				return i2cOverrun;			
			}	
			if (sr->wufh)  //Error 
			{
				I2C->wufh=0;
				//I2C->stop=1;
				return i2cWakeup;			
			}	
			return i2cBusError;
}
	


	
void I2C_Cmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
  I2C->pe=NewState;
}

/**
  * @brief  Enables or disables the I2C General Call feature.
  * @param  NewState : State of the General Call feature.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_GeneralCallCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));
	I2C->engc=NewState;
}


/**
  * @brief  Enables or disables I2C software reset.
  * @param  NewState : Specifies the new state of the I2C software reset.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_SoftwareResetCmd(void)
{
  I2C->swrst=1;
	nop();
	I2C->swrst=0;
}

/**
  * @brief  Enables or disables the I2C clock stretching.
  * @param  NewState : Specifies the new state of the I2C Clock stretching.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */

void I2C_StretchClockCmd(FunctionalState NewState)
{
  /* Check function parameters */
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Clock Stretching Enable */
    I2C->CR1 &= (uint8_t)(~I2C_CR1_NOSTRETCH);

  }
  else /* NewState == DISABLE */
  {
    /* Clock Stretching Disable (Slave mode) */
    I2C->CR1 |= I2C_CR1_NOSTRETCH;
  }
}

/**
  * @brief  Enables or disables the specified I2C interrupt.
  * @param  I2C_IT : Name of the interrupt to enable or disable.
  *         This parameter can be any of the  @ref I2C_IT_TypeDef enumeration.
  * @param  NewState : State of the interrupt to apply.
  *         This parameter can be any of the @ref FunctionalState enumeration.
  * @retval None
  */
void I2C_ITConfig(I2C_IT_TypeDef I2C_IT, FunctionalState NewState)
{
  /* Check functions parameters */
  assert_param(IS_I2C_INTERRUPT_OK(I2C_IT));
  assert_param(IS_FUNCTIONALSTATE_OK(NewState));

  if (NewState != DISABLE)
  {
    /* Enable the selected I2C interrupts */
    I2C->ITR |= (uint8_t)I2C_IT;
  }
  else /* NewState == DISABLE */
  {
    /* Disable the selected I2C interrupts */
    I2C->ITR &= (uint8_t)(~(uint8_t)I2C_IT);
  }
}

/**
  * @brief  Selects the specified I2C fast mode duty cycle.
  * @param  I2C_DutyCycle : Specifies the duty cycle to apply.
  *         This parameter can be any of the @ref I2C_DutyCycle_TypeDef enumeration.
  * @retval None
  */
void I2C_FastModeDutyCycleConfig(I2C_DutyCycle_TypeDef I2C_DutyCycle)
{
  /* Check function parameters */
  assert_param(IS_I2C_DUTYCYCLE_OK(I2C_DutyCycle));

  if (I2C_DutyCycle == I2C_DUTYCYCLE_16_9)
  {
    /* I2C fast mode Tlow/Thigh = 16/9 */
    I2C->CCRH |= I2C_CCRH_DUTY;
  }
  else /* I2C_DUTYCYCLE_2 */
  {
    /* I2C fast mode Tlow/Thigh = 2 */
    I2C->CCRH &= (uint8_t)(~I2C_CCRH_DUTY);
  }
}



i2cStatus_t I2C_MasterSend(uint8_t DeviceAddress, void *ArraySend, uint16_t NumSend)
{
	uint16_t i=0;
	//uint8_t stBit=0;
	i2cStatusReg_t SR;
	uint8_t *ArSend=(uint8_t *)ArraySend;
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			I2C->start=1;  // generation start-bit
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
	//i=0;
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//---------
		//Begin Handle Error
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		//End Handle Error
		//------------
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1;
			i=0;
			continue;
		}
		
		
		if (SR.addr)
		{
			if (NumSend)
			{
				I2C->DR=*ArSend++;
				NumSend--;
			}
			else
			{
				I2C->stop=1;
				return i2cSuccess;
			}
			i=0;
			continue;
		}
		//-------------
		if (SR.txe)
		{
			if (NumSend)
			{
				I2C->DR=*ArSend++;
				//I2C->ack=1;
				NumSend--;
				i=0;
			}
			else
			{
				if (SR.btf)
				{
					I2C->stop=1;
					return i2cSuccess;
				}
			}
			continue;
		}
		//Begin Safe block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		//End Safe block
	}
	//----------------
}

i2cStatus_t I2C_MasterReceive(uint8_t DeviceAddress, void *ArrayReceive, uint16_t NumReceive)
{
	uint16_t i=0;
	i2cStatusReg_t SR;
	uint8_t *ArReceive=(uint8_t *)ArrayReceive;
	//I2C->start=1;  // Генерация старта
	//if (!SR.busy )
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			I2C->start=1;  // generation start-bit
		}
		else
		{
			//if (SR.msl||SR.busy)
					I2C_restartInit(); 
			return i2cBusy;
		}
	
	
	//i=0;
	while(1)
	{
		//repeat_busy:
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		
		
		if (SR.SR2)
		{
			//HandlerErrorI2CReceive:
			return I2C_HandlerError(&SR);
		}
		//---Send adress-----
		if (SR.sb) 
		{
			//I2C->DR=DeviceAddress<<1|1;
			switch(NumReceive)
			{
				case 0:
					//I2C->DR=DeviceAddress<<1|1;
					//I2C->stop=1;
					//break;
				case 1:
					I2C->DR=DeviceAddress<<1|1;
					I2C->ack=0;
					break;
				default:
					I2C->DR=DeviceAddress<<1|1;
					I2C->ack=1;
					break;
			}
			i=0;
			continue;
		}
		if (SR.addr)
		{
			switch(NumReceive)
			{
				case 0:
					//I2C->stop=1;
					//return i2cSuccess;
				case 1:
					I2C->stop=1;
					break;	
				default:
					I2C->ack=1;
			}
			i=0;
			continue;
		}
		
		if (SR.rxne)
		{
			switch(NumReceive)
			{
				case 0:
					return i2cSuccess;
				case 1:
					*ArReceive=I2C->DR;
					return i2cSuccess;
				case 2:
					*ArReceive++=I2C->DR;
					I2C->ack=0;
					I2C->stop=1;
					NumReceive--;
					break;	
				default:
					*ArReceive++=I2C->DR;
					I2C->ack=1;
					NumReceive--;
					
			}
			i=0;
			continue;
		}
		
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		
	}
	//----------------	
	
		
}

i2cStatus_t I2C_MasterSendPtrSendData(uint8_t DeviceAddress,void *Ptr, uint16_t PtrSize, void *ArraySend, uint16_t NumSend)
{
	uint16_t i=0;
	i2cStatusReg_t SR;
	uint8_t *ArSend=(uint8_t *)Ptr;
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			I2C->start=1;  // generation start-bit
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
	//------------	
	//Send Adr, Ptr
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//---------
		//Begin Handle Error
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		//End Handle Error
		//------------
		//---Begin Send Adress-----
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1;
			if (!PtrSize)
				{
					goto SendData1;
				}
			continue;
		}
		//---End Send adress-----
		//----------------------
		// 
		if (SR.addr)
		{
			I2C->DR=*ArSend++;
			PtrSize--;
			if (!PtrSize)
			{
				goto SendData1;
			}
			continue;
		}
		//-------------
		if (SR.txe)
		{
			I2C->DR=*ArSend++;
			PtrSize--;
			i=0;
			if (!PtrSize)
			{
				goto SendData1;
			}
			continue;
		}
		//Begin Safe block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		//End Safe block
	}
	SendData1:
	i=0;
	ArSend=(uint8_t *)ArraySend;
	
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//---------
		//Begin Handle Error
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		//End Handle Error
		//------------
		if (SR.addr)
		{
			if (NumSend)
			{
				I2C->DR=*ArSend++;
				NumSend--;
			}
			else
			{
				I2C->stop=1;
				return i2cSuccess;
			}
			i=0;
			continue;
		}
		//-------------
		if (SR.txe)
		{
			if (NumSend)
			{
				I2C->DR=*ArSend++;
				//I2C->ack=1;
				NumSend--;
				i=0;
			}
			else
			{
				if (SR.btf)
				{
					I2C->stop=1;
					return i2cSuccess;
				}
			}
			
			continue;
		}
		//Begin Safe block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		//End Safe block
	}
		
}


i2cStatus_t I2C_MasterSendPtrReceiveData(uint8_t DeviceAddress,void *Ptr, uint16_t PtrSize, void *ArrayReceive, uint16_t NumReceive)
{
	uint16_t i=0;
	i2cStatusReg_t SR;
	uint8_t *ArSend=(uint8_t *)Ptr;
	//------------------------
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//-------------------------
		// Begin Error I2C
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		// End Error I2C
		//----------------------
		if (!SR.busy)
		{
			if (PtrSize)
				{
					I2C->start=1;  // generation start-bit
				}
				else
				{
					goto ReceiveData1;
				}
			
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
	//------------	
	//Send Adr, Ptr
	while(1)
	{
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		//---------
		//Begin Handle Error
		if (SR.SR2)
		{
			return I2C_HandlerError(&SR);
		}
		//End Handle Error
		//------------
		//---Begin Send Adress-----
		if (SR.sb) 
		{
			I2C->DR=DeviceAddress<<1;
			continue;
		}
		//---End Send adress-----
		//----------------------
		// 
		if (SR.addr)
		{
			I2C->DR=*ArSend++;
			PtrSize--;
			if (!PtrSize)
			{
				goto ReceiveData1;
			}
			continue;
		}
		//-------------
		if (SR.txe)
		{
			
			if (PtrSize)
			{
				I2C->DR=*ArSend++;
				PtrSize--;
			}
			else
			{
				if (SR.btf)
					goto ReceiveData1;
			}
			i=0;
			continue;
		}
		//Begin Safe block
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		//End Safe block
	}
	ReceiveData1:
	I2C->start=1;  // generation start-bit
	i=0;
	ArSend=(uint8_t *)ArrayReceive;
	while(1)
	{
		//repeat_busy:
		SR.SR1=I2C->SR1;
		SR.SR3=I2C->SR3;
		SR.SR2=I2C->SR2;
		
		
		if (SR.SR2)
		{
			//HandlerErrorI2CReceive:
			return I2C_HandlerError(&SR);
		}
		//---Send adress-----
		if (SR.sb) 
		{
			switch(NumReceive)
			{
				case 0:
				case 1:
					I2C->DR=DeviceAddress<<1|1;
					I2C->ack=0;
					break;
				default:
					I2C->DR=DeviceAddress<<1|1;
					I2C->ack=1;
					break;
			}
			i=0;
			continue;
		}
		
		if (SR.addr)
		{
			switch(NumReceive)
			{
				case 0:
				case 1:
					I2C->stop=1;
					break;	
				default:
					I2C->ack=1;
			}
			i=0;
			continue;
		}
		
		if (SR.rxne)
		{
			switch(NumReceive)
			{
				case 0:
					return i2cSuccess;
				case 1:
					*ArSend=I2C->DR;
					return i2cSuccess;
				case 2:
					I2C->ack=0;
					*ArSend++=I2C->DR;
					I2C->stop=1;
					NumReceive--;
					break;	
				default:
					*ArSend++=I2C->DR;
					I2C->ack=1;
					NumReceive--;
					
			}
			i=0;
			continue;
		}
		
		if (i<10000) 
		{
			i++;
		}
		else
		{
			I2C_restartInit(); 
			return i2cBusy;
		}
		
	}
		
}


/*
// Функция передает в начале массив адреса регистра, а затем передает данные.
void I2C_MasterSendSend(uint8_t DeviceAddress, uint8_t *ArrayAddress, uint8_t NumAddress, uint8_t *ArraySend, uint8_t NumSend)
{

	while(i2c_Task.Func);
	i2c_Task.Func=i2cSendSend; // Загрузка функции
	i2c_Task.DeviceAddrRW=DeviceAddress<<1;//загрузка адреса
	i2c_Task.ArraySend=ArrayAddress; // загрузка указателя на адрес массива
	i2c_Task.NumSend=NumAddress;// загрузка кол-во байт для передачи
	i2c_Task.ArrSendReceive=ArraySend; // загрузка указателя массива на данные 
	i2c_Task.NumSendReceive=NumSend; // загрузка кол-во данных для передачи
	i2c_Task.CurrentIndex=0;
	i2c_Task.Error=0;
	//i2c_Task.ItEvent=I2C_GetLastEvent();
	I2C->start=1;  // Генерация старта
}

void I2C_MasterSendReceive(uint8_t DeviceAddress, uint8_t *ArrSend, uint8_t NumSend, uint8_t *ArrReceive, uint8_t NumReceive)
{

	
	while(i2c_Task.Func);
	i2c_Task.Func=i2cSendReceive; // Загрузка функции
	switch(NumSend)
	{
		case 0:
			i2c_Task.DeviceAddrRW=(DeviceAddress<<1)|1;//загрузка адреса на прием
		break;
		default:
			i2c_Task.DeviceAddrRW=(DeviceAddress<<1);//загрузка адреса на передачу
	}
	i2c_Task.ArraySend=ArrSend; // загрузка указателя на адрес массива
	i2c_Task.NumSend=NumSend;// загрузка кол-во байт для передачи
	i2c_Task.ArrSendReceive=ArrReceive; // загрузка указателя массива на данные 
	i2c_Task.NumSendReceive=NumReceive; // загрузка кол-во данных для передачи
	i2c_Task.Error=0;
	//i2c_Task.ItEvent=I2C_GetLastEvent();
	I2C->start=1;  // Генерация старта
}
*/

/*
void i2cHandler(void)
{
	
	//i2c_Task.ItEvent=I2C_GetLastEvent();
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
		case I2C_EVENT_MASTER_MODE_SELECT: // < BUSY, MSL and SB flag  
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
				// Обработка функции отправить и принять
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
									I2C->start=1;  // Генерация старта
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
		
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:	//< BUSY, MSL and ADDR						flags 
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
		
		case I2C_EVENT_MASTER_BYTE_RECEIVED:					//!< BUSY, MSL and 			RXNE 			flags 
		case I2C_EVENT_MASTER_2_BYTE_RECEIVED:				//!< BUSY, MSL and 			RXNE BTF 	flags 
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
		
		
		case 0x300:	//< BUSY, MSL 	flags 
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
}
*/

/**
 * @brief
 ****************************************************************************************
 *
 *                         I2C State Monitoring Functions
 *
 ****************************************************************************************
 * This I2C driver provides three different ways for I2C state monitoring
 *  depending on the application requirements and constraints:
 *
 *
 * 1) Basic state monitoring:
 *    Using I2C_CheckEvent() function:
 *    It compares the status registers (SR1, SR2 and SR3) content to a given event
 *    (can be the combination of one or more flags).
 *    It returns SUCCESS if the current status includes the given flags
 *    and returns ERROR if one or more flags are missing in the current status.
 *    - When to use:
 *      - This function is suitable for most applications as well as for startup
 *      activity since the events are fully described in the product reference manual
 *      (RM0016).
 *      - It is also suitable for users who need to define their own events.
 *    - Limitations:
 *      - If an error occurs (ie. error flags are set besides to the monitored flags),
 *        the I2C_CheckEvent() function may return SUCCESS despite the communication
 *        hold or corrupted real state.
 *        In this case, it is advised to use error interrupts to monitor the error
 *        events and handle them in the interrupt IRQ handler.
 *
 *        @note
 *        For error management, it is advised to use the following functions:
 *          - I2C_ITConfig() to configure and enable the error interrupts (I2C_IT_ERR).
 *          - I2C_IRQHandler() which is called when the I2C interrupts occur.
 *          - I2C_GetFlagStatus() or I2C_GetITStatus() to be called into the
 *           I2C_IRQHandler() function in order to determine which error occurred.
 *          - I2C_ClearFlag() or I2C_ClearITPendingBit() and/or I2C_SoftwareResetCmd()
 *            and/or I2C_GenerateStop() in order to clear the error flag and
 *            source and return to correct communication status.
 *
 *
 *  2) Advanced state monitoring:
 *     Using the function I2C_GetLastEvent() which returns the image of both SR1
 *     & SR3 status registers in a single word (uint16_t) (Status Register 3 value
 *     is shifted left by 8 bits and concatenated to Status Register 1).
 *     - When to use:
 *       - This function is suitable for the same applications above but it allows to
 *         overcome the limitations of I2C_GetFlagStatus() function (see below).
 *         The returned value could be compared to events already defined in the
 *         library (stm8s_i2c.h) or to custom values defined by user.
 *       - This function is suitable when multiple flags are monitored at the same time.
 *       - At the opposite of I2C_CheckEvent() function, this function allows user to
 *         choose when an event is accepted (when all events flags are set and no
 *         other flags are set or just when the needed flags are set like
 *         I2C_CheckEvent() function).
 *     - Limitations:
 *       - User may need to define his own events.
 *       - Same remark concerning the error management is applicable for this
 *         function if user decides to check only regular communication flags (and
 *         ignores error flags).
 *
 *
 *  3) Flag-based state monitoring:
 *     Using the function I2C_GetFlagStatus() which simply returns the status of
 *     one single flag (ie. I2C_FLAG_RXNE ...).
 *     - When to use:
 *        - This function could be used for specific applications or in debug phase.
 *        - It is suitable when only one flag checking is needed (most I2C events
 *          are monitored through multiple flags).
 *     - Limitations:
 *        - When calling this function, the Status register is accessed. Some flags are
 *          cleared when the status register is accessed. So checking the status
 *          of one Flag, may clear other ones.
 *        - Function may need to be called twice or more in order to monitor one
 *          single event.
 *
 *  For detailed description of Events, please refer to section I2C_Events in
 *  stm8s_i2c.h file.
 *
 */
/**
 *
 *  1) Basic state monitoring
 *******************************************************************************
 */


/**
 *
 *  2) Advanced state monitoring
 *******************************************************************************
 */
/**
  * @brief  Returns the last I2C Event.
  *
  * @note: For detailed description of Events, please refer to section
  *    I2C_Events in stm8s_i2c.h file.
  *
  * @retval The last event
  *   This parameter can be any of the  @ref I2C_Event_TypeDef enumeration.
  */
	/*
I2C_Event_TypeDef I2C_GetLastEvent(void)
{
<<<<<<< HEAD
  __IO uint16_t lastevent = 0;
 
 uint16_t flag1 = 0;
  uint16_t flag2 = 0;
	/*
  if ((I2C->SR2 & I2C_SR2_AF) != 0x00)
  {
    lastevent = I2C_EVENT_SLAVE_ACK_FAILURE;
  }
  else
 */
 /*
	{
    // Read the I2C status register 
		flag1 = I2C->SR1;
    flag2 = I2C->SR3;
    // Get the last event value from I2C status register 
    lastevent = ((uint16_t)((uint16_t)flag2 << 8) | (uint16_t)flag1);
  }
  // Return status 
  return (I2C_Event_TypeDef)lastevent;
=======
  byte16_t evt;
	evt.bytelow = I2C->SR1;
	evt.bytehigh = I2C->SR3;
  Return status 
  return (I2C_Event_TypeDef)evt.byte16;
>>>>>>> 8f48970bcb2974f538705e863d351115a376a7c6
}

*/

i2cFunc_t i2cCheckStatusTransfer(void)
{
	return  (i2c_Task.Func);
}

uint8_t i2cCheckErrorTransfer(void)
{
	return  (i2c_Task.Error);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
