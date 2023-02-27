/*
 * heartRate.h
 *
 *  Created on: 02-Feb-2023
 *      Author: hp
 */

#ifndef MAX30102_LIB_INC_HEARTRATE_H_
#define MAX30102_LIB_INC_HEARTRATE_H_

#include "BML_DEF.h"

bool checkForBeat(int32_t sample);
int16_t averageDCEstimator(int32_t *p, uint16_t x);
int16_t lowPassFIRFilter(int16_t din);
int32_t mul16(int16_t x, int16_t y);

#endif /* MAX30102_LIB_INC_HEARTRATE_H_ */
