#ifndef __STM8S_BME280_C
#define __STM8S_BME280_C
#include "inc/stm8s_bme280.h" 


uint8_t BME280_Init(bme280_t *bme, 
									uint8_t I2Caddress, 
									bme280_osrs_t Pressure,
									bme280_osrs_t Temperature, 
									bme280_osrs_t Humidity,
									bme280_filter_t Filter,
									bme280_standby_t StandBy,
									bme280_mode_t Mode
									)
{
	uint8_t i,temp;
	bme->i2c_address=I2Caddress;
	bme->osrs_p=Pressure;
	bme->osrs_t=Temperature;
	bme->ctrl_hum=Humidity;
	bme->filter=Filter;
	bme->t_sb=StandBy;
	//bme->spi3w_en=spi3w_en;

	if (Temperature==BME280_NO_OVERSAMPLING) bme->osrs_t=BME280_OVERSAMPLING_16X; 
	// Отправка настроки температуры и давления и режима работы

	bme->mode=BME280_SLEEP_MODE; // Остановка режима измерения
	bme->a[0]=0xF4;
	I2C_MasterSendSend(I2Caddress, bme->a, 1, bme->a+48, 1);
	while(i2cCheckStatusTransfer());
	//Чтение информации о поправочных коэффициентах 
	bme->a[0]=0x88;
	I2C_MasterSendReceive(I2Caddress, bme->a, 1, bme->a+1, 24);
	while(i2cCheckStatusTransfer());
	bme->a[0]=0xA1;
	I2C_MasterSendReceive(I2Caddress, bme->a, 1, bme->a+25, 1);
	while(i2cCheckStatusTransfer());
	bme->a[0]=0xE1;
	I2C_MasterSendReceive(I2Caddress, bme->a, 1, bme->a+26, 5);
	while(i2cCheckStatusTransfer());
	bme->a[0]=0xE5;
	I2C_MasterSendReceive(I2Caddress, bme->a, 1, bme->a+31, 3);
	while(i2cCheckStatusTransfer());
	// Преобразование коэффициентов для вычисления
	// Преобразование коэффициентов для T1-T3 P1-P9
	for(i=1;i<24;i=i+2)
	{
		temp=bme->a[i];
		bme->a[i]=bme->a[i+1];
		bme->a[i+1]=temp;
	}
	//Преобразование H2	
		i=26;
		temp=bme->a[i];
		bme->a[i]=bme->a[i+1];
		bme->a[i+1]=temp;
	//Преобразование H4
	bme->a[30]=bme->a[30]<<4;
	bme->dig_H4=bme->dig_H4>>4;
	// Преобразование H5
	i=31;
	temp=bme->a[i];
	bme->a[i]=bme->a[i+1];
	bme->a[i+1]=temp;
	bme->dig_H5=bme->dig_H5>>4;
	//------------
	// Отправка времени и фильтра
	bme->a[0]=0xF5;
	I2C_MasterSendSend(I2Caddress, bme->a, 1, bme->a+46, 1);
	while(i2cCheckStatusTransfer());
	// Отправка настройки влажности
	bme->a[0]=0xF2;
	I2C_MasterSendSend(I2Caddress, bme->a, 1, bme->a+47, 1);
	while(i2cCheckStatusTransfer());
	// Отправка настроки температуры и давления и режима работы
	bme->mode=Mode;
	bme->a[0]=0xF4;
	I2C_MasterSendSend(I2Caddress, bme->a, 1, bme->a+48, 1);
	while(i2cCheckStatusTransfer());
	return i2cCheckErrorTransfer();
}

uint16_t BME280_T_oversample(bme280_osrs_t t)
{
	switch(t)
	{
		case 0: return 0;
		break;
		case 1: return 1;
		break;
		case 2: return 2;
		break;
		case 3: return 4;
		break;
		case 4: return 8;
		break;
		case 5: return 16;
		break;
	}
}



uint16_t BME280_StartStop(bme280_t *bme, bme280_mode_t mode)
{
	uint16_t time;
	bme->mode=mode;
	// Отправка настройки температуры и давления и режима работы
	bme->a[0]=0xF4;
	I2C_MasterSendSend(bme->i2c_address, bme->a, 1, bme->a+48, 1);
	time =31+23*(BME280_T_oversample(bme->osrs_t)+
		BME280_T_oversample(bme->osrs_p)+
		BME280_T_oversample(bme->osrs_h));
	time=time/10+1;
	if (bme->mode=BME280_NORMAL_MODE)
	{
		switch(bme->t_sb)
		{
			case BME280_STANDBY_TIME_1_MS:
			time++;
			break;
			case BME280_STANDBY_TIME_10_MS:
			time+=10;
			break;
			case BME280_STANDBY_TIME_20_MS:
			time+=20;
			break;
			case BME280_STANDBY_TIME_62_5_MS:
			time+=63;
			break;
			case BME280_STANDBY_TIME_125_MS:
			time+=125;
			break;
			case BME280_STANDBY_TIME_250_MS:
			time+=250;
			break;
			case BME280_STANDBY_TIME_500_MS:
			time+=500;
			break;
			case BME280_STANDBY_TIME_1000_MS:
			time+=1000;
			break;
		}
	}
	
	while(i2cCheckStatusTransfer());
	return time;
}

uint8_t BME280_GetMeasurement(bme280_t *bme)
{
	int32_t var1, var2;
	uint8_t ret;
	// Запрос на чтение статусного регистра
	bme->a[0]=0xF3;
	I2C_MasterSendReceive(bme->i2c_address, bme->a, 1, bme->a+49, 1);
	while(i2cCheckStatusTransfer());
	// Запрос на чтение регистра данных о давлении, температуры, влажности
	bme->a[0]=0xF7;
	I2C_MasterSendReceive(bme->i2c_address, bme->a, 1, bme->a+35, 8);
	while(i2cCheckStatusTransfer());
	// Результат передачи
	ret=i2cCheckErrorTransfer();
	// Преобразование формата влажности
	bme->a[44]=bme->a[41];
	bme->a[45]=bme->a[42];
	bme->a[42]=0;
	// Преобразование давления
	bme->uncomp_press=bme->uncomp_press>>4;
	// Преобразование температуры
	bme->uncomp_temp=bme->uncomp_temp>>12;
	// Вычисление промежуточной температуры
	var1 = ((((bme->uncomp_temp>>3) - ((int32_t)bme->dig_T1<<1))) * ((int32_t)bme->dig_T2)) >> 11;
	var2 = (((((bme->uncomp_temp>>4) - ((int32_t)bme->dig_T1)) * ((bme->uncomp_temp>>4) - ((int32_t)bme->dig_T1))) >> 12) *
	((int32_t)bme->dig_T3)) >> 14;
	bme->t_fine = var1 + var2;
	return ret;
}

uint8_t BME280_StatusMeasuring(bme280_t *bme)
{
	return bme->measuring;
}

uint8_t BME280_StatusImUpdate(bme280_t *bme)
{
	return bme->im_update;
}

void BME280_Reset(bme280_t *bme)
{
	uint8_t r[1]={0xB6};
	bme->a[0]=0xE0;
	I2C_MasterSendSend(bme->i2c_address, bme->a, 1, r, 1);
	while(i2cCheckStatusTransfer());
}


int32_t BME280_compensate_T_int32(bme280_t *bme)
{
	/*
int32_t var1, var2, T;
var1 = ((((bme->uncomp_temp>>3) - ((int32_t)bme->dig_T1<<1))) * ((int32_t)bme->dig_T2)) >> 11;
var2 = (((((bme->uncomp_temp>>4) - ((int32_t)bme->dig_T1)) * ((bme->uncomp_temp>>4) - ((int32_t)bme->dig_T1))) >> 12) *
((int32_t)bme->dig_T3)) >> 14;
bme->t_fine = var1 + var2;
T = (bme->t_fine * 5 + 128) >> 8;
return T; */
return (int32_t)((bme->t_fine * 5 + 128) >> 8);
}

uint32_t BME280_compensate_P_int32(bme280_t *bme)
{
int32_t var1, var2;
uint32_t p;
var1 = (((int32_t)bme->t_fine)>>1) - (int32_t)64000;
var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)bme->dig_P6);
var2 = var2 + ((var1*((int32_t)bme->dig_P5))<<1);
var2 = (var2>>2)+(((int32_t)bme->dig_P4)<<16);
var1 = (((bme->dig_P3 * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)bme->dig_P2) * var1)>>1))>>18;
var1 =((((32768+var1))*((int32_t)bme->dig_P1))>>15);
if (var1 == 0)
{
return 0; // avoid exception caused by division by zero
}
p = (((uint32_t)(((int32_t)1048576)-bme->uncomp_press)-(var2>>12)))*3125;
if (p < 0x80000000)
{
p = (p << 1) / ((uint32_t)var1);
}
else
{
p = (p / (uint32_t)var1) * 2;
}
var1 = (((int32_t)bme->dig_P9) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
var2 = (((int32_t)(p>>2)) * ((int32_t)bme->dig_P8))>>13;
p = (uint32_t)((int32_t)p + ((var1 + var2 + bme->dig_P7) >> 4));
return p;
}


// Output value of “47445” represents 47445/1024 = 46.333 %RH
uint32_t BME280_compensate_H_int32(bme280_t *bme)
{
int32_t v_x1_u32r;
v_x1_u32r = (bme->t_fine - ((int32_t)76800));
v_x1_u32r = (((((bme->uncomp_hum << 14) - (((int32_t)bme->dig_H4) << 20) - (((int32_t)bme->dig_H5) * v_x1_u32r)) +
((int32_t)16384)) >> 15) * (((((((v_x1_u32r * ((int32_t)bme->dig_H6)) >> 10) * (((v_x1_u32r *
((int32_t)bme->dig_H3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
((int32_t)bme->dig_H2) + 8192) >> 14));
v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)bme->dig_H1)) >> 4));
v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
v_x1_u32r = (v_x1_u32r > 419430400 ? 419430400 : v_x1_u32r);
return (uint32_t)(v_x1_u32r>>12);
}

#endif 