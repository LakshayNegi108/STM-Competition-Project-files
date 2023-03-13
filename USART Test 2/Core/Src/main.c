#include "main.h"
#include "BML_DEF.h"
#include "rcc.h"
#include "uart.h"
#include "ringbuffer.h"
#include "ESP8266.h"

char str[2050] = { 0 };
int idx = 0;
char command[100] = { 0 };

bool espInit = false;
bool espClient = false;
bool espClose = false;
bool clientRecFlag = false;

bool usartBusy = false;
bool usartIdle = false;

ring_buffer_t ringBuffer;

char host[] = "api.thingspeak.com";
char ssid[] = "realme GT Master Edition";
char password[] = "9999161397";

char api[] = "NDLZ3M336U8E039S";
char tempField[] = "field1";
char heartRateField[] = "field2";

uint16_t numItem = 0;
char strCache[100] = { 0 };
float temp[5] = { 34.67, 32.07, 28, 26.02, 25.67, 29.09 };
int hr[5] = { 71, 82, 79, 76, 75 };
int i = 0;

uint32_t start = 0;

bool esp_init();

void thingspeak_send();

int main(void) {
	RCC_CONFIG_84MHZ();

	uart_print_config(115200);
	USART_IT_EN(USART2, 0, 1, 0);
	print("<=====================|Working|=======================>\n\r");

	gpio_config(PORTA, 5, OUTPUT_MODE, HIGH_SPEED, DI_PUPD, PHPL);

	USART_INIT(USART6, PORTA, PORTA, 11, 12, 115200);
	USART_IT_EN(USART6, 0, 1, 1);

	ring_buffer_init(&ringBuffer);

	print("Starting ESP\r\n");

	esp_init();
	start = HAL_GetTick();

	while (1) {

//		if ((HAL_GetTick() - start > 15000)) {
//			sprintf(strCache, "AT+CIPSTART=0,\"TCP\",\"%s\",80,1200\r\n", host);
//			if (esp_sendCommand(&ringBuffer, strCache, "OK\r\n", 5)) {
//				print("HOST:OK\r\n");
//			} else {
//				print("HOST:ERROR\r\n");
//			}
//			char dataStr[100];
//			sprintf(dataStr,
//					"GET /update?api_key=NDLZ3M336U8E039S&%s=%0.2f&%s=%d\r\n",
//					tempField, temp[i], heartRateField, hr[i]);
////			int dataLen = strlen(dataStr);
//			sprintf(strCache, "AT+CIPSEND=0,%d\r\n", strlen(dataStr));
//			Delay(10);
//			if (esp_sendCommand(&ringBuffer, strCache, "OK\r\n", 5)) {
//				print("HOST:OK\r\n");
//			} else {
//				print("HOST:ERROR\r\n");
//			}
//			Delay(10);
//			if (esp_sendCommand(&ringBuffer, dataStr, ",CLOSED\r\n", 5)) {
//				print("Data Sent:OK\r\n");
//			} else {
//				print("Data Sent:ERROR\r\n");
//			}
//			Delay(10);
//
//			i = (i + 1) % 5;
//			start = HAL_GetTick();
//		}

		thingspeak_send();

		if (HAL_GetTick() % 500 == 0) {
			gpio_toggle(PORTA, 5);
//			start = HAL_GetTick();
		}

	}
	return 0;
}

bool esp_init() {
	espInit = false;

	uint16_t numItem = 0;
	char strCache[100] = { 0 };
	bool checker = true;
//	if (esp_reset(&ringBuffer)) {
//		print("ESP: Resetted\r\n");
//	} else {
//		print("AT+RST:ERROR\r\n");
//		checker = false;
//	}

	if (esp_sendCommand(&ringBuffer, "ATE0\r\n", "OK\r\n", 5) == true) {
		print("ECHO:OFF\r\n");
	} else {
		print("ECHO:ERROR\r\n");
	}

	if (esp_sendAT(&ringBuffer, 5) == true) {
		print("AT:OK\r\n");
	} else {
		print("AT:ERROR\r\n");
		checker = false;
	}

	if (esp_setCWMODE(&ringBuffer, 1, 5)) {
		print("AT+CWMODE:OK\r\n");
	} else {
		print("AT+CWMODE:Error\r\n");
		checker = false;
	}

	if (esp_wifiDisconnect(&ringBuffer, 50)) {
		print("wifiDisconnect:OK\r\n");
	} else {
		print("wifiDisconnect:ERROR\r\n");
		checker = false;
	}
	Delay(500);
	print("Connecting to Wifi\r\n");
	if (esp_wifiConnect(&ringBuffer, ssid, password, 10000)) {
		print("AT+CWJAP:OK WIFI Sent\r\n");
	} else {
		print("AT+CWJAP:ERROR\r\n");
//		checker = false;
	}
//	for (int j = 0; j < 10; j++) {
//		Delay(1000);
//		while (ring_buffer_num_items(&ringBuffer)) {
//			numItem = ring_buffer_num_items(&ringBuffer);
//			if (numItem) {
//				ring_buffer_dequeue_arr(&ringBuffer, str, numItem);
//				str[numItem++] = '\0';
//				USART_WRITE(USART2, str, numItem, 10);
//			}
//		}
//	}
	Delay(1000);
	if (esp_getIP(&ringBuffer, strCache, 10)) {
		print("IP: %s\r\n", strCache);
	}
	Delay(50);
	if (esp_sendCIPMUX(&ringBuffer, 1, 5)) {
		print("CIPMUX=1:OK\r\n");
	} else {
		print("CIPMUX: ERROR\r\n");
		checker = false;
	}
//	if (esp_startServer(&ringBuffer, 1, 80, 5)) {
//		print("Server Started\r\n");
//	} else {
//		print("Error in Server\r\n");
//		checker = false;
//	}

	numItem = ring_buffer_num_items(&ringBuffer);
	if (numItem) {
		memset(strCache, "\0", 100);
		ring_buffer_dequeue_arr(&ringBuffer, strCache, numItem);
		print("Flushed String: %s\r\n", strCache);
	}


	if (checker == true) {
		espInit = true;
		return true;
	} else {
		return false;
	}
}

void thingspeak_send() {

	char response[100] = { 0 };

	if ((HAL_GetTick() - start > 15000)) {
		sprintf(strCache, "AT+CIPSTART=0,\"TCP\",\"%s\",80,1200\r\n", host);


//		if(esp_waitResponse(&ringBuffer, strCache, response, 15, 500)){
//			print("Response1: %s", response);
//		}
//		else{
//			print("AT+CIPSTART:ERROR\r\n");
//		}

//		if (esp_getResponse(&ringBuffer, strCache, response, 50)) {
//			print("Response1: %s", response);
//		} else {
//			print("AT+CIPSTART:ERROR\r\n");
//		}

		USART_WRITE(USART6, strCache, strlen(strCache), 50);

		char dataStr[100];
		sprintf(dataStr,
				"GET /update?api_key=NDLZ3M336U8E039S&%s=%0.2f&%s=%d\r\n",
				tempField, temp[i], heartRateField, hr[i]);
		//			int dataLen = strlen(dataStr);
		sprintf(strCache, "AT+CIPSEND=0,%d\r\n", strlen(dataStr));
		Delay(300);
//		if (esp_getResponse(&ringBuffer, strCache, response, 50)) {
//			print("Response2: %s", response);
//		} else {
//			print("HOST:ERROR\r\n");
//		}
		USART_WRITE(USART6, strCache, strlen(strCache), 50);
		Delay(300);
//		if (esp_getResponse(&ringBuffer, dataStr, response, 50)) {
//			print("Response3: %s", response);
//		} else {
//			print("Data Sent:ERROR\r\n");
//		}
		USART_WRITE(USART6, dataStr, strlen(dataStr), 50);
		Delay(300);
		numItem = ring_buffer_num_items(&ringBuffer);
		if (numItem) {
			memset(dataStr, "\0", 100);
			ring_buffer_dequeue_arr(&ringBuffer, dataStr, numItem);
			print("Flushed String: %s\r\n", dataStr);
		}

		i = (i + 1) % 5;
		start = HAL_GetTick();
	}
}

void USART2_IRQHandler(void) {
	if (USART2->SR & USART_SR_RXNE) {
		char chr = USART2->DR;
		if (chr == '\n') {
			command[idx++] = '\n';
			command[idx] = '\r';
			USART_WRITE(USART6, command, idx, 10);
			USART_WRITE(USART2, command, idx, 10);
			idx = 0;
		} else {
			command[idx++] = chr;
		}
	}
}

void USART6_IRQHandler(void) {
	if (USART6->SR & USART_SR_RXNE) {
		char chr = USART6->DR;
		ring_buffer_queue(&ringBuffer, chr);
		usartBusy = true;
		usartIdle = false;
	}
	if (USART6->SR & USART_SR_IDLE) {
		uint8_t read = USART6->SR;
		read = USART6->DR;
		usartBusy = false;
		usartIdle = true;
	}
	if ((clientRecFlag == false) && (espInit == true)) {
		if ((espClose == false) && (espClient == false)) {
			clientRecFlag = true;
			espClient = true;
		}
	}

}
