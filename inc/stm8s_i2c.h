/**
  ******************************************************************************
  * @file    stm8s_i2c.h
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief  This file contains all functions prototype and macros for the I2C peripheral.
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
#ifndef __STM8S_I2C_H
#define __STM8S_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "inc/stm8s_clk.h" // драйвер тактирования
/* Exported types ------------------------------------------------------------*/

/** @addtogroup I2C_Exported_Types
  * @{
  */

/**
  * @brief  I2C duty cycle (fast mode only)
  */
typedef enum
{
  I2C_DUTYCYCLE_2    = (uint8_t)0x00,  /*!< Fast mode Tlow/THigh = 2 */
  I2C_DUTYCYCLE_16_9 = (uint8_t)0x40   /*!< Fast mode Tlow/Thigh = 16/9 */
} I2C_DutyCycle_TypeDef;

/**
  * @brief  I2C Acknowledgement configuration
  */
typedef enum
{
  I2C_ACK_NONE = (uint8_t)0x00,  /*!< No acknowledge */
  I2C_ACK_CURR = (uint8_t)0x01,  /*!< Acknowledge on the current byte */
  I2C_ACK_NEXT = (uint8_t)0x02   /*!< Acknowledge on the next byte */
} I2C_Ack_TypeDef;

/**
  * @brief  I2C Addressing Mode (slave mode only)
  */
typedef enum
{
  I2C_ADDMODE_7BIT  = (uint8_t)0x00,  /*!< 7-bit slave address (10-bit address not acknowledged) */
  I2C_ADDMODE_10BIT = (uint8_t)0x80   /*!< 10-bit slave address (7-bit address not acknowledged) */
} I2C_AddMode_TypeDef;

/**
  * @brief  I2C Interrupt sources
  * Warning: the values correspond to the bit position in the ITR register
  */
typedef enum
{
    I2C_IT_ERR     = (uint8_t)0x01, 	/*!< Error Interruption */
    I2C_IT_EVT     = (uint8_t)0x02, 	/*!< Event Interruption */
    I2C_IT_BUF     = (uint8_t)0x04 	/*!< Buffer Interruption */
} I2C_IT_TypeDef;

/**
  * @brief  I2C transfer direction
  * Warning: the values correspond to the ADD0 bit position in the OARL register
  */
typedef enum
{
  I2C_DIRECTION_TX = (uint8_t)0x00,  /*!< Transmission direction */
  I2C_DIRECTION_RX = (uint8_t)0x01   /*!< Reception direction */
} I2C_Direction_TypeDef;
//------------------------
// Показывает какая функция работает
typedef enum
{
	i2cIdle=0,
	i2cSend=1,
	i2cSendSend=2,
	i2cReceive=3,
	i2cSendReceive=4
} i2cFunc_t;


 /*!< I2C status register 2 */
typedef  struct
{
	uint8_t berr:1; // buss error=1
	uint8_t arlo:1; // arbitration lost=1
	uint8_t af:1; // acknowledge failure=1
	uint8_t ovr:1; // overrun or underrun=1
	uint8_t reserv_sr2:1; // reserved
	uint8_t wufh:1;	// wakeup from Halt=1
	uint8_t reserv2_sr2:2;// reserved
}i2c_sr2bit_t;
	
typedef struct
{
  union
	{
		uint8_t SR1;       /*!< I2C status register 1 */
		struct
		{
			uint8_t sb:1; // start bit generation=1
			uint8_t addr:1; // addess sent=1 (master)/match=1 (slave)
			uint8_t btf:1;	// byte transfer=1
			uint8_t add10:1; // master has sent header 10-bit=1 address
			uint8_t stopf:1; //stop detection=1 (slave)
			uint8_t reserv_sr1:1; // reserved
			uint8_t rxne:1; //data register not empty=1 (receiver)
			uint8_t txe:1; // data register empty=1 (transmitters)
		};
	};
	union
	{
		uint8_t SR2;       /*!< I2C status register 2 */
		struct
		{
			uint8_t berr:1; // buss error=1
			uint8_t arlo:1; // arbitration lost=1
			uint8_t af:1; // acknowledge failure=1
			uint8_t ovr:1; // overrun or underrun=1
			uint8_t reserv_sr2:1; // reserved
			uint8_t wufh:1;	// wakeup from Halt=1
			uint8_t reserv2_sr2:2;// reserved
		};
	};
	union
	{
	uint8_t SR3;       /*!< I2C status register 3 */
	struct
		{
			uint8_t msl:1; // master=1 / slave=0
			uint8_t busy:1; //bus busy =1
			uint8_t tra:1; // transmitter=1/ reciever=0
			uint8_t reserv_sr3:1; // reserved
			uint8_t gencall:1;	// generall call header=1 (slave)
			uint8_t reserv2_sr3:2; // reserved
			uint8_t dualf:1; // Dual flag OAR2=0 OAR2=1 (slave)
		};
	};
}i2cStatusReg_t;

// Result Send or Receive
typedef enum
{
	i2cSuccess=0,
	i2cBusError=1,
	i2cArbitLost=2,
	i2cNack=3,
	i2cOverrun=4,
	i2cWakeup=5,
	i2cBusy=6
}i2cStatus_t;
//---------------
/**
  * @brief  I2C Flags
  * @brief Elements values convention: 0xXXYY
  *  X = SRx registers index
  *      X = 1 : SR1
  *      X = 2 : SR2
  *      X = 3 : SR3
  *  Y = Flag mask in the register
  */

typedef enum
{
  /* SR1 register flags */
  I2C_FLAG_TXEMPTY             = (uint16_t)0x0180,  /*!< Transmit Data Register Empty flag */
  I2C_FLAG_RXNOTEMPTY          = (uint16_t)0x0140,  /*!< Read Data Register Not Empty flag */
  I2C_FLAG_STOPDETECTION       = (uint16_t)0x0110,  /*!< Stop detected flag */
  I2C_FLAG_HEADERSENT          = (uint16_t)0x0108,  /*!< 10-bit Header sent flag */
  I2C_FLAG_TRANSFERFINISHED    = (uint16_t)0x0104,  /*!< Data Byte Transfer Finished flag */
  I2C_FLAG_ADDRESSSENTMATCHED  = (uint16_t)0x0102,  /*!< Address Sent/Matched (master/slave) flag */
  I2C_FLAG_STARTDETECTION      = (uint16_t)0x0101,  /*!< Start bit sent flag */

  /* SR2 register flags */
  I2C_FLAG_WAKEUPFROMHALT      = (uint16_t)0x0220,  /*!< Wake Up From Halt Flag */
  I2C_FLAG_OVERRUNUNDERRUN     = (uint16_t)0x0208,  /*!< Overrun/Underrun flag */
  I2C_FLAG_ACKNOWLEDGEFAILURE  = (uint16_t)0x0204,  /*!< Acknowledge Failure Flag */
  I2C_FLAG_ARBITRATIONLOSS     = (uint16_t)0x0202,  /*!< Arbitration Loss Flag */
  I2C_FLAG_BUSERROR            = (uint16_t)0x0201,  /*!< Misplaced Start or Stop condition */

  /* SR3 register flags */
  I2C_FLAG_GENERALCALL         = (uint16_t)0x0310,  /*!< General Call header received Flag */
  I2C_FLAG_TRANSMITTERRECEIVER = (uint16_t)0x0304,  /*!< Transmitter Receiver Flag */
  I2C_FLAG_BUSBUSY             = (uint16_t)0x0302,  /*!< Bus Busy Flag */
  I2C_FLAG_MASTERSLAVE         = (uint16_t)0x0301   /*!< Master Slave Flag */
} I2C_Flag_TypeDef;

/**
  * @brief I2C Pending bits
  * Elements values convention: 0xXYZZ
  *  X = SRx registers index
  *      X = 1 : SR1
  *      X = 2 : SR2
  *  Y = Position of the corresponding Interrupt
  *  ZZ = flag mask in the dedicated register(X register)
  */

typedef enum
{
    /* SR1 register flags */
    I2C_ITPENDINGBIT_TXEMPTY             = (uint16_t)0x1680, 	/*!< Transmit Data Register Empty  */
    I2C_ITPENDINGBIT_RXNOTEMPTY          = (uint16_t)0x1640, 	/*!< Read Data Register Not Empty  */
    I2C_ITPENDINGBIT_STOPDETECTION       = (uint16_t)0x1210, 	/*!< Stop detected  */
    I2C_ITPENDINGBIT_HEADERSENT          = (uint16_t)0x1208, 	/*!< 10-bit Header sent */
    I2C_ITPENDINGBIT_TRANSFERFINISHED    = (uint16_t)0x1204, 	/*!< Data Byte Transfer Finished  */
    I2C_ITPENDINGBIT_ADDRESSSENTMATCHED  = (uint16_t)0x1202, 	/*!< Address Sent/Matched (master/slave)  */
    I2C_ITPENDINGBIT_STARTDETECTION      = (uint16_t)0x1201, 	/*!< Start bit sent  */

    /* SR2 register flags */
    I2C_ITPENDINGBIT_WAKEUPFROMHALT      = (uint16_t)0x2220, 	/*!< Wake Up From Halt  */
    I2C_ITPENDINGBIT_OVERRUNUNDERRUN     = (uint16_t)0x2108, 	/*!< Overrun/Underrun  */
    I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE  = (uint16_t)0x2104, 	/*!< Acknowledge Failure  */
    I2C_ITPENDINGBIT_ARBITRATIONLOSS     = (uint16_t)0x2102, 	/*!< Arbitration Loss  */
    I2C_ITPENDINGBIT_BUSERROR            = (uint16_t)0x2101  	/*!< Misplaced Start or Stop condition */
} I2C_ITPendingBit_TypeDef;

/**
  * @brief I2C possible events
  * Values convention: 0xXXYY
  * XX = Event SR3 corresponding value
  * YY = Event SR1 corresponding value
  * @note if Event = EV3_2 the rule above does not apply
  * YY = Event SR2 corresponding value
  */

typedef enum
{
  
	// IDLE bus
	I2C_IDLE =  (uint16_t)0x0000, // IDLE bus
	/*========================================

 

                       I2C Slave Events (Events grouped in order of communication)
                                                          ==========================================*/

  /**
    * @brief  Communication start events
    *
    * Wait on one of these events at the start of the communication. It means that
    * the I2C peripheral detected a Start condition on the bus (generated by master
    * device) followed by the peripheral address.
    * The peripheral generates an ACK condition on the bus (if the acknowledge
    * feature is enabled through function I2C_AcknowledgeConfig()) and the events
    * listed above are set :
    *
    * 1) In normal case (only one address managed by the slave), when the address
    *   sent by the master matches the own address of the peripheral (configured by
    *   I2C_OwnAddress1 field) the I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED event is set
    *   (where XXX could be TRANSMITTER or RECEIVER).
    *
    * 2) In case the address sent by the master is General Call (address 0x00) and 
    *   if the General Call is enabled for the peripheral (using function I2C_GeneralCallCmd()) 
    *   the following event is set I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED.  
    * 
    */

  /* --EV1  (all the events below are variants of EV1) */
  /* 1) Case of One Single Address managed by the slave */
  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED    = (uint16_t)0x0202,  /*!< BUSY and ADDR flags */
  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED = (uint16_t)0x0682,  /*!< TRA, BUSY, TXE and ADDR flags */

  /* 2) Case of General Call enabled for the slave */
  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED  = (uint16_t)0x1200,  /*!< EV2: GENCALL and BUSY flags */

  /**
    * @brief  Communication events
    *
    * Wait on one of these events when EV1 has already been checked :
    *
    * - Slave RECEIVER mode:
    *     - EV2: When the application is expecting a data byte to be received.
    *     - EV4: When the application is expecting the end of the communication:
    *       master sends a stop condition and data transmission is stopped.
    *
    * - Slave Transmitter mode:
    *    - EV3: When a byte has been transmitted by the slave and the application
    *      is expecting the end of the byte transmission.
    *      The two events I2C_EVENT_SLAVE_BYTE_TRANSMITTED and I2C_EVENT_SLAVE_BYTE_TRANSMITTING
    *      are similar. The second one can optionally be used when the user software
    *      doesn't guarantee the EV3 is managed before the current byte end of transfer.
    *    - EV3_2: When the master sends a NACK in order to tell slave that data transmission
    *      shall end (before sending the STOP condition).
    *      In this case slave has to stop sending data bytes and expect a Stop
    *      condition on the bus.
    *
    *  @note In case the  user software does not guarantee that the event EV2 is
    *  managed before the current byte end of transfer, then user may check on EV2
    *  and BTF flag at the same time (ie. (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_BTF)).
    *  In this case the communication may be slower.
    *
    */
  /* Slave RECEIVER mode --------------------------*/
  /* --EV2 */
  I2C_EVENT_SLAVE_BYTE_RECEIVED              = (uint16_t)0x0240,  /*!< BUSY and RXNE flags */
  /* --EV4  */
  I2C_EVENT_SLAVE_STOP_DETECTED              = (uint16_t)0x0010,  /*!< STOPF flag */

  /* Slave TRANSMITTER mode -----------------------*/
  /* --EV3 */
  I2C_EVENT_SLAVE_BYTE_TRANSMITTED           = (uint16_t)0x0684,  /*!< TRA, BUSY, TXE and BTF flags */
  I2C_EVENT_SLAVE_BYTE_TRANSMITTING          = (uint16_t)0x0680,  /*!< TRA, BUSY and TXE flags */
  /* --EV3_2 */
  //I2C_EVENT_SLAVE_ACK_FAILURE                = (uint16_t)0x0004,  /*!< AF flag */
	I2C_BUS_BUSY															=(uint16_t)0x0200, // BUSY flags
	I2C_START_FLAG														=(uint16_t)0x0001, // START flags
	I2C_RXNE_FLAG															=(uint16_t)0x0040 // RXNE flags
} I2C_Event_TypeDef;

/**
  * @}
  */
typedef struct
	{
		uint8_t msl:1; // master=1 / slave=0
		uint8_t busy:1; //bus busy =1
		uint8_t tra:1; // transmitter=1/ reciever=0
		uint8_t reserv_sr3:1; // reserved
		uint8_t gencall:1;	// generall call header=1 (slave)
		uint8_t reserv2_sr3:2; // reserved
		uint8_t dualf:1; // Dual flag OAR2=0 OAR2=1 (slave)
			//-----------------------
		uint8_t sb:1; // start bit generation=1
		uint8_t addr:1; // address sent=1 (master)/match=1 (slave)
		uint8_t btf:1;	// byte transfer=1
		uint8_t add10:1; // master has sent header 10-bit=1 address
		uint8_t stopf:1; //stop detection=1 (slave)
		uint8_t reserv_sr1:1; // reserved
		uint8_t rxne:1; //data register not empty=1 (receiver)
		uint8_t txe:1; // data register empty=1 (transmitters)
	} I2C_EventBit_t;


typedef struct
{
i2cFunc_t Func;
union
{
	I2C_Event_TypeDef ItEvent;
	I2C_EventBit_t 		ItEventBit;
};
union
{
		uint8_t	DeviceAddrRW;
	struct
	{	
		uint8_t Dir:1;
		uint8_t	DeviceAddr:7;
	};
};
union
{
	uint8_t Error;
	i2c_sr2bit_t	ErrorBit;
};
uint8_t *ArraySend;
uint8_t NumSend;
uint8_t *ArrSendReceive; 
uint8_t NumSendReceive;
uint8_t CurrentIndex;
}i2cTask_t;

typedef union
	{
		uint8_t ccrh;      /*!< I2C clock control register high */
		struct
		{
			uint8_t ccr11_8:4; // clock control in Master mode
			uint8_t reserv_ccrh:2;
			uint8_t duty:1; // 0: tlow/thigh=2/1; 1:tlow/thigh=16/9 
			uint8_t fs:1; // mode standard=0 fast=1
		};
	}CCRH_t;
/* Exported constants --------------------------------------------------------*/
/** @addtogroup I2C_Exported_Constants
  * @{
  */
#define I2C_MAX_STANDARD_FREQ ((uint32_t)100000)
#define I2C_MAX_FAST_FREQ     ((uint32_t)400000)
#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) 
 #define I2C_MAX_INPUT_FREQ     ((uint8_t)24)
#else
 #define I2C_MAX_INPUT_FREQ     ((uint8_t)16)
#endif

/**
  * @}
  */

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/

/** @addtogroup I2C_Private_Macros
  * @{
  */

/**
  * @brief  Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief   Macro used by the assert function to check the different I2C duty cycles.
  */
#define IS_I2C_DUTYCYCLE_OK(DUTY) \
  (((DUTY) == I2C_DUTYCYCLE_2) || \
   ((DUTY) == I2C_DUTYCYCLE_16_9))

/**
  * @brief   Macro used by the assert function to check the different acknowledgement configuration
  */
#define IS_I2C_ACK_OK(ACK) \
  (((ACK) == I2C_ACK_NONE) || \
   ((ACK) == I2C_ACK_CURR) || \
   ((ACK) == I2C_ACK_NEXT))

/**
  * @brief   Macro used by the assert function to check the different I2C addressing modes.
  */
#define IS_I2C_ADDMODE_OK(ADDMODE) \
  (((ADDMODE) == I2C_ADDMODE_7BIT) || \
   ((ADDMODE) == I2C_ADDMODE_10BIT))

/**
  * @brief   Macro used by the assert function to check the different I2C interrupt types.
  */
#define IS_I2C_INTERRUPT_OK(IT) \
  (((IT) == I2C_IT_ERR)     || \
   ((IT) == I2C_IT_EVT)     || \
   ((IT) == I2C_IT_BUF)     || \
   ((IT) == (I2C_IT_ERR | I2C_IT_EVT)) || \
   ((IT) == (I2C_IT_ERR | I2C_IT_BUF)) || \
   ((IT) == (I2C_IT_EVT | I2C_IT_BUF)) || \
   ((IT) == (I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR)))
/**
  * @brief   Macro used by the assert function to check the different I2C communcation direction.
  */
#define IS_I2C_DIRECTION_OK(DIR) \
  (((DIR) == I2C_DIRECTION_TX) || \
   ((DIR) == I2C_DIRECTION_RX))

/**
  * @brief   Macro used by the assert function to check the different I2C flags.
  */
#define IS_I2C_FLAG_OK(FLAG) \
  (((FLAG) == I2C_FLAG_TXEMPTY)         || \
   ((FLAG) == I2C_FLAG_RXNOTEMPTY)       || \
   ((FLAG) == I2C_FLAG_STOPDETECTION)      || \
   ((FLAG) == I2C_FLAG_HEADERSENT)       || \
   ((FLAG) == I2C_FLAG_TRANSFERFINISHED)   || \
   ((FLAG) == I2C_FLAG_ADDRESSSENTMATCHED)  || \
   ((FLAG) == I2C_FLAG_STARTDETECTION)    || \
   ((FLAG) == I2C_FLAG_WAKEUPFROMHALT)    || \
   ((FLAG) == I2C_FLAG_OVERRUNUNDERRUN)    || \
   ((FLAG) == I2C_FLAG_ACKNOWLEDGEFAILURE)  || \
   ((FLAG) == I2C_FLAG_ARBITRATIONLOSS)    || \
   ((FLAG) == I2C_FLAG_BUSERROR)       || \
   ((FLAG) == I2C_FLAG_GENERALCALL)      || \
   ((FLAG) == I2C_FLAG_TRANSMITTERRECEIVER) || \
   ((FLAG) == I2C_FLAG_BUSBUSY) || \
   ((FLAG) == I2C_FLAG_MASTERSLAVE))
/**
  * @brief   Macro used by the assert function to check the I2C flags to clear.
  */

#define IS_I2C_CLEAR_FLAG_OK(FLAG) ((((uint16_t)(FLAG) & (uint16_t)0xFD00) == 0x00) \
                                     && ((uint16_t)(FLAG) != 0x00))

/**
  * @brief   Macro used by the assert function to check the different I2C possible pending bits.
  */
#define IS_I2C_ITPENDINGBIT_OK(ITPENDINGBIT) \
   (((ITPENDINGBIT) == I2C_ITPENDINGBIT_TXEMPTY)   					|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_RXNOTEMPTY) 				|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_STOPDETECTION) 			|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_HEADERSENT) 				|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_TRANSFERFINISHED) 	|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ADDRESSSENTMATCHED) || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_STARTDETECTION) 		|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_WAKEUPFROMHALT)		 	|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_OVERRUNUNDERRUN) 		|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE) || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ARBITRATIONLOSS) 		|| \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_BUSERROR))
    
/**
  * @brief  Macro used by the assert function to check the different I2C possible
  *   pending bits to clear by writing 0.
  */
#define IS_I2C_CLEAR_ITPENDINGBIT_OK(ITPENDINGBIT) \
   (((ITPENDINGBIT) == I2C_ITPENDINGBIT_WAKEUPFROMHALT)     || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_OVERRUNUNDERRUN)    || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ACKNOWLEDGEFAILURE) || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_ARBITRATIONLOSS)    || \
    ((ITPENDINGBIT) == I2C_ITPENDINGBIT_BUSERROR))
   
/**
  * @brief   Macro used by the assert function to check the different I2C possible events.
  */
#define IS_I2C_EVENT_OK(EVENT) (((EVENT) == I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_RECEIVED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_RECEIVED | (uint16_t)I2C_FLAG_GENERALCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_TRANSMITTED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | (uint16_t)I2C_FLAG_GENERALCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_ACK_FAILURE) || \
                             ((EVENT) == I2C_EVENT_SLAVE_STOP_DETECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_SELECT) || \
                             ((EVENT) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_RECEIVED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTING) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_ADDRESS10))

/**
  * @brief   Macro used by the assert function to check the different I2C possible own address.
  */
#define IS_I2C_OWN_ADDRESS_OK(ADDRESS) \
  ((ADDRESS) <= (uint16_t)0x03FF)

/* The address must be even */
#define IS_I2C_ADDRESS_OK(ADD) \
  (((ADD) & (uint8_t)0x01) == (uint8_t)0x00)

/**
  * @brief   Macro used by the assert function to check that I2C Input clock frequency must be between 1MHz and 50MHz.
  */
#define IS_I2C_INPUT_CLOCK_FREQ_OK(FREQ) \
  (((FREQ) >= (uint8_t)1) && ((FREQ) <= I2C_MAX_INPUT_FREQ))


/**
  * @brief   Macro used by the assert function to check that I2C Output clock frequency must be between 1Hz and 400kHz.
  */
#define IS_I2C_OUTPUT_CLOCK_FREQ_OK(FREQ) \
  (((FREQ) >= (uint8_t)1) && ((FREQ) <= I2C_MAX_FAST_FREQ))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup I2C_Exported_Functions
  * @{
  */
// Init I2C module	
void I2C_Init_7bit(uint32_t OutputClockFrequencyHz);

i2cStatus_t I2C_MasterSend(uint8_t DeviceAddress, void *ArraySend, uint16_t NumSend);
i2cStatus_t I2C_MasterReceive(uint8_t DeviceAddress, void *ArrayReceive, uint16_t NumReceive);
i2cStatus_t I2C_MasterSendPtrSendData(uint8_t DeviceAddress,void *Ptr, uint16_t PtrSize, void *ArraySend, uint16_t NumSend);
i2cStatus_t I2C_MasterSendPtrReceiveData(uint8_t DeviceAddress,void *Ptr, uint16_t PtrSize, void *ArrayReceive, uint16_t NumReceive);

void I2C_DeInit(void);					
void I2C_Cmd(FunctionalState NewState);
//void I2C_GeneralCallCmd(FunctionalState NewState);
//void I2C_ITConfig(I2C_IT_TypeDef I2C_IT, FunctionalState NewState);

// Функция передает в начале массив адреса регистра, а затем передает данные.
//void I2C_MasterSendSend(uint8_t DeviceAddress, uint8_t *ArrayAddress, uint8_t NumAddress, uint8_t *ArraySend, uint8_t NumSend);

// Функция отправляет в начале массив *ArrSend в количестве NumSend, затем считывает в массив *ArrReceive в количестве NumReceive
//void I2C_MasterSendReceive(uint8_t DeviceAddress, uint8_t *ArrSend, uint8_t NumSend, uint8_t *ArrReceive, uint8_t NumReceive);
// Фукнция показывающая идет ли прием или передача данных,
// Если передачи или приема нет, тогда результат функции 0
// Ожидание окончания передачи или приема 
// while(i2cCheckStatusTransfer());
//i2cFunc_t i2cCheckStatusTransfer(void);
// Проверка ошибки передачи или приема
// Если ошибок передачи не было, то результат функции 0
//uint8_t i2cCheckErrorTransfer(void);
// System function
//void I2C_SoftwareResetCmd(void);
//void I2C_StretchClockCmd(FunctionalState NewState);
//void I2C_FastModeDutyCycleConfig(I2C_DutyCycle_TypeDef I2C_DutyCycle);

// Resrart I2C module
//void I2C_restartInit(void);
/*
void i2cHandler(void);
*/
//----------------------------------------------------------


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
 *           I2Cx_IRQHandler() function in order to determine which error occurred.
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
 */

/**
 *
 *  1) Basic state monitoring
 *******************************************************************************
 */
ErrorStatus I2C_CheckEvent(I2C_Event_TypeDef I2C_Event);
/**
 *
 *  2) Advanced state monitoring
 *******************************************************************************
 */
I2C_Event_TypeDef I2C_GetLastEvent(void);
/**
 *
 *  3) Flag-based state monitoring
 *******************************************************************************
 */
//FlagStatus I2C_GetFlagStatus(I2C_Flag_TypeDef I2C_Flag);
/**
 *
 *******************************************************************************
 */
void I2C_ClearFlag(I2C_Flag_TypeDef I2C_FLAG);
ITStatus I2C_GetITStatus(I2C_ITPendingBit_TypeDef I2C_ITPendingBit);
void I2C_ClearITPendingBit(I2C_ITPendingBit_TypeDef I2C_ITPendingBit);


/**
  * @}
  */

#endif /* __STM8S_I2C_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
