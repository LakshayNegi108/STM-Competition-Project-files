#include "main.h"
#include "BML_DEF.h"
#include "rcc.h"
#include "i2c.h"

#define SLAVE_ADDR 0x5A    /* 1101 000.    DS1337 */

void I2C1_init(void);
int I2C1_byteRead(char saddr, char maddr, char *data);
int I2C1_DevReady(char saddr, uint8_t tries);
void delayMs(int n);

int main() {

	RCC_CONFIG_84MHZ();

	uart_print_config(9600);
	print("Working\n");

	gpio_config(PORTB, 8, ALT_MODE, LOW_SPEED, EN_PU, OD);
	gpio_config(PORTB, 9, ALT_MODE, LOW_SPEED, EN_PU, OD);
	gpio_altfn(PORTB, 8, PB8_I2C1_SCL);
	gpio_altfn(PORTB, 9, PB9_I2C1_SDA);

	if (I2C_INIT(I2C1, STANDARD_MODE, 42, 0x32) != BML_OK) {
		print("Error in Init\n");
		while (1)
			;
	}

//	I2C1_byteWrite(0x5A, 0x03, 1);
//	for (int i = 1; i < 255; i++) {
//		if (I2C_DeviceReady(I2C1, i, 5000) != BML_OK){
//			print("X ");
//		} else {
//			print("\nDevice Found at %d\n", i);
//		}
//	}

	uint8_t data;
	while (1) {
//		print("Running\n");
		I2C1_byteRead(SLAVE_ADDR, 0x07, &data);
		print("Temp: %d\n", data);
//		Delay(500);
	}
	return 0;
}

/* this funtion reads a byte of data from the memory location
 * maddr of a device with I2C slave device address saddr.
 * For simplicity, no error checking or error report is done.
 */
int I2C1_byteRead(char saddr, char maddr, char *data) {
	volatile int tmp;

	while (I2C1->SR2 & 2)
		; /* wait until bus not busy */

	I2C1->CR1 |= 0x100; /* generate start */
	while (!(I2C1->SR1 & 1))
		; /* wait until start flag is set */

	I2C1->DR = saddr << 1; /* transmit slave address + Write */
	while (!(I2C1->SR1 & 2))
		; /* wait until addr flag is set */
	tmp = I2C1->SR2; /* clear addr flag */

	while (!(I2C1->SR1 & 0x80))
		; /* wait until data register empty */
	I2C1->DR = maddr; /* send memory address */

	while (!(I2C1->SR1 & 0x80))
		; /* wait until data register empty */

	I2C1->CR1 |= 0x100; /* generate restart */
	while (!(I2C1->SR1 & 1))
		; /* wait until start flag is set */
	I2C1->DR = saddr << 1 | 1; /* transmit slave address + Read */

	while (!(I2C1->SR1 & 2))
		; /* wait until addr flag is set */
	I2C1->CR1 &= ~0x400; /* Disable Acknowledge */
	tmp = I2C1->SR2; /* clear addr flag */

	I2C1->CR1 |= 0x200; /* generate stop after data received */

	while (!(I2C1->SR1 & 0x40))
		; /* Wait until RXNE flag is set */
	*data++ = I2C1->DR; /* Read data from DR */

	return 0;
}
