/*
 * MAX30102.c
 *
 *  Created on: Feb 1, 2023
 *      Author: hp
 */
#include "MAX30102.h"

BML_StatusTypeDef max30102_ret;

#define arrLen	32

uint32_t ir_arr[arrLen];
uint32_t red_arr[arrLen];
uint8_t ir_arrPutPtr = 0;
uint8_t red_arrPutPtr = 0;
uint8_t ir_arrGetPtr = 0;
uint8_t red_arrGetPtr = 0;
/**
 * @brief Built-in plotting function. Called during an interrupt to print/plot the current sample.
 * @note Override this in your main.c if you do not use printf() for printing.
 * @param ir_sample
 * @param red_sample
 */
void max30102_plot(uint32_t ir_sample, uint32_t red_sample) {

//	print("%d, %d\n\r", ir_sample, red_sample);
//	print("%d \n\r", ir_sample);
//	print("%d \n\r", red_sample);
	ir_arr[ir_arrPutPtr] = ir_sample;
	red_arr[red_arrPutPtr] = red_sample;
	ir_arrPutPtr = (ir_arrPutPtr + 1) % arrLen;
	red_arrPutPtr = (red_arrPutPtr + 1) % arrLen;
}

uint32_t max30102_getIR(){
	uint32_t irVal = ir_arr[ir_arrGetPtr];
	ir_arrGetPtr = (ir_arrGetPtr + 1) % arrLen;

	return irVal;
}

uint32_t max30102_getRed(){
	uint32_t redVal = red_arr[red_arrGetPtr];
	red_arrGetPtr = (red_arrGetPtr + 1) % arrLen;

	return redVal;
}

/**
 * @brief MAX30102 initiation function.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param hi2c Pointer to I2C object handle
 */
bool max30102_init(max30102_t *obj, I2C_HandleTypeDef *hi2c) {
	obj->_ui2c = hi2c;
	obj->_interrupt_flag = 0;
	memset(obj->_ir_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));
	memset(obj->_red_samples, 0, MAX30102_SAMPLE_LEN_MAX * sizeof(uint32_t));

	I2C_INIT(obj->_ui2c, FAST_MODE_400KHZ);
	max30102_ret = I2C_DeviceReady(obj->_ui2c, MAX30102_I2C_ADDR, 10);
	if (max30102_ret != BML_OK) {
		return false;
	} else {
		return true;
	}
}

/**
 * @brief Write buffer of buflen bytes to a register of the MAX30102.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to write to.
 * @param buf Pointer containing the bytes to write.
 * @param buflen Number of bytes to write.
 */

bool max30102_writeByte(max30102_t *obj, uint8_t maddr, uint8_t data) {
	max30102_ret = I2C_MasterByteWrite(obj->_ui2c, MAX30102_I2C_ADDR, maddr,
			data, 100);

	if (max30102_ret == BML_OK)
		return true;
	else
		return false;
}

bool max30102_writeMulti(max30102_t *obj, uint8_t maddr, uint8_t *data,
		uint32_t n) {
	max30102_ret = I2C_MasterMultiWrite(obj->_ui2c, MAX30102_I2C_ADDR, maddr, n,
			data, 100);

	if (max30102_ret == BML_OK)
		return true;
	else
		return false;
}

/**
 * @brief Read buflen bytes from a register of the MAX30102 and store to buffer.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param reg Register address to read from.
 * @param buf Pointer to the array to write to.
 * @param buflen Number of bytes to read.
 */
bool max30102_readByte(max30102_t *obj, uint8_t maddr, uint8_t *data) {
//	uint8_t reg_addr = reg;
//	HAL_I2C_Master_Transmit(obj->_ui2c, MAX30102_I2C_ADDR << 1, &reg_addr, 1,
//	MAX30102_I2C_TIMEOUT);
//	HAL_I2C_Master_Receive(obj->_ui2c, MAX30102_I2C_ADDR << 1, buf, buflen,
//	MAX30102_I2C_TIMEOUT);

	max30102_ret = I2C_MasterByteRead(obj->_ui2c, MAX30102_I2C_ADDR, maddr,
			data, 100);

	if (max30102_ret == BML_OK)
		return true;
	else
		return false;
}

bool max30102_readMulti(max30102_t *obj, uint8_t maddr, uint8_t *data,
		uint16_t n) {

	max30102_ret = I2C_MasterMultiRead(obj->_ui2c, MAX30102_I2C_ADDR, maddr, n,
			data, 100);

	if (max30102_ret == BML_OK)
		return true;
	else
		return false;
}

/**
 * @brief Reset the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 */
bool max30102_reset(max30102_t *obj) {
	uint8_t val = 0x40;
	return max30102_writeByte(obj, MAX30102_MODE_CONFIG, val);
}

/**
 * @brief Enable A_FULL interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
bool max30102_set_a_full(max30102_t *obj, uint8_t enable) {
	uint8_t reg = 0;
	if (max30102_readByte(obj, MAX30102_INTERRUPT_ENABLE_1, &reg) != true) {
		return false;
	}
	reg &= ~(0x01 << MAX30102_INTERRUPT_A_FULL);
	reg |= ((enable & 0x01) << MAX30102_INTERRUPT_A_FULL);
	if (max30102_writeByte(obj, MAX30102_INTERRUPT_ENABLE_1, reg) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Enable PPG_RDY interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
bool max30102_set_ppg_rdy(max30102_t *obj, uint8_t enable) {
	uint8_t reg = 0;
	if (max30102_readByte(obj, MAX30102_INTERRUPT_ENABLE_1, &reg) != true) {
		return false;
	}
	reg &= ~(0x01 << MAX30102_INTERRUPT_PPG_RDY);
	reg |= ((enable & 0x01) << MAX30102_INTERRUPT_PPG_RDY);
	if (max30102_writeByte(obj, MAX30102_INTERRUPT_ENABLE_1, reg) != true) {
		return false;
	}

	return true;
}

/**
 * @brief Enable ALC_OVF interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
bool max30102_set_alc_ovf(max30102_t *obj, uint8_t enable) {
	uint8_t reg = 0;
	if (max30102_readByte(obj, MAX30102_INTERRUPT_ENABLE_1, &reg) != true) {
		return false;
	}
	reg &= ~(0x01 << MAX30102_INTERRUPT_ALC_OVF);
	reg |= ((enable & 0x01) << MAX30102_INTERRUPT_ALC_OVF);
	if (max30102_writeByte(obj, MAX30102_INTERRUPT_ENABLE_1, reg) != true) {
		return false;
	}

	return true;
}

/**
 * @brief Enable DIE_TEMP_RDY interrupt.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
bool max30102_set_die_temp_rdy(max30102_t *obj, uint8_t enable) {
	uint8_t reg = (enable & 0x01) << MAX30102_INTERRUPT_DIE_TEMP_RDY;
	return max30102_writeByte(obj, MAX30102_INTERRUPT_ENABLE_2, reg);
}

/**
 * @brief Enable temperature measurement.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param enable Enable (1) or disable (0).
 */
bool max30102_set_die_temp_en(max30102_t *obj, uint8_t enable) {
	uint8_t reg = (enable & 0x01) << MAX30102_DIE_TEMP_EN;
	return max30102_writeByte(obj, MAX30102_DIE_TEMP_CONFIG, reg);
}

/**
 * @brief Set interrupt flag on interrupt. To be called in the corresponding external interrupt handler.
 *
 * @param obj Pointer to max30102_t object instance.
 */
void max30102_on_interrupt(max30102_t *obj) {
	obj->_interrupt_flag = 1;
}

/**
 * @brief Check whether the interrupt flag is active.
 *
 * @param obj Pointer to max30102_t object instance.
 * @return uint8_t Active (1) or inactive (0).
 */
uint8_t max30102_has_interrupt(max30102_t *obj) {
	return obj->_interrupt_flag;
}

/**
 * @brief Read interrupt status registers (0x00 and 0x01) and perform corresponding tasks.
 *
 * @param obj Pointer to max30102_t object instance.
 */
bool max30102_interrupt_handler(max30102_t *obj) {
	uint8_t reg[2] = { 0x00 };
	// Interrupt flag in registers 0x00 and 0x01 are cleared on read
//	max30102_read(obj, MAX30102_INTERRUPT_STATUS_1, reg, 2);
	if (max30102_readMulti(obj, MAX30102_INTERRUPT_STATUS_1, reg, 2) != true) {
		return false;
	}

	if ((reg[0] >> MAX30102_INTERRUPT_A_FULL) & 0x01) {
		// FIFO almost full
		if (max30102_read_fifo(obj) != true) {
			return false;
		}
	}

	if ((reg[0] >> MAX30102_INTERRUPT_PPG_RDY) & 0x01) {
		// New FIFO data ready
	}

	if ((reg[0] >> MAX30102_INTERRUPT_ALC_OVF) & 0x01) {
		// Ambient light overflow
	}

	if ((reg[1] >> MAX30102_INTERRUPT_DIE_TEMP_RDY) & 0x01) {
		// Temperature data ready
		int8_t temp_int;
		uint8_t temp_frac;
		if (max30102_read_temp(obj, &temp_int, &temp_frac) != true) {
			return false;
		}
		// float temp = temp_int + 0.0625f * temp_frac;
	}

	// Reset interrupt flag
	obj->_interrupt_flag = 0;
	return true;
}

/**
 * @brief Shutdown the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param shdn Shutdown bit.
 */
bool max30102_shutdown(max30102_t *obj, uint8_t shdn) {
	uint8_t config;
	if (max30102_readByte(obj, MAX30102_MODE_CONFIG, &config) != true) {
		return false;
	}
	config = (config & 0x7f) | (shdn << MAX30102_MODE_SHDN);
	if (max30102_writeByte(obj, MAX30102_MODE_CONFIG, config) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Set measurement mode.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param mode Measurement mode enum (max30102_mode_t).
 */
bool max30102_set_mode(max30102_t *obj, max30102_mode_t mode) {
	uint8_t config;
	if (max30102_readByte(obj, MAX30102_MODE_CONFIG, &config) != true) {
		return false;
	}
	config = (config & 0xf8) | mode;
	if (max30102_writeByte(obj, MAX30102_MODE_CONFIG, config) != true) {
		return false;
	}
	if (max30102_clear_fifo(obj) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Set sampling rate.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param sr Sampling rate enum (max30102_spo2_st_t).
 */
bool max30102_set_sampling_rate(max30102_t *obj, max30102_sr_t sr) {
	uint8_t config;
	if (max30102_readByte(obj, MAX30102_SPO2_CONFIG, &config) != true) {
		return false;
	}
	config = (config & 0x63) << MAX30102_SPO2_SR;
	if (max30102_writeByte(obj, MAX30102_SPO2_CONFIG, config) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Set led pulse width.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param pw Pulse width enum (max30102_led_pw_t).
 */
bool max30102_set_led_pulse_width(max30102_t *obj, max30102_led_pw_t pw) {
	uint8_t config;
	if (max30102_readByte(obj, MAX30102_SPO2_CONFIG, &config) != true) {
		return false;
	}
	config = (config & 0x7c) | (pw << MAX30102_SPO2_LEW_PW);
	if (max30102_writeByte(obj, MAX30102_SPO2_CONFIG, config) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Set ADC resolution.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param adc ADC resolution enum (max30102_adc_t).
 */
bool max30102_set_adc_resolution(max30102_t *obj, max30102_adc_t adc) {
	uint8_t config;
	if (max30102_readByte(obj, MAX30102_SPO2_CONFIG, &config) != true) {
		return false;
	}
	config = (config & 0x1f) | (adc << MAX30102_SPO2_ADC_RGE);
	if (max30102_writeByte(obj, MAX30102_SPO2_CONFIG, config) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Set LED current.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param ma LED current float (0 < ma < 51.0).
 */
bool max30102_set_led_current_1(max30102_t *obj, uint8_t ma) {
//	uint8_t pa = ma / 0.2;
	  //LED Pulse Amplitude Configuration
	  //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
	  //Default is 0x1F which gets us 6.4mA
	  //powerLevel = 0x02, 0.4mA - Presence detection of ~4 inch
	  //powerLevel = 0x1F, 6.4mA - Presence detection of ~8 inch
	  //powerLevel = 0x7F, 25.4mA - Presence detection of ~8 inch
	  //powerLevel = 0xFF, 50.0mA - Presence detection of ~12 inch
	return max30102_writeByte(obj, MAX30102_LED_IR_PA1, ma);
}

/**
 * @brief Set LED current.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param ma LED current float (0 < ma < 51.0).
 */
bool max30102_set_led_current_2(max30102_t *obj, uint8_t ma) {
//	uint8_t pa = ma / 0.2;
	return max30102_writeByte(obj, MAX30102_LED_RED_PA2, ma);
}

/**
 * @brief Set slot mode when in multi-LED mode.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param slot1 Slot 1 mode enum (max30102_multi_led_ctrl_t).
 * @param slot2 Slot 2 mode enum (max30102_multi_led_ctrl_t).
 */
bool max30102_set_multi_led_slot_1_2(max30102_t *obj,
		max30102_multi_led_ctrl_t slot1, max30102_multi_led_ctrl_t slot2) {
	uint8_t val = 0;
	val |= ((slot1 << MAX30102_MULTI_LED_CTRL_SLOT1)
			| (slot2 << MAX30102_MULTI_LED_CTRL_SLOT2));
	return max30102_writeByte(obj, MAX30102_MULTI_LED_CTRL_1, val);
}

/**
 * @brief Set slot mode when in multi-LED mode.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param slot1 Slot 1 mode enum (max30102_multi_led_ctrl_t).
 * @param slot2 Slot 2 mode enum (max30102_multi_led_ctrl_t).
 */
bool max30102_set_multi_led_slot_3_4(max30102_t *obj,
		max30102_multi_led_ctrl_t slot3, max30102_multi_led_ctrl_t slot4) {
	uint8_t val = 0;
	val |= ((slot3 << MAX30102_MULTI_LED_CTRL_SLOT3)
			| (slot4 << MAX30102_MULTI_LED_CTRL_SLOT4));
	if (max30102_writeByte(obj, MAX30102_MULTI_LED_CTRL_2, val) != true) {
		return false;
	}

	return true;
}

/**
 * @brief
 *
 * @param obj Pointer to max30102_t object instance.
 * @param smp_ave
 * @param roll_over_en Roll over enabled(1) or disabled(0).
 * @param fifo_a_full Number of empty samples when A_FULL interrupt issued (0 < fifo_a_full < 15).
 */
bool max30102_set_fifo_config(max30102_t *obj, max30102_smp_ave_t smp_ave,
		uint8_t roll_over_en, uint8_t fifo_a_full) {
	uint8_t config = 0x00;
	config |= smp_ave << MAX30102_FIFO_CONFIG_SMP_AVE;
	config |= ((roll_over_en & 0x01) << MAX30102_FIFO_CONFIG_ROLL_OVER_EN);
	config |= ((fifo_a_full & 0x0f) << MAX30102_FIFO_CONFIG_FIFO_A_FULL);
	if (max30102_writeByte(obj, MAX30102_FIFO_CONFIG, config) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Clear all FIFO pointers in the sensor.
 *
 * @param obj Pointer to max30102_t object instance.
 */
bool max30102_clear_fifo(max30102_t *obj) {
	uint8_t val[4] = { 0x00, 0x00, 0x00, 0x00 };
	if (max30102_writeMulti(obj, MAX30102_FIFO_WR_PTR, &val, 3) != true) {
		return false;
	}
	if (max30102_writeMulti(obj, MAX30102_FIFO_RD_PTR, &val, 3) != true) {
		return false;
	}
	if (max30102_writeMulti(obj, MAX30102_OVF_COUNTER, &val, 3) != true) {
		return false;
	}
	return true;
}

/**
 * @brief Read FIFO content and store to buffer in max30102_t object instance.
 *
 * @param obj Pointer to max30102_t object instance.
 */
bool max30102_read_fifo(max30102_t *obj) {
	// First transaction: Get the FIFO_WR_PTR
	uint8_t wr_ptr = 0, rd_ptr = 0;
	if (max30102_readByte(obj, MAX30102_FIFO_WR_PTR, &wr_ptr) != true) {
		return false;
	}
	if (max30102_readByte(obj, MAX30102_FIFO_RD_PTR, &rd_ptr) != true) {
		return false;
	}

	int8_t num_samples;
//	rb_data_type value;

	num_samples = (int8_t) wr_ptr - (int8_t) rd_ptr;
	if (num_samples < 1) {
		num_samples += 32;
	}

	// Second transaction: Read NUM_SAMPLES_TO_READ samples from the FIFO
	for (int8_t i = 0; i < num_samples; i++) {
		uint8_t sample[6];
//		max30102_read(obj, MAX30102_FIFO_DATA, sample, 6);
		if(max30102_readMulti(obj, MAX30102_FIFO_DATA, sample, 6) != true){
			return false;
		}
		uint32_t ir_sample = ((uint32_t) (sample[0] << 16)
				| (uint32_t) (sample[1] << 8) | (uint32_t) (sample[3]))
				& 0x3ffff;
		uint32_t red_sample = ((uint32_t) (sample[3] << 16)
				| (uint32_t) (sample[4] << 8) | (uint32_t) (sample[5]))
				& 0x3ffff;
		obj->_ir_samples[i] = ir_sample;
		obj->_red_samples[i] = red_sample;
//		value.ir = obj->_ir_samples[i];
//		value.red = obj->_red_samples[i];
//		rb_push(value);
		max30102_plot(ir_sample, red_sample);
	}
	return true;
}

bool max30102_update(max30102_t *obj) {
	return max30102_read_fifo(obj);
}

/**
 * @brief Read die temperature.
 *
 * @param obj Pointer to max30102_t object instance.
 * @param temp_int Pointer to store the integer part of temperature. Stored in 2's complement format.
 * @param temp_frac Pointer to store the fractional part of temperature. Increments of 0.0625 deg C.
 */

bool max30102_read_temp(max30102_t *obj, int8_t *temp_int, uint8_t *temp_frac) {
	if (max30102_readByte(obj, MAX30102_DIE_TINT, temp_int) != true) {
		return false;
	}
	if (max30102_readByte(obj, MAX30102_DIE_TFRAC, temp_frac) != true) {
		return false;
	}
	return true;
}

