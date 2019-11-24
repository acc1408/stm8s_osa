/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_BME280_H
#define __STM8S_BME280_H

#include "stm8s.h"
#include "inc/stm8s_i2c.h" // драйвер для шины I2C

// Инициализация структуры содержащей всей регистры
//
// Инициализация шины
// I2C_Init_7bit(100000);
// Инициализация датчика
// BME280_Init(&bm, 0b1110110,
//						BME280_OVERSAMPLING_16X,
//						BME280_OVERSAMPLING_16X,
//						BME280_OVERSAMPLING_16X,
//						BME280_FILTER_COEFF_8,
//						BME280_STANDBY_TIME_250_MS,
//						BME280_NORMAL_MODE,
//						DISABLE
//						);
// Запрос на чтение регистров из датчика
// BME280_GetMeasurement(&bm);
// Вычисление температуры 
// t=BME280_compensate_T_int32(&bm);
// Вычисление Давления
// pres=BME280_compensate_P_int32(&bm);
// Вычисление Влажности
// hum=BME280_compensate_H_int32(&bm);

		
		

typedef enum
{
	BME280_NO_OVERSAMPLING=0,		
	BME280_OVERSAMPLING_1X=1,		
	BME280_OVERSAMPLING_2X=2,		
	BME280_OVERSAMPLING_4X=3,		
	BME280_OVERSAMPLING_8X=4,		
	BME280_OVERSAMPLING_16X=5		
} bme280_osrs_t;

typedef enum
{
	BME280_SLEEP_MODE=0,		
	BME280_FORCED_MODE=1,		
	BME280_NORMAL_MODE=3		
} bme280_mode_t;

typedef enum
{
	BME280_STANDBY_TIME_1_MS=0,
  BME280_STANDBY_TIME_10_MS=6,
	BME280_STANDBY_TIME_20_MS=7,
	BME280_STANDBY_TIME_62_5_MS=1,
	BME280_STANDBY_TIME_125_MS=2,
	BME280_STANDBY_TIME_250_MS=3,
	BME280_STANDBY_TIME_500_MS=4,
	BME280_STANDBY_TIME_1000_MS=5
	
} bme280_standby_t;

typedef enum
{
	BME280_FILTER_COEFF_OFF=0,               
BME280_FILTER_COEFF_2=1,                 
BME280_FILTER_COEFF_4=2,
BME280_FILTER_COEFF_8=3, 
BME280_FILTER_COEFF_16=4  
} bme280_filter_t;
 


typedef union {
uint8_t a[54];
struct{
 uint8_t address; // 0
 uint16_t dig_T1; // 2
 int16_t dig_T2;	// 4
 int16_t dig_T3;	// 6
 uint16_t dig_P1;	// 8
 int16_t dig_P2;	// 10
 int16_t dig_P3;	// 12
 int16_t dig_P4;	// 14
 int16_t dig_P5;	// 16
 int16_t dig_P6;	// 18
 int16_t dig_P7;	// 20
 int16_t dig_P8;	// 22
 int16_t dig_P9;	// 24
 uint8_t dig_H1;	// 25
 //-------------
 int16_t dig_H2;	// 27
 uint8_t dig_H3;	// 28
 int16_t dig_H4;	// 30
 int16_t dig_H5;	// 32
 int8_t  dig_H6;	// 33
 
 uint32_t uncomp_press; // 34
 uint32_t uncomp_temp;	// 38
 uint32_t uncomp_hum;		// 42

union										// 46
	{
		uint8_t config;
		struct
		{
			uint8_t spi3w_en:1;
			uint8_t res_con:1;
			bme280_filter_t	filter:3;
			bme280_standby_t	t_sb:3;
		};
	};
 
union										// 47
	{
		uint8_t ctrl_hum;
		struct
		{
			bme280_osrs_t osrs_h:3;
			uint8_t res_hum:5;
		};
	};
 
union										// 48
	{
		uint8_t ctrl_meas;
		struct
		{
			bme280_mode_t mode:2;
			bme280_osrs_t osrs_t:3;
			bme280_osrs_t osrs_p:3;
		};
	};
union										// 49
	{
		uint8_t status;
		struct
		{
			uint8_t im_update:1;
			uint8_t rec_status:2;
			uint8_t measuring:1;
			uint8_t rec2_status:4;
		};
	};
 uint8_t i2c_address; // 50
 int32_t t_fine;	// 54
};
}bme280_t;

// Инициализация барометра
// Если результат функции 0, то ошибок нет, иначе ошибка инициализации
uint8_t BME280_Init(	bme280_t *bme, 			// структура содержащая все значения
									uint8_t I2Caddress, // адрес 0x76 или 0x77
									bme280_osrs_t Pressure, // настройка давления
									bme280_osrs_t Temperature, // Температуры
									bme280_osrs_t Humidity, //Влажности
									bme280_filter_t Filter, // Фильтр
									bme280_standby_t StandBy, // время между измерениями
									bme280_mode_t Mode,				// Режим работы
									FunctionalState spi3w_en // Включение 3 проводного интерфейса
									);
// Старт однократного или многократных преобразований
// Результат функции время преобразования в ms
uint8_t BME280_StartStop(bme280_t *bme, bme280_mode_t mode);
// Получить данные измерения
uint8_t BME280_GetMeasurement(bme280_t *bme);
// Вычислить Температуру
// Returns temperature in DegC, resolution is 0.01 DegC. Output value of “5123” equals 51.23 DegC.
int32_t BME280_compensate_T_int32(bme280_t *bme);
// Вычислслить Давление
// Returns pressure in Pa as unsigned 32 bit integer. Output value of “96386” equals 96386 Pa = 963.86 hPa
uint32_t BME280_compensate_P_int32(bme280_t *bme);
// Вычислить Влажность
// Output value of “47445” represents 47445/1024 = 46.333 %RH
uint32_t BME280_compensate_H_int32(bme280_t *bme);
// Получить статус измерения
uint8_t BME280_StatusMeasuring(bme280_t *bme);
// Получить статус обновления
uint8_t BME280_StatusImUpdate(bme280_t *bme);
// Сброс всех регистров
void BME280_Reset(bme280_t *bme);

#endif 