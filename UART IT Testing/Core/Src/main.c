#include "main.h"
#include "rcc.h"
#include "BML_DEF.h"
#include "uart.h"

char str[50] = { 0 };

int main(void){

	RCC_CONFIG_84MHZ();
	uart_print_config(9600);
	print("Working\n\r");

	gpio_config(PORTA, 5, OUTPUT_MODE, HIGH_SPEED, DI_PUPD, PHPL);

	USART_INIT(USART6, PORTA, PORTA, 11, 12, 115200);

	USART_WRITE(USART6, "AT+CWMODE=1\r\n", 13, 100);
	sprintf(str, "AT+CWJAP=\"%s\",\"%s\"\r\n", "realme GT Master Edition", "9999161397");
	USART_WRITE(USART6, str, strlen(str), 10);
	USART_WRITE(USART6, "AT+CIFSR\r\n", 10, 10);
	USART_WRITE(USART6, "AT+CIPMUX=1\r\n", 13, 10);
	USART_WRITE(USART6, "AT+CIPSERVER=1,80\r\n", 19, 10);
	print("Now Connect to the IP ADRESS\n\n");

	while(1){

		char datatosend[1024] = { 0 };
		sprintf(datatosend, Basic_inclusion);
				strcat(datatosend, LED_ON);
				strcat(datatosend, Terminate);
				Server_Send(datatosend, Link_ID);

		gpio_toggle(PORTA, 5);
		Delay(500);
	}

	return 0;
}
