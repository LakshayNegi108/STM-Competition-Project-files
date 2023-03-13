/*
 * i2c.c
 *
 *  Created on: Oct 23, 2022
 *      Author: hp
 */
#include "i2c.h"

void I2C_EN(I2C_TypeDef *i2c) {
	i2c->CR1 |= I2C_CR1_PE;
}

void I2C_DI(I2C_TypeDef *i2c) {
	i2c->CR1 &= ~I2C_CR1_PE;
}

void I2C_Reset(I2C_TypeDef *i2c) {
	i2c->CR1 |= 0x8000;
	i2c->CR1 &= ~0x8000;
}

BML_StatusTypeDef I2C_INIT(I2C_TypeDef *i2c, uint8_t timingMode,
		uint8_t pClkVal, uint8_t ownAddr) {
	if (i2c == I2C1) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	} else if (i2c == I2C2) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	} else if (i2c == I2C3) {
		RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
	}
	I2C_Reset(i2c);
	if ((pClkVal < 50) | (pClkVal > 2)) {
		i2c->CR2 |= pClkVal;
	}
//	i2c->OAR1 |= ownAddr << 1;

	switch (timingMode) {
	case STANDARD_MODE:
		i2c->CCR &= ~(1 << 15);
		i2c->CCR |= pClkVal * 5;
		break;
	case FAST_MODE:
		i2c->CCR |= (1 << 15);
		break;
	default:
		return BML_ERROR;
	}
	i2c->TRISE |= pClkVal + 1;
	I2C_EN(i2c);

	return BML_OK;
}

void I2C_DEINIT(I2C_TypeDef *i2c) {

	i2c->CR1 = 0; /* software reset I2C1 */
	i2c->CCR = 0; /* 100 KHz, peripheral clock is 8/48 MHz */

	if (i2c == I2C1) {
		RCC->APB1ENR &= ~RCC_APB1ENR_I2C1EN;
	} else if (i2c == I2C2) {
		RCC->APB1ENR &= ~RCC_APB1ENR_I2C2EN;
	} else if (i2c == I2C2) {
		RCC->APB1ENR &= ~RCC_APB1ENR_I2C3EN;
	}
}

void I2C_CR2(I2C_TypeDef *i2c, uint32_t val) {
	i2c->CR2 = val;
}

BML_StatusTypeDef I2C_DeviceReady(I2C_TypeDef *i2c, uint8_t saddr,
		uint16_t timeout) {
	uint16_t i = timeout;

	while (i2c->SR2 & I2C_SR2_BUSY) {/* wait until bus not busy */
//		I2C_Delay(1);
		i--;
		if (i <= 0) {
			return BML_BUSY;
		}
	}

	i2c->CR1 |= I2C_CR1_START;
	i = timeout;
	while (!(i2c->SR1 & I2C_SR1_SB)) {
		i--;
		if (i <= 0) {
			return BML_BUSY;
		}
	}
	i = timeout;
	i2c->DR = saddr << 1;
	while (!(I2C1->SR1 & 2))
			; /* wait until addr flag is set */
//	while (!(i2c->SR1 & I2C_SR1_ADDR)) {
//		i--;
//		if (i <= 0) {
//			return BML_BUSY;
//		}
//	}
	{
		uint16_t temp = i2c->SR2;
	}

//	if((i2c->SR1 & I2C_SR1_AF) == ~(I2C_SR1_AF)){
//		return BML_ERROR;
//	}else{
//
//	}
	return BML_OK;
}

//BML_StatusTypeDef I2C_MasterByteWrite(I2C_TypeDef *i2c, uint8_t saddr,
//		uint8_t maddr, uint8_t data, uint16_t timeout) {
//	uint16_t i = timeout;
//	while (i2c->ISR & I2C_ISR_BUSY) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_BUSY;
//		}
//	} /* wait until bus not busy */
//
//	i = timeout;
//
//	I2C_CR2(i2c, I2C_CR2_START | I2C_CR2_AUTOEND | (2 << 16) | (saddr << 1));
//	/* generate start, autoend, byte count 2 and send slave address */
//
//	while (!(i2c->ISR & I2C_ISR_TXIS)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until TXIS is set */
//	i2c->TXDR = maddr; /* send register address */
//
//	i = timeout;
//
//	while (!(i2c->ISR & I2C_ISR_TXIS)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until TXIS is set */
//	i2c->TXDR = data; /* send data */
//
//	i = timeout;
//
//	while (!(i2c->ISR & I2C_ISR_STOPF)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	}/* wait until stop flag is set */
//
//	i2c->ICR = I2C_ICR_STOPCF; /* clear stop flag */
//	I2C_CR2(i2c, I2C_CR2_AUTOEND);
//
//	return BML_OK;
//}
//
//BML_StatusTypeDef I2C_MasterByteRead(I2C_TypeDef *i2c, uint8_t saddr,
//		uint8_t maddr, uint8_t *data, uint16_t timeout) {
//	uint16_t i = timeout;
//	while (i2c->ISR & I2C_ISR_BUSY) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_BUSY;
//		}
//	} /* wait until bus not busy */
//
//	i = timeout;
//
//	I2C_CR2(i2c, I2C_CR2_START | (1 << 16) | (saddr << 1));
//	/* generate start and send slave address */
//	while (!(i2c->ISR & I2C_ISR_TXIS)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until start flag is set */
//	i = timeout;
//
//	i2c->TXDR = maddr; /* send memory address */
//
//	while (!(i2c->ISR & I2C_ISR_TC)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until transfer complete */
//
//	i = timeout;
//
//	/* generate restart, send slave address, read 1 byte, set for auto end */
//	I2C_CR2(i2c,
//			I2C_CR2_START | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | (1 << 16)
//					| (saddr << 1));
//
//	while (!(i2c->ISR & I2C_ISR_RXNE)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* Wait until RXNE flag is set */
//	*data = i2c->RXDR; /* Read data from DR */
//
//	i = timeout;
//
//	while (!(i2c->ISR & I2C_ISR_STOPF)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until stop flag is set */
//
//	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
//	I2C_CR2(i2c, I2C_CR2_AUTOEND);
//
//	return BML_OK;
//}
//
//BML_StatusTypeDef I2C_MasterMultiWrite(I2C_TypeDef *i2c, uint8_t saddr,
//		uint8_t maddr, uint32_t n, uint8_t *data, uint16_t timeout) {
//	int x;
//	uint16_t i = timeout;
//
//	while (i2c->ISR & I2C_ISR_BUSY) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_BUSY;
//		}
//	} /* wait until bus not busy */
//	I2C_CR2(i2c,
//			I2C_CR2_START | I2C_CR2_AUTOEND | ((n + 1) << 16) | (saddr << 1));
//	/* set byte count and send slave address */
//	/* generate start, set auto end, */
//	i = timeout;
//	while (!(i2c->ISR & I2C_ISR_TXIS)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until TXIS is set */
//	i2c->TXDR = maddr; /* send memory address */
//	i = timeout;
//	for (x = 0; x < n; x++) /* send n bytes of data */
//	{
//		while (!(i2c->ISR & I2C_ISR_TXIS)) {
//			I2C_Delay(1);
//			i--;
//			if (i <= 0) {
//				return BML_TIMEOUT;
//			}
//		} /* wait until TXIS is set */
//		i2c->TXDR = *data++; /* send a byte of data */
//	}
//	i = timeout;
//	while (!(i2c->ISR & I2C_ISR_STOPF)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until stop flag is set */
//
//	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
//	I2C_CR2(i2c, I2C_CR2_AUTOEND);
//
//	return BML_OK;
//}
//
//BML_StatusTypeDef I2C_MasterMultiRead(I2C_TypeDef *i2c, uint8_t saddr,
//		uint8_t maddr, uint32_t n, uint8_t *data, uint16_t timeout) {
//	uint16_t i = timeout;
//	while (i2c->ISR & I2C_ISR_BUSY) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_BUSY;
//		}
//	} /* wait until bus not busy */
//	i = timeout;
//
//	I2C_CR2(i2c, I2C_CR2_START | (1 << 16) | (saddr << 1));/* generate start and send slave address */
//
//	while (!(i2c->ISR & I2C_ISR_TXIS)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	}/* wait until transmitter is ready */
//
//	i = timeout;
//
//	i2c->TXDR = maddr; /* send memory address */
//	while (!(i2c->ISR & I2C_ISR_TC)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until transfer complete */
//
//	/* generate restart, send slave address, repeat read n bytes, set for auto end */
//
//	I2C_CR2(i2c,
//			I2C_CR2_START | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND
//					| ((n & 0xFF) << 16) | (saddr << 1));
//
//	for (; n > 0; n--) {
//		while (!(i2c->ISR & I2C_ISR_RXNE)) {
//			I2C_Delay(1);
//			i--;
//			if (i <= 0) {
//				return BML_TIMEOUT;
//			}
//		} /* Wait until RXNE flag is set */
//		i = timeout;
//		*data++ = i2c->RXDR; /* Read data from DR */
//	}
//
//	while (!(i2c->ISR & I2C_ISR_STOPF)) {
//		I2C_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_TIMEOUT;
//		}
//	} /* wait until stop flag is set */
//	i = timeout;
//	i2c->ICR = I2C_ISR_STOPF; /* clear stop flag */
//	I2C_CR2(i2c, I2C_CR2_AUTOEND);
//
//	return BML_OK;
//}

int I2C1_byteWrite(char saddr, char maddr, char data) {
    volatile int tmp;

    while (I2C1->SR2 & 2);                  /* wait until bus not busy */

    I2C1->CR1 |=  0x100;                    /* generate start */
    while (!(I2C1->SR1 & 1));               /* wait until start flag is set */

    I2C1->DR = saddr;                  /* transmit slave address */
    while (!(I2C1->SR1 & 2));               /* wait until addr flag is set */
    tmp = I2C1->SR2;                        /* clear addr flag */

    while (!(I2C1->SR1 & 0x80));            /* wait until data register empty */
    I2C1->DR = maddr;                       /* send memory address */

    while (!(I2C1->SR1 & 0x80));            /* wait until data register empty */
    I2C1->DR = data;                        /* transmit data */

    while (!(I2C1->SR1 & 4));               /* wait until transfer finished */
    I2C1->CR1 |= 0x200;                     /* generate stop */

    return 0;
}
