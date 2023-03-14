/*
 * ESP8266_HAL.c
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */

#include "UartRingbuffer_multi.h"
#include "ESP8266_HAL.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;

#define wifi_uart &huart6
#define pc_uart &huart2

char buffer[20];

char *Basic_inclusion =
		"<!DOCTYPE html> <html>\n<head><meta name=\"viewport\"\
		content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n\
		<title>Health Monitoring Web App</title>\n<style>html { font-family: Helvetica; \
		display: inline-block; margin: 0px auto; text-align: center;}\n\
		body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\
		h3 {color: #444444;margin-bottom: 50px;}\n.button {display: block;\
		width: 80px;background-color: #1abc9c;border: none;color: white;\
		padding: 13px 30px;text-decoration: none;font-size: 25px;\
		margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n\
		.button-on {background-color: #1abc9c;}\n.button-on:active \
		{background-color: #16a085;}\n.button-off {background-color: #34495e;}\n\
		.button-off:active {background-color: #2c3e50;}\np {font-size: 14px;color: #888;margin-bottom: 10px;}\n\
		</style>\n</head>\n<body>\n<h1>Health Monitoring System</h1>\n";

char *LED_ON =
		"<p>LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>";
char *LED_OFF =
		"<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>";
char *REFRESH = "<a class=\"button button-on\" href=\"/hms\">Refresh</a>";
char *Terminate = "</body></html>";

/*****************************************************************************************************************************************/

void ESP_Init(char *SSID, char *PASSWD) {
	char data[80];

	Ringbuf_init();

	Uart_sendstring("AT+RST\r\n", wifi_uart);
	Uart_sendstring("RESETTING.", pc_uart);
	for (int i = 0; i < 5; i++) {
		Uart_sendstring(".", pc_uart);
		HAL_Delay(1000);
	}

	/********* AT **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("AT\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	Uart_sendstring("AT---->OK\n\n", pc_uart);

	/********* AT+CWMODE=1 **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("AT+CWMODE=1\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	Uart_sendstring("CW MODE---->1\n\n", pc_uart);

	/********* AT+CWJAP="SSID","PASSWD" **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("connecting... to the provided AP\n", pc_uart);
	sprintf(data, "AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PASSWD);
	Uart_sendstring(data, wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	sprintf(data, "Connected to,\"%s\"\n\n", SSID);
	Uart_sendstring(data, pc_uart);

	/********* AT+CIFSR **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("AT+CIFSR\r\n", wifi_uart);
	while (!(Wait_for("CIFSR:STAIP,\"", wifi_uart)))
		;
	while (!(Copy_upto("\"", buffer, wifi_uart)))
		;
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	int len = strlen(buffer);
	buffer[len - 1] = '\0';
	sprintf(data, "IP ADDR: %s\n\n", buffer);
	Uart_sendstring(data, pc_uart);

	/********* AT+CIPMUX **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("AT+CIPMUX=1\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	Uart_sendstring("CIPMUX---->OK\n\n", pc_uart);

	/********* AT+CIPSERVER **********/
	Uart_flush(wifi_uart);
	Uart_sendstring("AT+CIPSERVER=1,80\r\n", wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	Uart_sendstring("CIPSERVER---->OK\n\n", pc_uart);

	Uart_sendstring("Now Connect to the IP ADRESS\n\n", pc_uart);

}

int Server_Send(char *str, int Link_ID) {
	int len = strlen(str);
	char data[80];
	sprintf(data, "AT+CIPSEND=%d,%d\r\n", Link_ID, len);
	Uart_sendstring(data, wifi_uart);
	while (!(Wait_for(">", wifi_uart)))
		;
	Uart_sendstring(str, wifi_uart);
	while (!(Wait_for("SEND OK", wifi_uart)))
		;
	sprintf(data, "AT+CIPCLOSE=5\r\n");
	Uart_sendstring(data, wifi_uart);
	while (!(Wait_for("OK\r\n", wifi_uart)))
		;
	return 1;
}

void Server_Handle(char *str, int Link_ID, float temp, int HR, int SpO2,
		int ECG) {
	char datatosend[2048] = { 0 };
	char tempData[400];
	sprintf(tempData,
			"<p><h2>Temp: %0.2f</h2><h2>Heart Rate: %dBPM</h2><h2>SpO2: %d%</h2><h2>ECG: %dBPM</h2></p>",
			temp, HR, SpO2, ECG);
//	if (!(strcmp(str, "/ledon"))) {
//		sprintf(datatosend, Basic_inclusion);
//		strcat(datatosend, LED_ON);
//		strcat(datatosend, tempData);
//		strcat(datatosend, Terminate);
//		Server_Send(datatosend, Link_ID);
//	}
//
//	else if (!(strcmp(str, "/ledoff"))) {
//		sprintf(datatosend, Basic_inclusion);
//		strcat(datatosend, LED_OFF);
//		strcat(datatosend, tempData);
//		strcat(datatosend, Terminate);
//		Server_Send(datatosend, Link_ID);
//	}
//
//	else {
//		sprintf(datatosend, Basic_inclusion);
//		strcat(datatosend, LED_OFF);
//		strcat(datatosend, tempData);
//		strcat(datatosend, Terminate);
//		Server_Send(datatosend, Link_ID);
//	}

	sprintf(datatosend, Basic_inclusion);
	strcat(datatosend, tempData);
	strcat(datatosend, REFRESH);
	strcat(datatosend, Terminate);
	Server_Send(datatosend, Link_ID);

}

uint8_t Server_Start(float temp, int HR, int SpO2, int ECG) {
	char buftocopyinto[64] = { 0 };
	char Link_ID;
	while (!(Get_after("+IPD,", 1, &Link_ID, wifi_uart)))
		;
	Link_ID -= 48;
	while (!(Copy_upto(" HTTP/1.1", buftocopyinto, wifi_uart)))
		;
	if (Look_for("/ledon", buftocopyinto) == 1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
		Server_Handle("/ledon", Link_ID, temp, HR, SpO2, ECG);
	}

	else if (Look_for("/ledoff", buftocopyinto) == 1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		Server_Handle("/ledoff", Link_ID, temp, HR, SpO2, ECG);
	}

	else if (Look_for("/favicon.ico", buftocopyinto) == 1)
		;

	else {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
		Server_Handle("/ ", Link_ID, temp, HR, SpO2, ECG);
	}
	return 1;
}