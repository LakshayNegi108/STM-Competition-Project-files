/*
 * MLX90614.h
 *
 *  Created on: Oct 25, 2022
 *      Author: hp
 */

#ifndef MLX90614_LIB_INC_MLX90614_H_
#define MLX90614_LIB_INC_MLX90614_H_

#include "BML_DEF.h"

extern I2C_HandleTypeDef hi2c1;

#define _MLX90614_I2C         hi2c1
#define _MLX90614_I2C_ADDRESS 0xB4
#define _MLX90614_FREERTOS    0

typedef enum
{
	MLX90614_UNIT_RAW,
	MLX90614_UNIT_K,
	MLX90614_UNIT_C,
	MLX90614_UNIT_F

} MLX90614_UNIT_t;

typedef struct
{
  uint8_t   IIR : 3;
  uint8_t   RepeatSensorTestON: 1;
  uint8_t   SelectObjAmb: 2;
  uint8_t   DualIRSensor: 1;
  uint8_t   NegativeSignOfKs: 1;
  uint8_t   FIR: 3;
  uint8_t   GAIN: 3;
  uint8_t   NegativeSignOfKt2: 1;
  uint8_t   DisableSensorTest: 1;

}MLX90614_CONFIG_REG_t;

typedef struct
{
  MLX90614_UNIT_t unit;
	int16_t rawEmissivity, rawAmbient, rawObject1, rawObject2, rawMax, rawMin;
  float emissivity, ambient, object1, object2, max, min;
	uint16_t id[4];
  MLX90614_CONFIG_REG_t configReg;

}MLX90614_t;

bool mlx90614_init(void);
void mlx90614_setUnit(MLX90614_UNIT_t MLX90614_UNIT_);
bool mlx90614_readID(int16_t *id);
bool mlx90614_getEmissivity(float *emissivity);
bool mlx90614_setEmissivity(float emissivity);
bool mlx90614_setMax(float maxTemp);
bool mlx90614_setMin(float minTemp);
bool mlx90614_getMax(float *maxTemp);
bool mlx90614_getMin(float *minTemp);
bool mlx90614_getAmbient(float *ambientTemp);
bool mlx90614_getObject1(float *objectTemp);
bool mlx90614_getObject2(float *objectTemp);

#endif /* MLX90614_LIB_INC_MLX90614_H_ */
