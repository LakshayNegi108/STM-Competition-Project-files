#include "main.h"
#include "BML_DEF.h"
#include "rcc.h"

int main() {

	RCC_CONFIG_84MHZ();

	gpio_config(PORTA, 5, OUTPUT_MODE, VERY_HIGH_SPEED, DI_PUPD, PHPL);
	gpio_config(PORTC, 13, INPUT_MODE, LOW_SPEED, EN_PU, PHPL);

	uart_print_config(9600);
	print("Working\n");

	while (1) {
		if (gpio_read(PORTC, 13) == LOW) {
			gpio_write(PORTA, 5, HIGH);
			Delay(250);
			gpio_write(PORTA, 5, LOW);
			Delay(250);
		}
		print("Running\n");

	}
	return 0;
}
