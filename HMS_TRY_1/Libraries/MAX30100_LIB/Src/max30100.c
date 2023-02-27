/*
 * max30100.c
 *
 *  Created on: Dec 25, 2022
 *      Author: hp
 */
#include "max30100.h"
#include "registers.h"

HAL_StatusTypeDef max30100_init(max30100_config_t *this, I2C_HandleTypeDef *hi2c,
		max30100_mode_t mode, max30100_sampling_rate_t sampling_rate,
		max30100_pulse_width_t pulse_width, max30100_current_t ir_current,
		max30100_current_t start_red_current, uint8_t mean_filter_size,
		uint8_t pulse_bpm_sample_size,
		bool high_res_mode,
		bool debug) {
	this->_ui2c = hi2c;
	if (HAL_I2C_IsDeviceReady(this->_ui2c, MAX30100_DEVICE, 2, 100) != HAL_OK) {
			return HAL_ERROR;
		}

	this->acceptable_intense_diff = MAX30100_DEFAULT_ACCEPTABLE_INTENSITY_DIFF;
	this->red_current_adj_ms = MAX30100_DEFAULT_RED_LED_CURRENT_ADJUSTMENT_MS;
	this->reset_spo2_pulse_n = MAX30100_DEFAULT_RESET_SPO2_EVERY_N_PULSES;
	this->dc_alpha = MAX30100_DEFAULT_ALPHA;
	this->pulse_min_threshold = MAX30100_DEFAULT_PULSE_MIN_THRESHOLD;
	this->pulse_max_threshold = MAX30100_DEFAULT_PULSE_MAX_THRESHOLD;

	this->mean_filter_size = mean_filter_size;
	this->pulse_bpm_sample_size = pulse_bpm_sample_size;

	this->debug = debug;
	this->current_pulse_detector_state = MAX30100_PULSE_IDLE;

	this->mean_diff_ir.values = NULL;
	this->values_bpm = NULL;
	this->mean_diff_ir.values = malloc(sizeof(float) * mean_filter_size);
	this->values_bpm = malloc(sizeof(float) * pulse_bpm_sample_size);

	if (!(this->values_bpm) || !(this->mean_diff_ir.values))
		return HAL_ERROR;

	HAL_StatusTypeDef ret = max30100_set_mode(this, mode);
	if (ret != HAL_OK)
		return ret;

	ret = max30100_set_sampling_rate(this, sampling_rate);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_set_pulse_width(this, pulse_width);
	if (ret != HAL_OK)
		return ret;

	this->red_current = (uint8_t) start_red_current;
	this->last_red_current_check = 0;

	this->ir_current = ir_current;
	ret = max30100_set_led_current(this, this->red_current, ir_current);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_set_high_res(this, high_res_mode);
	if (ret != HAL_OK)
		return ret;

	this->dc_filter_ir.w = 0;
	this->dc_filter_ir.result = 0;

	this->dc_filter_red.w = 0;
	this->dc_filter_red.result = 0;

	this->lpb_filter_ir.v[0] = 0;
	this->lpb_filter_ir.v[1] = 0;
	this->lpb_filter_ir.result = 0;

	memset(this->mean_diff_ir.values, 0, sizeof(float) * mean_filter_size);
	this->mean_diff_ir.index = 0;
	this->mean_diff_ir.sum = 0;
	this->mean_diff_ir.count = 0;

	memset(this->values_bpm, 0, sizeof(float) * pulse_bpm_sample_size);
	this->values_bpm_sum = 0;
	this->values_bpm_count = 0;
	this->bpm_index = 0;

	this->ir_ac_sq_sum = 0;
	this->red_ac_sq_sum = 0;
	this->samples_recorded = 0;
	this->pulses_detected = 0;
	this->current_spO2 = 0;

	this->last_beat_threshold = 0;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_update(max30100_config_t *this,
		max30100_data_t *data) {
	data->pulse_detected = false;
	data->heart_bpm = 0.0;
	data->ir_cardiogram = 0.0;
	data->ir_dc_value = 0.0;
	data->red_dc_value = 0.0;
	data->spO2 = this->current_spO2;
	data->last_beat_threshold = 0;
	data->dc_filtered_ir = 0.0;
	data->dc_filtered_red = 0.0;

	max30100_fifo_t raw_data;
	HAL_StatusTypeDef ret = max30100_read_fifo(this, &raw_data);
//=================================================
//	print("IR: %d, Red: %d\n", raw_data.raw_ir, raw_data.raw_red);
//=================================================
	if (ret != HAL_OK)
		return ret;

	this->dc_filter_ir = max30100_dc_removal((float) raw_data.raw_ir,
			this->dc_filter_ir.w, this->dc_alpha);
	this->dc_filter_red = max30100_dc_removal((float) raw_data.raw_red,
			this->dc_filter_red.w, this->dc_alpha);

	float mean_diff_res_ir = max30100_mean_diff(this,
			this->dc_filter_ir.result);
	//=================================================
//	print("IR mean difference: %f\n", mean_diff_res_ir);
	//=================================================

	max30100_lpb_filter(this, mean_diff_res_ir/*-dcFilterIR.result*/);

	//=================================================
//	print("lpb_filter_ir: %f\n", this->lpb_filter_ir.result);
	//=================================================

	this->ir_ac_sq_sum += this->dc_filter_ir.result * this->dc_filter_ir.result;
	this->red_ac_sq_sum += this->dc_filter_red.result * this->dc_filter_red.result;
	this->samples_recorded++;

	if (max30100_detect_pulse(this, this->lpb_filter_ir.result) && this->samples_recorded) {
		data->pulse_detected = true;
		this->pulses_detected++;

		float ratio_rms = log(sqrt(this->red_ac_sq_sum / (float) this->samples_recorded))
				/ log(sqrt(this->ir_ac_sq_sum / (float) this->samples_recorded));

		if (this->debug)
			print("RMS Ratio: %f\n", ratio_rms);

		//This is my adjusted standard model, so it shows 0.89 as 94% saturation.
		//It is probably far from correct, requires proper empircal calibration.
		this->current_spO2 = 110.0 - 16.0 * ratio_rms;
		data->spO2 = this->current_spO2;

		if (!(this->pulses_detected % this->reset_spo2_pulse_n)) {
			this->ir_ac_sq_sum = 0;
			this->red_ac_sq_sum = 0;
			this->samples_recorded = 0;
		}
	}

	ret = max30100_balance_intensities(this, this->dc_filter_red.w,
			this->dc_filter_ir.w);
	if (ret != HAL_OK)
		return ret;

	data->heart_bpm = this->current_bpm;
	data->ir_cardiogram = this->lpb_filter_ir.result;
	data->ir_dc_value = this->dc_filter_ir.w;
	data->red_dc_value = this->dc_filter_red.w;
	data->last_beat_threshold = this->last_beat_threshold;
	data->dc_filtered_ir = this->dc_filter_ir.result;
	data->dc_filtered_red = this->dc_filter_red.result;

	return HAL_OK;
}

bool max30100_detect_pulse(max30100_config_t *this, float sensor_value) {
	static float prev_sensor_value = 0;
	static uint8_t values_went_down = 0;
	static uint32_t current_beat = 0;
	static uint32_t last_beat = 0;

	if (sensor_value > this->pulse_max_threshold) {
		this->current_pulse_detector_state = MAX30100_PULSE_IDLE;
		prev_sensor_value = 0;
		last_beat = 0;
		current_beat = 0;
		values_went_down = 0;
		this->last_beat_threshold = 0;
		return false;
	}

	switch (this->current_pulse_detector_state) {
	case MAX30100_PULSE_IDLE:
		if (sensor_value >= this->pulse_min_threshold) {
			this->current_pulse_detector_state = MAX30100_PULSE_TRACE_UP;
			values_went_down = 0;
		}
		break;
	case MAX30100_PULSE_TRACE_UP:
		if (sensor_value > prev_sensor_value) {
			current_beat = HAL_GetTick();
			this->last_beat_threshold = sensor_value;
		} else {
			if (this->debug) {
				print("Peak reached: %f %f\n", sensor_value, prev_sensor_value);
			}

			uint32_t beat_duration = current_beat - last_beat;
			last_beat = current_beat;

			float raw_bpm = 0;
			if (beat_duration)
				raw_bpm = 60000.0 / (float) beat_duration;

			if (this->debug) {
				print("Beat duration: %u\n", beat_duration);
				print("Raw BPM: %f\n", raw_bpm);
			}

			this->current_pulse_detector_state = MAX30100_PULSE_TRACE_DOWN;

			// Reset filter after a while without pulses
			if (beat_duration > 2500) { // 2.5 seconds
				memset(this->values_bpm, 0,
						sizeof(float) * this->pulse_bpm_sample_size);
				this->values_bpm_sum = 0;
				this->values_bpm_count = 0;
				this->bpm_index = 0;

				if (this->debug)
					print("Moving avg. reseted\n");
			}

			// Test if out of bounds
			if (raw_bpm < 50 || raw_bpm > 220) {
				if (this->debug)
					print("BPM out of bounds. Not adding to Moving Avg.\n");

				return false;
			}

			// Optimized filter
			this->values_bpm_sum -= this->values_bpm[this->bpm_index];
			this->values_bpm[this->bpm_index] = raw_bpm;
			this->values_bpm_sum += this->values_bpm[this->bpm_index++];

			this->bpm_index %= this->pulse_bpm_sample_size;

			if (this->values_bpm_count < this->pulse_bpm_sample_size)
				this->values_bpm_count++;

			this->current_bpm = this->values_bpm_sum / this->values_bpm_count;

			if (this->debug) {
				print("CurrentMoving Avg: ");

				for (int i = 0; i < this->values_bpm_count; i++)
					print("%f ", this->values_bpm[i]);

				print(" \n");
				print("AVg. BPM: %f\n", this->current_bpm);
			}

			return true;
		}
		break;
	case MAX30100_PULSE_TRACE_DOWN:
		if (sensor_value < prev_sensor_value)
			values_went_down++;

		if (sensor_value < this->pulse_min_threshold)
			this->current_pulse_detector_state = MAX30100_PULSE_IDLE;

		break;
	}

	prev_sensor_value = sensor_value;
	return false;
}

HAL_StatusTypeDef max30100_balance_intensities(max30100_config_t *this,
		float red_dc, float ir_dc) {
	if ((uint32_t) (HAL_GetTick()) - this->last_red_current_check
			>= this->red_current_adj_ms) {
		//print("%f\n", red_dc - ir_dc);
		if (ir_dc - red_dc > this->acceptable_intense_diff
				&& this->red_current < MAX30100_LED_CURRENT_50MA) {
			this->red_current++;
			HAL_StatusTypeDef ret = max30100_set_led_current(this,
					this->red_current, this->ir_current);
			if (ret != HAL_OK)
				return ret;
			if (this->debug)
				print("RED LED Current +\n");

		} else if (red_dc - ir_dc > this->acceptable_intense_diff
				&& this->red_current > 0) {
			this->red_current--;
			HAL_StatusTypeDef ret = max30100_set_led_current(this,
					this->red_current, this->ir_current);
			if (ret != HAL_OK)
				return ret;
			if (this->debug)
				print("RED LED Current -\n");
		}

		this->last_red_current_check = (uint32_t) (HAL_GetTick());

	}
	return HAL_OK;
}

// Writes val to address register on device
HAL_StatusTypeDef max30100_write_register(max30100_config_t *this,
		uint8_t address, uint8_t val) {
	// start transmission to device
//	uint8_t *pVal = &val;
//	return HAL_I2C_Mem_Write(this->_ui2c, MAX30100_DEVICE, address, I2C_MEMADD_SIZE_8BIT, pVal, 1, 10);

	uint8_t pVal[2] = {address, val};
	return HAL_I2C_Master_Transmit(this->_ui2c, MAX30100_DEVICE, pVal, 2, 100);
}

HAL_StatusTypeDef max30100_read_register(max30100_config_t *this,
		uint8_t address, uint8_t *reg) {
//	return I2C_MasterByteRead(this->_ui2c, MAX30100_DEVICE, address, reg,
//			100);
	return HAL_I2C_Mem_Read(this->_ui2c, MAX30100_DEVICE, address, I2C_MEMADD_SIZE_8BIT, reg, 1, 100);
}

// Reads num bytes starting from address register on device in to _buff array
HAL_StatusTypeDef max30100_read_from(max30100_config_t *this, uint8_t address,
		uint8_t *reg, uint8_t size) {
	if (!size)
		return HAL_OK;

//	return I2C_MasterMultiRead(this->_ui2c, MAX30100_DEVICE, address, size,
//			reg, 100);
	return HAL_I2C_Mem_Read(this->_ui2c, MAX30100_DEVICE, address, I2C_MEMADD_SIZE_8BIT, reg, size, 100);
}

HAL_StatusTypeDef max30100_set_mode(max30100_config_t *this,
		max30100_mode_t mode) {
	uint8_t current_mode_reg;
	//Tratar erros
	HAL_StatusTypeDef ret = max30100_read_register(this, MAX30100_MODE_CONF,
			&current_mode_reg);
	if (ret != HAL_OK)
		return ret;
	return max30100_write_register(this, MAX30100_MODE_CONF,
			(current_mode_reg & 0xF8) | mode);
}

HAL_StatusTypeDef max30100_set_high_res(max30100_config_t *this, bool enabled) {
	uint8_t previous;

	//Tratar erros
	HAL_StatusTypeDef ret = max30100_read_register(this, MAX30100_SPO2_CONF,
			&previous);
	if (ret != HAL_OK)
		return ret;
	if (enabled) {
		return max30100_write_register(this, MAX30100_SPO2_CONF,
				previous | MAX30100_SPO2_HI_RES_EN);
	} else {
		return max30100_write_register(this, MAX30100_SPO2_CONF,
				previous & ~MAX30100_SPO2_HI_RES_EN);
	}
}

HAL_StatusTypeDef max30100_set_sampling_rate(max30100_config_t *this,
		max30100_sampling_rate_t rate) {
	uint8_t current_spO2_reg;

	//Tratar erros
	HAL_StatusTypeDef ret = max30100_read_register(this, MAX30100_SPO2_CONF,
			&current_spO2_reg);
	if (ret != HAL_OK)
		return ret;
	return max30100_write_register(this, MAX30100_SPO2_CONF,
			(current_spO2_reg & 0xE3) | (rate << 2));
}

HAL_StatusTypeDef max30100_set_pulse_width(max30100_config_t *this,
		max30100_pulse_width_t pw) {
	uint8_t current_spO2_reg;

	//Tratar erros
	HAL_StatusTypeDef ret = max30100_read_register(this, MAX30100_SPO2_CONF,
			&current_spO2_reg);
	if (ret != HAL_OK)
		return ret;
	return max30100_write_register(this, MAX30100_SPO2_CONF,
			(current_spO2_reg & 0xFC) | pw);
}

HAL_StatusTypeDef max30100_set_led_current(max30100_config_t *this,
		max30100_current_t red_current, max30100_current_t ir_current) {
	//Tratar erros
	return max30100_write_register(this, MAX30100_LED_CONF,
			(red_current << 4) | ir_current);
}

HAL_StatusTypeDef max30100_set_acceptable_intense_difff(max30100_config_t *this,
		uint32_t acceptable_intense_diff) {
	//Add possible error check
	this->acceptable_intense_diff = acceptable_intense_diff;
	return HAL_OK;
}
HAL_StatusTypeDef max30100_set_red_current_adj_ms(max30100_config_t *this,
		uint32_t red_current_adj_ms) {
	//Add possible error check
	this->red_current_adj_ms = red_current_adj_ms;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_set_reset_spo2_pulse_n(max30100_config_t *this,
		uint8_t reset_spo2_pulse_n) {
	//Add possible error check
	this->reset_spo2_pulse_n = reset_spo2_pulse_n;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_set_dc_alpha(max30100_config_t *this, float dc_alpha) {
	//Add possible error check
	this->dc_alpha = dc_alpha;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_set_pulse_min_threshold(max30100_config_t *this,
		uint16_t pulse_min_threshold) {
	//Add possible error check
	this->pulse_min_threshold = pulse_min_threshold;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_set_pulse_max_threshold(max30100_config_t *this,
		uint16_t pulse_max_threshold) {
	//Add possible error check
	this->pulse_max_threshold = pulse_max_threshold;
	return HAL_OK;
}

HAL_StatusTypeDef max330100_read_temperature(max30100_config_t *this,
		float *temperature) {
	uint8_t current_mode_reg;
	//Tratar erros
	HAL_StatusTypeDef ret = max30100_read_register(this, MAX30100_MODE_CONF,
			&current_mode_reg);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_write_register(this, MAX30100_MODE_CONF,
			current_mode_reg | MAX30100_MODE_TEMP_EN);
	if (ret != HAL_OK)
		return ret;
	//This can be changed to a while loop, (with interrupt flag!)
	//there is an interrupt flag for when temperature has been read.
	HAL_Delay(100);

	int8_t temp;
	//Tratar erros
	ret = max30100_read_register(this, MAX30100_TEMP_INT, (uint8_t*) &temp);
	if (ret != HAL_OK)
		return ret;

	float temp_fraction;
	ret = max30100_read_register(this, MAX30100_TEMP_FRACTION,
			(uint8_t*) &temp_fraction);
	if (ret != HAL_OK)
		return ret;
	temp_fraction *= 0.0625;
	*temperature = (float) temp + temp_fraction;
	return HAL_OK;
}

HAL_StatusTypeDef max30100_read_fifo(max30100_config_t *this,
		max30100_fifo_t *fifo) {
	uint8_t buffer[4];
	//Testar erros
	HAL_StatusTypeDef ret = max30100_read_from(this, MAX30100_FIFO_DATA, buffer,
			4);
	if (ret != HAL_OK)
		return ret;
	fifo->raw_ir = ((uint16_t) buffer[0] << 8) | buffer[1];
	fifo->raw_red = ((uint16_t) buffer[2] << 8) | buffer[3];

	return HAL_OK;
}

max30100_dc_filter_t max30100_dc_removal(float x, float prev_w, float alpha) {
	max30100_dc_filter_t filtered = { };
	filtered.w = x + alpha * prev_w;
	filtered.result = filtered.w - prev_w;

	return filtered;
}

void max30100_lpb_filter(max30100_config_t *this, float x) {
	this->lpb_filter_ir.v[0] = this->lpb_filter_ir.v[1];

	//Fs = 100Hz and Fc = 10Hz
	this->lpb_filter_ir.v[1] = (2.452372752527856026e-1 * x)
			+ (0.50952544949442879485 * this->lpb_filter_ir.v[0]);

	//Fs = 100Hz and Fc = 4Hz
	//this->lpb_filter_ir.v[1] = (1.367287359973195227e-1 * x)
	//                   + (0.72654252800536101020 * this->lpb_filter_ir.v[0]);
	//Very precise butterworth filter

	this->lpb_filter_ir.result = this->lpb_filter_ir.v[0]
			+ this->lpb_filter_ir.v[1];
}

float max30100_mean_diff(max30100_config_t *this, float M) {
	float avg = 0;

	this->mean_diff_ir.sum -=
			this->mean_diff_ir.values[this->mean_diff_ir.index];
	this->mean_diff_ir.values[this->mean_diff_ir.index] = M;
	this->mean_diff_ir.sum +=
			this->mean_diff_ir.values[this->mean_diff_ir.index++];

	this->mean_diff_ir.index = this->mean_diff_ir.index
			% this->mean_filter_size;

	if (this->mean_diff_ir.count < this->mean_filter_size)
		this->mean_diff_ir.count++;

	avg = this->mean_diff_ir.sum / this->mean_diff_ir.count;
	return avg - M;
}

HAL_StatusTypeDef max30100_print_registers(max30100_config_t *this) {
	uint8_t int_status, int_enable, fifo_write, fifo_ovf_cnt, fifo_read;
	uint8_t fifo_data, mode_conf, sp02_conf, led_conf, temp_int, temp_frac;
	uint8_t rev_id, part_id;
	HAL_StatusTypeDef ret;

	ret = max30100_read_register(this, MAX30100_INT_STATUS, &int_status);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_INT_ENABLE, &int_enable);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_FIFO_WRITE, &fifo_write);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_FIFO_OVERFLOW_COUNTER,
			&fifo_ovf_cnt);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_FIFO_READ, &fifo_read);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_FIFO_DATA, &fifo_data);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_MODE_CONF, &mode_conf);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_SPO2_CONF, &sp02_conf);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_LED_CONF, &led_conf);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_TEMP_INT, &temp_int);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_TEMP_FRACTION, &temp_frac);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_REV_ID, &rev_id);
	if (ret != HAL_OK)
		return ret;
	ret = max30100_read_register(this, MAX30100_PART_ID, &part_id);
	if (ret != HAL_OK)
		return ret;

	print("%x\n", int_status);
	print("%x\n", int_enable);
	print("%x\n", fifo_write);
	print("%x\n", fifo_ovf_cnt);
	print("%x\n", fifo_read);
	print("%x\n", fifo_data);
	print("%x\n", mode_conf);
	print("%x\n", sp02_conf);
	print("%x\n", led_conf);
	print("%x\n", temp_int);
	print("%x\n", temp_frac);
	print("%x\n", rev_id);
	print("%x\n", part_id);

	return HAL_OK;
}

