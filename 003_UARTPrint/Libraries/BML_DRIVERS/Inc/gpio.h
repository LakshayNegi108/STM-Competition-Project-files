/*
 * BML_GPIO.h
 *
 *  Created on: Feb 18, 2022
 *      Author: hp
 */

#ifndef INC_BML_DRIVER_H_
#define INC_BML_DRIVER_H_

#include "BML_DEF.h"

#define PORTA		GPIOA
#define PORTB		GPIOB
#define PORTC		GPIOC
#define PORTD		GPIOD
#define PORTE		GPIOE
#define PORTF		GPIOF

// PIN FUNCTION MODES
#define INPUT_MODE					((uint32_t)0x00U)
#define OUTPUT_MODE					((uint32_t)0x01U)
#define ALT_MODE					((uint32_t)0x02U)
#define ANALOG_MODE					((uint32_t)0x03U)

// PIN SPEED MODES
#define LOW_SPEED					((uint32_t)0x00U)
#define MEDIUM_SPEED				((uint32_t)0x01U)
#define HIGH_SPEED					((uint32_t)0x02U)
#define VERY_HIGH_SPEED				((uint32_t)0x03U)

// ENABLE-DISABLE PULL-UP/PULL-DOWN
#define DI_PUPD						((uint32_t)0x00U)
#define EN_PU						((uint32_t)0x01U)
#define EN_PD						((uint32_t)0x02U)

// ENABLE-DISABLE PUSH-PULL or OPEN-DRAIN
#define PHPL						((uint32_t)0x00U)
#define OD							((uint32_t)0x01U)

// RESET CLOCK FOR DIFFERENT PORTS
#define GPIOA_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOARST)
#define GPIOB_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOBRST)
#define GPIOC_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOCRST)
#define GPIOD_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIODRST)
#define GPIOE_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOERST)
#define GPIOH_CLK_RST				(RCC->AHB1RSTR |= RCC_AHB1RSTR_GPIOHRST)

// ENABLE CLOCK FOR DIFFERENT PORTS
#define GPIOA_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN)
#define GPIOB_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN)
#define GPIOC_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)
#define GPIOD_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN)
#define GPIOE_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN)
#define GPIOH_CLK_EN				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN)

#define SYSCFG_CLK_RST				(RCC->APB2ENR |= RCC_APB2RSTR_SYSCFGRST)
#define SYSCFG_CLK_EN				(RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN)

// NAMING FOR ALTERNATE FUNCTION ARRAY
#define ALT_FUN_0					0x00
#define ALT_FUN_1					0x01
#define ALT_FUN_2					0x02
#define ALT_FUN_3					0x03
#define ALT_FUN_4					0x04
#define ALT_FUN_5					0x05
#define ALT_FUN_6					0x06
#define ALT_FUN_7					0x07
#define ALT_FUN_8					0x08
#define ALT_FUN_9					0x09
#define ALT_FUN_10					0x0A
#define ALT_FUN_11					0x0B
#define ALT_FUN_12					0x0C
#define ALT_FUN_13					0x0D
#define ALT_FUN_14					0x0E
#define ALT_FUN_15					0x0F

//========================== ALTERNATE PERIPHERAL NAMES ==========================//
//============= ALTERNATE FUNCTION 0 ==================//
#define PA8_MCO_1						ALT_FUN_0	//PA8
#define PA13_JTMS_SWDIO					ALT_FUN_0	//PA13
#define PA14_JTCK_SWCLK					ALT_FUN_0	//PA14
#define PA15_JTDI						ALT_FUN_0	//PA15

#define PB3_JTDO_SWO					ALT_FUN_0	//PB3
#define PB4_JTRST						ALT_FUN_0	//PB4
#define PB3_RTC_REFN					ALT_FUN_0	//PB15

#define PC9_MCO_2						ALT_FUN_0	//PC9


//============= ALTERNATE FUNCTION 1 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2


//============= ALTERNATE FUNCTION 2 ==================//
#define USB_NOE_PA4						ALT_FUN_2	//PA4

#define TIM1_CH2N_PB0					ALT_FUN_2	//PB0

//============= ALTERNATE FUNCTION 3 =================//
#define EVENTOUT_PA8					ALT_FUN_3	//PA8

#define I2C1_SMBA_PB5					ALT_FUN_3	//PB5


//============= ALTERNATE FUNCTION 4 ==================//
#define USART4_TX_PA0					ALT_FUN_4	//PA0

#define USART3_CK_PB0					ALT_FUN_4	//PB0

//============= ALTERNATE FUNCTION 5 ==================//
#define TIM15_CH1N_PA1					ALT_FUN_5	//PA1


#define TIM17_BKIN_PB4					ALT_FUN_5	//PB4

//============= ALTERNATE FUNCTION 6 ==================//
#define EVENTOUT_PA6					ALT_FUN_6	//PA6

//============= ALTERNATE FUNCTION 7 ==================//
#define PA0_USART2_CTS					ALT_FUN_7	//PA0
#define PA1_USART2_RTS					ALT_FUN_7	//PA1
#define PA2_USART2_TX					ALT_FUN_7	//PA2
#define PA3_USART2_RX					ALT_FUN_7	//PA3
#define PA4_USART2_CK					ALT_FUN_7	//PA4
#define PA8_USART1_CK					ALT_FUN_7	//PA8
#define PA9_USART1_TX					ALT_FUN_7	//PA9
#define PA10_USART2_RX					ALT_FUN_7	//PA10
#define PA11_USART2_CTS					ALT_FUN_7	//PA11
#define PA12_USART2_RTS					ALT_FUN_7	//PA12

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 8 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 9 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 10 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 11 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 12 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 13 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 14 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//============= ALTERNATE FUNCTION 15 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2

//======================================================//

#define ADC0_PORT	GPIOA
#define ADC0_PIN	0
#define ADC1_PORT	GPIOA
#define ADC1_PIN	1
#define ADC2_PORT	GPIOA
#define ADC2_PIN	2
#define ADC3_PORT	GPIOA
#define ADC3_PIN	3
#define ADC4_PORT	GPIOA
#define ADC4_PIN	4
#define ADC5_PORT	GPIOA
#define ADC5_PIN	5
#define ADC6_PORT	GPIOA
#define ADC6_PIN	6
#define ADC7_PORT	GPIOA
#define ADC7_PIN	7
#define ADC8_PORT	GPIOB
#define ADC8_PIN	0
#define ADC9_PORT	GPIOB
#define ADC9_PIN	1
#define ADC10_PORT	GPIOC
#define ADC10_PIN	0
#define ADC11_PORT	GPIOC
#define ADC11_PIN	1
#define ADC12_PORT	GPIOC
#define ADC12_PIN	2
#define ADC13_PORT	GPIOC
#define ADC13_PIN	3
#define ADC14_PORT	GPIOC
#define ADC14_PIN	4
#define ADC15_PORT	GPIOC
#define ADC15_PIN	5

typedef struct {
	GPIO_TypeDef *port;
	uint32_t pin;
	uint32_t mode;
	uint32_t pull;
	uint32_t speed;
	uint32_t alt_func;
} GPIO_TYPE;

typedef enum {
	RISING_EDGE, FALLING_EDGE, RISING_FALLING_EDGE
} edge_select;



uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

//===============================CLOCK FUNCTION=======================================//
void gpio_clk_en(GPIO_TypeDef *port);
//====================================================================================//

//===============================GPIO CONFIGURATION FUNCTION==========================//
void gpio_config(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode,
		uint32_t pinSpeed, uint32_t pinPUPD, uint32_t PHPL_OD);
//void gpio_input_config(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode, uint32_t pinPUPD);
void gpio_moder(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode);
void gpio_speed(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinMode);
void gpio_PUPD(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinPUPD);
void gpio_PHPL_OD(GPIO_TypeDef *port, uint32_t pinNumber, uint32_t PHPL_OD);
void gpio_altfn(GPIO_TypeDef *port, uint32_t pinNumber,
		uint32_t alternate_function);
//====================================================================================//

//==============================GPIO OPERATION FUNCTION===============================//
void gpio_write(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t state);
void gpio_toggle(GPIO_TypeDef *port, uint32_t pinNumber);
uint8_t gpio_read(GPIO_TypeDef *port, uint32_t pinNumber);
//====================================================================================//

//=============================GPIO INTERRUPT FUNCTION================================//
void gpio_IT_config(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t edge_sel);
void gpio_IT_EN(uint8_t pinNumber, IRQn_Type irqNumber);
void gpio_IT_DI(uint8_t pinNumber, IRQn_Type irqNumber);
void gpio_IT_SW(uint8_t pinNumber);
uint8_t gpio_IT_CHK(uint8_t pinNumber);
void gpio_IT_CLR(uint8_t pinNumber);
//====================================================================================//




#endif /* INC_BML_DRIVER_H_ */
