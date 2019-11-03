#ifndef __main_H
#define __main_H

typedef enum
{
	i2cIdle=0,
	i2cSend=1,
	i2cSendSend=2,
	i2cReceive=3,
	i2cSendReceive=4
} i2cFunc_t;

void i2cSendAddress(void);
ErrorStatus i2cNumSendCheck(void);
i2cFunc_t i2cFuncCheck(void);
void i2cFuncSendSend(void);
void i2cFuncSend(void);
I2C_Event_TypeDef i2cEventGet(void);
void i2cFuncReceive(void);
void i2cFuncAddressSendReceive(void);
void i2cFuncSendReceive(void);
#endif