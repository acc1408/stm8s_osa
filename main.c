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


/* Includes ------------------------------------------------------------------*/

#include "stm8s.h"
#include "stm8s_it.c"
/* Private defines -----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



//i2cTask_t i2cTask={0,0,0,0,0,0,0,0};


//uint8_t a[10]={0xFF,2,3,4,5,6,7,8,9,10};
typedef union												//	*
{															//	*
struct														//	*
{															//	*
// Настройка увеличения счетчкиа курсора или экрана
uint8_t	ID_cursorShiftRightLeft:1;// разрешен=1/запрещен=0*
uint8_t S_ScreenShiftOnOff:1;// Сдвиг экрана при записи разшереш=1/ запрещен=0*
//--------------------------------------------------------------*
//Настройка экран и курсора
uint8_t B_BlinkOnOff:	1; 	// Мигание курсора вкл=1/откл=0	*
uint8_t C_CursorOnOff:	1; 	// курсор-подчеркивание виден=1/ невиден=0*
uint8_t D_DisplayOnOff:1; 	// Дисплей включен=1/отключен=0 *
//--------------------------------------------------------------*
// прокрутка дисплея без изменения DDRAM 
uint8_t SC_shiftScreenCursor:1;
uint8_t RL_RightLeft:1;
//--------------------
uint8_t F_font:	1;	// Ширифт: 5*7px=0/5*10px=1					*
uint8_t N_lines:	1;	// кол-во строк: строка1=0/строк2=1		*
//uint8_t bit:	1;	// битность передачи 4бит=0 8бит=1
uint8_t Backlight:1;
};															//	*
uint8_t set;		// доступ ко всем полям		*
}SetLCD_t;													//	*
//***************************************************************
SetLCD_t lcd;


typedef union
{
	uint8_t data;
	struct
	{
		uint8_t rs:1;
		uint8_t rw:1;
		uint8_t e:1;
		uint8_t bl:1; //backlight
		uint8_t db4:4;
	};
}i2cLcd1602_t;

i2cLcd1602_t b[10];

void LcdSendByte(uint8_t dc,uint8_t data)
{
	while(i2cCheckStatusTransfer());
	b[0].bl=lcd.Backlight;
	b[0].e=1;
	b[0].rs=dc;
	b[0].rw=0;
	b[0].db4=data>>4;
	b[1].data=b[0].data;
	b[1].e=0;
	//----------------
	b[2].data=b[0].data;
	b[2].db4=data&0x0F;
	//------------------
	b[3].data=b[2].data;
	b[3].e=0;
	i2cMasterSend(0b0111111, &b[0].data, 4);
	delay_ms(50);
}

const char st[]="Hellow world!!!";
void inputstring(char *st)
{
	//uint8_t i=0;
	while(*st)
	{
		LcdSendByte(1,*st++);
		//st++;
	}
}

void SettingLcd(void)
{
	lcd.ID_cursorShiftRightLeft=1;
	lcd.S_ScreenShiftOnOff=0;
	//---------------
	lcd.B_BlinkOnOff=1;
	lcd.C_CursorOnOff=1;
	lcd.D_DisplayOnOff=1;
	//------------
	lcd.SC_shiftScreenCursor=0;
	lcd.RL_RightLeft=0;
	//---------------------
	lcd.F_font=0;
	lcd.N_lines=1;
	//---------------
	lcd.Backlight=1;
	//-------------
	// Настройка 8 битного режима
	b[0].db4=0b0011;
	b[0].bl=1;
	b[0].e=1;
	b[0].rs=0;
	b[0].rw=0;
	b[1].data=b[0].data;
	b[1].e=0;
	//Предвартельная Настройка 4 битного режима
	b[2].data=b[0].data;
	b[2].db4=0b0010;
	b[3].data=b[2].data;
	b[3].e=0;
	//--------------------------
	i2cMasterSend(0b0111111, &b[0].data, 4);
	delay_ms(40);
	//LcdSendByte(0,1);
	//delay_ms(1600);
	//LcdSendByte(0,2);
	//delay_ms(1600);
	LcdSendByte(0,1<<2|lcd.ID_cursorShiftRightLeft<<1|lcd.S_ScreenShiftOnOff);
	LcdSendByte(0,1<<3|lcd.D_DisplayOnOff<<2|lcd.C_CursorOnOff<<1|lcd.B_BlinkOnOff);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	LcdSendByte(0,1<<5|lcd.N_lines<<3|lcd.F_font<<2);
	//delay_ms(2000);
	//LcdSendByte(1,'a');
	//LcdSendByte(1,'s');
	//LcdSendByte(1,'d');
	//delay_ms(2000);
	
	//LcdSendByte(1,'v');
	//LcdSendByte(1,'b');
	LcdSendByte(0,0x80|0x00);
	inputstring(st);
	delay_ms(2000);
	/*lcd.SC_shiftScreenCursor=1;
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(2000);
	lcd.RL_RightLeft=1;
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	delay_ms(1000);
	LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	*/
	LcdSendByte(0,0x80|0x00);
	//LcdSendByte(0,0x80|0x05);
}

void Task(void)
{
	Init_Delay();
	GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_LOW_FAST);
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_SoftwareResetCmd(ENABLE);
	I2C_SoftwareResetCmd(DISABLE);
	I2C_Init(100000, 50, 
              I2C_DUTYCYCLE_2, I2C_ACK_NONE, 
              I2C_ADDMODE_7BIT, 16 );
	I2C_ITConfig(I2C_IT_ERR|I2C_IT_EVT|I2C_IT_BUF, ENABLE);
	I2C_Cmd(ENABLE);
	SettingLcd();
	/*
	b[0].db4=0b0010;
	b[0].bl=1;
	b[0].e=1;
	b[0].rs=0;
	b[0].rw=0;
	b[1].data=b[0].data;
	b[1].e=0;
	//---------------------------------
	b[2].data=b[0].data;
	b[2].db4=0b0011;
	b[3].data=b[2].data;
	b[3].e=0;
	//----------------------------------
	
	
	i2cMasterSend(0b0111111, &b[0].data, 2);*/
//	i2cMasterSend(0b0111111, a+5, 5);
//	i2cMasterSendSend(0b0111111, a, 1,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 2,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,5);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,0);
//	i2cMasterSendSend(0b0111111, a, 0,a+5,1);
//	i2cMasterSendSend(0b0111111, a, 1,a+5,0);
//	i2cMasterReceive(0b0111111, a+9, 1);
// i2cMasterSendReceive(0b0111111, a, 3, a+5, 2);	

// i2cMasterSendReceive(0b0111111, a+3, 3, a+8, 2);	
	//i2cMasterReceive(0b0111111, a, 0);
	//i2cMasterSendBuf(0b0111111);	
	//i2cMasterUploadBuf(102);
	
	
	
	while(1)
	{
		GPIO_WriteReverse(GPIOE, GPIO_PIN_5);
		OS_Delay(1000);
	}
}

void main(void)
{
 #ifdef  __OSA__
 // Увеличиваем частоты тактирования до 16МГц
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
 	OS_Init();  // Инициализация RTOS OSA
	TIM4_TimerOSA(1000); //Настраиваем прерывание 500мкс
	OS_Task_Create(7, Task); // создаем задачу
	
	OS_EI();   // Разрешить все прерывания
	OS_Run(); // Запуск ядра RTOS OSA
#else
	/* Infinite loop */
	
	//Init_Delay();
	//GPIO_Init(GPIOD, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_FL_NO_IT);
  while (1)
  {
  //GPIO_WriteReverse(GPIOC, GPIO_PIN_1);
	//delay_ms(1000);
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
