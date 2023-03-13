/*
 * ESP8266.h
 *
 *  Created on: 02-Mar-2023
 *      Author: hp
 */

#ifndef ESP8266_INC_ESP8266_H_
#define ESP8266_INC_ESP8266_H_

#include "BML_DEF.h"
#include "uart.h"
#include "ringbuffer.h"

extern bool usartBusy;
extern bool usartIdle;

int isNumber(char s);

bool esp_sendCommand(ring_buffer_t *ringBuffer, char *cmd, char *response,
		uint16_t timeout);
bool esp_getResponse(ring_buffer_t *ringBuffer, char *cmd, char *response,
		uint16_t timeout);
bool esp_waitResponse(ring_buffer_t *ringBuffer, char *cmd, char *response,int items,
		uint16_t timeout);
bool esp_multiResponse(ring_buffer_t *ringBuffer, char *cmd, char *response,
		int numMsg, uint16_t timeout) ;
int16_t searchSubstring(char *str, char *subStr);

bool esp_sendAT(ring_buffer_t *ringBuffer, uint16_t timeout);
bool esp_reset(ring_buffer_t *ringBuffer, uint16_t timeout);
bool esp_setCWMODE(ring_buffer_t *ringBuffer, uint8_t n, uint16_t timeout);
bool esp_wifiDisconnect(ring_buffer_t *ringBuffer, uint16_t timeout);
bool esp_wifiConnect(ring_buffer_t *ringBuffer, char *ssid, char *password,
		uint16_t timeout);
bool esp_getIP(ring_buffer_t *ringBuffer, char *ip, uint16_t timeout);
bool esp_sendCIPMUX(ring_buffer_t *ringBuffer, uint8_t n, uint16_t timeout);
bool esp_startServer(ring_buffer_t *ringBuffer, uint8_t startServer,
		uint8_t port, uint16_t timeout);
bool esp_closeServer(ring_buffer_t *ringBuffer, uint8_t linkID,
		uint16_t timeout);

int Uart_peek(ring_buffer_t *ringBuffer);
int Wait_for (ring_buffer_t *ringBuffer, char *string);

#endif /* ESP8266_INC_ESP8266_H_ */