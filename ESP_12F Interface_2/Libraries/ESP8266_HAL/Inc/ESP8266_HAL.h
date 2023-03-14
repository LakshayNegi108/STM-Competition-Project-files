/*
 * ESP8266_HAL.h
 *
 *  Created on: Apr 14, 2020
 *      Author: Controllerstech
 */

#ifndef INC_ESP8266_HAL_H_
#define INC_ESP8266_HAL_H_


void ESP_Init (char *SSID, char *PASSWD);

void Server_Handle(char *str, int Link_ID, float temp);
uint8_t Server_Start (float temp);


#endif /* INC_ESP8266_HAL_H_ */
