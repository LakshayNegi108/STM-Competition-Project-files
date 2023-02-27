#include "main.h"
#include "BML_DEF.h"
#include "rcc.h"

uint8_t flag = 0;

int main() {

	RCC_CONFIG_84MHZ();

	gpio_config(PORTA, 5, OUTPUT_MODE, VERY_HIGH_SPEED, DI_PUPD, PHPL);
	gpio_config(PORTC, 13, INPUT_MODE, LOW_SPEED, EN_PU, PHPL);
	gpio_IT_config(PORTC, 13, FALLING_EDGE);
	gpio_IT_EN(13, EXTI15_10_IRQn);

	while (1) {
		if (flag == HIGH) {
			gpio_write(PORTA, 5, HIGH);
			Delay(250);
			gpio_write(PORTA, 5, LOW);
			Delay(250);
		}

	}
	return 0;
}

void EXTI15_10_IRQHandler(){
	if(gpio_IT_CHK(13)){
		flag ^= 1;
		gpio_IT_CLR(13);
	}
}
