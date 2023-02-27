#include "main.h"

#include "BML_DEF.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "i2c.h"
#include "Max30102.h"

BML_StatusTypeDef ret;
// MAX30102 object
max30102_t max30102;

uint16_t it = 0;

#define SCL_PORT	PORTB
#define SCL_PIN		8

#define SDA_PORT	PORTB
#define SDA_PIN		9

#define RATE_SIZE  8 //Increase this for more averaging. 4 is good.
uint8_t rates[RATE_SIZE]; //Array of heart rates
uint8_t rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

uint8_t beatsPerMinute;
int beatAvg;

void I2C_GPIOConfig();
void max30102_startSeq();

int main(void) {
	RCC_CONFIG_48MHZ();
	uart_print_config(115200);
	print("Working\n");

	I2C_GPIOConfig();

	gpio_config(PORTC, 4, INPUT_MODE, HIGH_SPEED, EN_PU, PHPL);
	gpio_IT_config(PORTC, 4, FALLING_EDGE);
	gpio_IT_EN(4, EXTI4_15_IRQn);
	max30102_startSeq();

	uint32_t ir = 0;
	uint16_t red = 0;

	uint32_t start = 0, end = 0;

	while (1) {

		end = HAL_GetTick();

//		if (max30102_has_interrupt(&max30102)) {
//			if (max30102_interrupt_handler(&max30102) != true) {
//				max30102_startSeq();
//			}
//		}
//		uint16_t start = HAL_GetTick();
		if(max30102_update(&max30102) != true){
			max30102_startSeq();
			Delay(1000);
		}
//		uint16_t end = HAL_GetTick();
//		print("Elapsed Time: %dms\n", end - start);
		ir = max30102_getIR();
//		red = max30102_getRed();
//		print("%d\n", ir);
//		print("%d, %d\n", ir, red);

		if (checkForBeat(ir) == true && (ir > 10000)) {
			//We sensed a beat!
			long delta = HAL_GetTick() - lastBeat;
			lastBeat = HAL_GetTick();

			beatsPerMinute = 60 / (delta / 1000.0);

			if (beatsPerMinute < 255 && beatsPerMinute > 20) {
				rates[rateSpot++] = (uint8_t) beatsPerMinute; //Store this reading in the array
				rateSpot %= RATE_SIZE; //Wrap variable

				//Take average of readings
				beatAvg = 0;
				for (uint8_t x = 0; x < RATE_SIZE; x++)
					beatAvg += rates[x];
				beatAvg /= RATE_SIZE;
			}
		}

		if (end - start > 500) {
			print("IR= %d, BPM = %d, Avg BPM = %d ", ir, beatsPerMinute,
					beatAvg);

			if (ir < 10000) {
				print("No Finger?");
				beatsPerMinute = 0;
				beatAvg = 0;
			}
			print(" \n\r");

			start = end;
		}

	}

	return 0;
}

void I2C_GPIOConfig() {
	gpio_config(SCL_PORT, SCL_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);
	gpio_config(SDA_PORT, SDA_PIN, ALT_MODE, HIGH_SPEED, EN_PU, OD);

	gpio_altfn(SCL_PORT, SCL_PIN, I2C1_SCL_PB8);
	gpio_altfn(SDA_PORT, SDA_PIN, I2C1_SDA_PB9);
}

void max30102_startSeq() {
	if (max30102_init(&max30102, I2C1)) {
		print("Success\n\r");
	} else {
		print("Device Error\n\r");
	}
	max30102_reset(&max30102);
	max30102_clear_fifo(&max30102);
	max30102_set_fifo_config(&max30102, max30102_smp_ave_16, 1, 7);

	uint8_t tempInt, tempFrac;
	max30102_read_temp(&max30102, &tempInt, &tempFrac);
	print("Temp: %d.%d\n\r", tempInt, tempFrac);

	// Sensor settings
	max30102_set_led_pulse_width(&max30102, max30102_pw_16_bit);
	max30102_set_adc_resolution(&max30102, max30102_adc_8192);
	max30102_set_sampling_rate(&max30102, max30102_sr_800);
	max30102_set_led_current_1(&max30102, 0x20);
	max30102_set_led_current_2(&max30102, 0x20);

	// Enter SpO2 mode
	max30102_set_mode(&max30102, max30102_spo2);
	max30102_set_a_full(&max30102, 1);

	// Initiate 1 temperature measurement
	max30102_set_die_temp_en(&max30102, 1);
	max30102_set_die_temp_rdy(&max30102, 1);

	uint8_t en_reg[2] = { 0 };
	max30102_readByte(&max30102, 0x00, en_reg);
}

void EXTI4_15_IRQHandler(void) {

	if (gpio_IT_CHK(4)) {
		print("Interrupt Happened\n\r");
		max30102_on_interrupt(&max30102);
		gpio_IT_CLR(4);
	}
}
