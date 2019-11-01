#ifndef __main_H
#define __main_H

uint8_t i2cAddressWrite(void);
I2C_Event_TypeDef i2cEventGetI(void);
ErrorStatus i2cMasterBufCheckSend(void);
uint8_t i2cMasterDownloadBuf(void);
void i2cStateSet(I2C_Event_TypeDef state);
ErrorStatus i2cMasterBufCheckReceive(void);
uint8_t i2cAddressRead(void);
void i2cMasterBufReadDataI(void);
#endif