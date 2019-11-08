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
// прокрутка дисплея влево в право без изменения DDRAM 
uint8_t SC_shiftScreenCursor:1;
uint8_t RL_RightLeft:1;
//--------------------
uint8_t F_font:	1;	// Ширифт: 5*7px=0/5*10px=1					*
uint8_t N_lines:	1;	// кол-во строк: строка1=0/строк2=1		*
//uint8_t bit:	1;	// битность передачи 4бит=0 8бит=1
uint8_t Backlight:1;
//uint8_t Address:7;  // Адрес шины
//i2cLcd1602_t data[4];
};															//	*
uint8_t set;		// доступ ко всем полям		*
}SetLCD_t;													//	*
//***************************************************************
SetLCD_t lcd;




i2cLcd1602_t b[4];

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

const char st[]="Hello world!!!qwertyuiopasdffghjj";
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
	LcdSendByte(0,1);
	delay_ms(1600);
	//LcdSendByte(0,2);
	//delay_ms(1600);
	LcdSendByte(0,1<<2|lcd.ID_cursorShiftRightLeft<<1|lcd.S_ScreenShiftOnOff);
	LcdSendByte(0,1<<3|lcd.D_DisplayOnOff<<2|lcd.C_CursorOnOff<<1|lcd.B_BlinkOnOff);
	//LcdSendByte(0,1<<4|lcd.SC_shiftScreenCursor<<3|lcd.RL_RightLeft<<2);
	LcdSendByte(0,1<<5|lcd.N_lines<<3|lcd.F_font<<2);
	//delay_ms(2000);
	//LcdSendByte(1,'a');
	//LcdSendByte(1,'s');
	//LcdSendByte(1,'d');
	delay_ms(2000);
	
	//LcdSendByte(1,'v');
	//LcdSendByte(1,'b');
	//LcdSendByte(0,0x80|0x00);
	//inputstring(st);
	//delay_ms(2000);
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
	//LcdSendByte(0,0x80|0x00);
	//LcdSendByte(0,0x80|0x05);
}


const char smile[] = 
{
	0x7e, 0x11, 0x11, 0x11, 0x7e,//A	0x90
	0x7f, 0x49, 0x49, 0x49, 0x33,//Р‘	0x91
	0x7f, 0x49, 0x49, 0x49, 0x36,//Р’	0x92
	0x7f, 0x01, 0x01, 0x01, 0x03,//Р“	0x93
	0xe0, 0x51, 0x4f, 0x41, 0xff,//Р”	0x94
	0x7f, 0x49, 0x49, 0x49, 0x41,//E	0x95
	0x77, 0x08, 0x7f, 0x08, 0x77,//Р–	0x96
	0x41, 0x49, 0x49, 0x49, 0x36,//Р—	0x97
	0x7f, 0x10, 0x08, 0x04, 0x7f,//Р	0x98
	0x7c, 0x21, 0x12, 0x09, 0x7c,//Р™	0x99
	0x7f, 0x08, 0x14, 0x22, 0x41,//K	0x9A
	0x20, 0x41, 0x3f, 0x01, 0x7f,//Р›	0x9B
	0x7f, 0x02, 0x0c, 0x02, 0x7f,//M	0x9C
	0x7f, 0x08, 0x08, 0x08, 0x7f,//H	0x9D
	0x3e, 0x41, 0x41, 0x41, 0x3e,//O	0x9E
	0x7f, 0x01, 0x01, 0x01, 0x7f,//Рџ	0x9F
	0x7f, 0x09, 0x09, 0x09, 0x06,//P	0xA0
	0x3e, 0x41, 0x41, 0x41, 0x22,//C	0xA1
	0x01, 0x01, 0x7f, 0x01, 0x01,//T	0xA2
	0x47, 0x28, 0x10, 0x08, 0x07,//РЈ	0xA3
	0x1c, 0x22, 0x7f, 0x22, 0x1c,//Р¤	0xA4
	0x63, 0x14, 0x08, 0x14, 0x63,//X	0xA5
	0x7f, 0x40, 0x40, 0x40, 0xff,//Р¦	0xA6
	0x07, 0x08, 0x08, 0x08, 0x7f,//Р§	0xA7
	0x7f, 0x40, 0x7f, 0x40, 0x7f,//РЁ	0xA8
	0x7f, 0x40, 0x7f, 0x40, 0xff,//Р©	0xA9
	0x01, 0x7f, 0x48, 0x48, 0x30,//РЄ	0xAA
	0x7f, 0x48, 0x30, 0x00, 0x7f,//Р«	0xAB
	0x00, 0x7f, 0x48, 0x48, 0x30,//Р­	0xAC
	0x22, 0x41, 0x49, 0x49, 0x3e,//Р¬	0xAD
	0x7f, 0x08, 0x3e, 0x41, 0x3e,//Р®	0xAE
	0x46, 0x29, 0x19, 0x09, 0x7f,//РЇ	0xAF
// РјР°Р»РµРЅСЊРєРёРµ Р±СѓРєРІС‹ 
 	0x20, 0x54, 0x54, 0x54, 0x78,//a	0xB0
	0x3c, 0x4a, 0x4a, 0x49, 0x31,//Р±	0xB1
	0x7c, 0x54, 0x54, 0x28, 0x00,//РІ	0xB2
	0x7c, 0x04, 0x04, 0x04, 0x0c,//Рі	0xB3
	0xe0, 0x54, 0x4c, 0x44, 0xfc,//Рґ	0xB4
	0x38, 0x54, 0x54, 0x54, 0x18,//e	0xB5
	0x6c, 0x10, 0x7c, 0x10, 0x6c,//Р¶	0xB6
	0x44, 0x44, 0x54, 0x54, 0x28,//Р·	0xB7
	0x7c, 0x20, 0x10, 0x08, 0x7c,//Рё	0xB8
	0x7c, 0x41, 0x22, 0x11, 0x7c,//Р№	0xB9
	0x7c, 0x10, 0x28, 0x44, 0x00,//Рє	0xBA
	0x20, 0x44, 0x3c, 0x04, 0x7c,//Р»	0xBB
	0x7c, 0x08, 0x10, 0x08, 0x7c,//Рј	0xBC
	0x7c, 0x10, 0x10, 0x10, 0x7c,//РЅ	0xBD
	0x38, 0x44, 0x44, 0x44, 0x38,//o	0xBE
	0x7c, 0x04, 0x04, 0x04, 0x7c //Рї	0xBF
};
bit8_t data;
uint8_t temp;
uint8_t i=0,k,y;
uint16_t j;
void SetChar(void)
{
	//LcdSendByte(0,0x40);
		
	for (k=0;k<30;k++)
	{
		LcdSendByte(0,0x40);
		for(i=0;i<8;i++)
		{
			temp=0;
			y=0;
			for(j=k*5;j<(k*5+5);j++)
			{
				temp|=((smile[j]>>i)&(1))<<(4-y);
				y++;
			}
			LcdSendByte(1,temp);
		}
	LcdSendByte(0,0x80|k);
	LcdSendByte(1,0);
	//LcdSendByte(1,1);
	}
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
	delay_ms(2000);
	SettingLcd();
	delay_ms(2000);
	SetChar();
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
