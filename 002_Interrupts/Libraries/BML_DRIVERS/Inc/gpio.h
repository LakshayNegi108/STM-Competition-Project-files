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

//========================== ALTERNATE PERIPHERAL NAMES ==========================//
//============= ALTERNATE FUNCTION 0 ==================//
#define EVENTOUT_PA1					ALT_FUN_0	//PA1
#define TIM15_CH1_PA2					ALT_FUN_0	//PA2
#define TIM15_CH2_PA3					ALT_FUN_0	//PA3
#define SPI1_NSS_PA4					ALT_FUN_0	//PA4
#define SPI1_SCK_PA5					ALT_FUN_0	//PA5
#define SPI1_MISO_PA6					ALT_FUN_0	//PA6
#define SPI1_MOSI_PA7					ALT_FUN_0	//PA7
#define MCO_PA8							ALT_FUN_0	//PA8
#define TIM15_BKIN_PA9					ALT_FUN_0	//PA9
#define TIM17_BKIN_PA10					ALT_FUN_0	//PA10
#define EVENTOUT_PA11					ALT_FUN_0	//PA11
#define EVENTOUT_PA12					ALT_FUN_0	//PA12
#define SWDIO_PA13						ALT_FUN_0	//PA13
#define SWCLK_PA14						ALT_FUN_0	//PA14
#define SPI1_NSS_PA15					ALT_FUN_0	//PA15

#define EVENTOUT_PB0					ALT_FUN_0	//PB0
#define TIM14_CH1_PB1					ALT_FUN_0	//PB1
#define SPI1_SCK_PB3					ALT_FUN_0	//PB3
#define SPI1_MISO_PB4					ALT_FUN_0	//PB4
#define SPI1_MOSI_PB5					ALT_FUN_0	//PB5
#define USART1_TX_PB6					ALT_FUN_0	//PB6
#define USART1_RX_PB7					ALT_FUN_0	//PB7
#define IR_OUT_PB9						ALT_FUN_0	//PB9
#define EVENTOUT_PB11					ALT_FUN_0	//PB11
#define SPI2_NSS_PB12					ALT_FUN_0	//PB12
#define SPI2_SCK_PB13					ALT_FUN_0	//PB13
#define SPI2_MISO_PB14					ALT_FUN_0	//PB14
#define SPI2_MOSI_PB15					ALT_FUN_0	//PB15

#define EVENTOUT_PC0					ALT_FUN_0	//PC0
#define EVENTOUT_PC1					ALT_FUN_0	//PC1
#define EVENTOUT_PC3					ALT_FUN_0	//PC3
#define EVENTOUT_PC4					ALT_FUN_0	//PC4
#define TIM3_CH1_PC6					ALT_FUN_0	//PC6
#define TIM3_CH2_PC7					ALT_FUN_0	//PC7
#define TIM3_CH3_PC8					ALT_FUN_0	//PC8
#define TIM3_CH4_PC9					ALT_FUN_0	//PC9
#define USART4_TX_PC10					ALT_FUN_0	//PC10
#define USART4_RX_PC11					ALT_FUN_0	//PC11
#define USART4_CK_PC12					ALT_FUN_0	//PC12

//============= ALTERNATE FUNCTION 1 ==================//
#define USART2_CTS_PA0					ALT_FUN_1	//PA0
#define USART2_RTS_PA1					ALT_FUN_1	//PA1
#define USART2_TX_PA2					ALT_FUN_1	//PA2
#define USART2_RX_PA3					ALT_FUN_1	//PA3
#define USART2_CK_PA4					ALT_FUN_1	//PA4
#define TIM3_CH1_PA6					ALT_FUN_1	//PA6
#define TIM3_CH2_PA7					ALT_FUN_1	//PA7
#define USART1_CK_PA8					ALT_FUN_1	//PA8
#define USART1_TX_PA9					ALT_FUN_1	//PA9
#define USART1_RX_PA10					ALT_FUN_1	//PA10
#define USART1_CTS_PA11					ALT_FUN_1	//PA11
#define USART1_RTS_PA12					ALT_FUN_1	//PA12
#define IR_OUT_PA13						ALT_FUN_1	//PA13
#define USART2_TX_PA14					ALT_FUN_1	//PA14
#define USART2_RX_PA15					ALT_FUN_1	//PA15

#define TIM3_CH3_PB0					ALT_FUN_1	//PB0
#define TIM3_CH4_PB1					ALT_FUN_1	//PB1
#define EVENTOUT_PB3					ALT_FUN_1	//PB3
#define TIM3_CH1_PB4					ALT_FUN_1	//PB4
#define TIM3_CH2_PB5					ALT_FUN_1	//PB5
#define I2C1_SCL_PB6					ALT_FUN_1	//PB6
#define I2C1_SDA_PB7					ALT_FUN_1	//PB7
#define I2C1_SCL_PB8					ALT_FUN_1	//PB8
#define I2C1_SDA_PB9					ALT_FUN_1	//PB9
#define I2C2_SCL_PB10					ALT_FUN_1	//PB10
#define I2C2_SDA_PB11					ALT_FUN_1	//PB11
#define EVENTOUT_PB12					ALT_FUN_1	//PB12
#define TIM15_CH1_PB14					ALT_FUN_1	//PB14
#define TIM15_CH2_PB15					ALT_FUN_1	//PB15

#define SPI2_MISO_PC2					ALT_FUN_1	//PC2
#define SPI2_MOSI_PC3					ALT_FUN_1	//PC3
#define USART3_TX_PC4					ALT_FUN_1	//PC4
#define USART3_RX_PC5					ALT_FUN_1	//PC5
#define USART3_TX_PC10					ALT_FUN_1	//PC10
#define USART3_RX_PC11					ALT_FUN_1	//PC11
#define USART3_CK_PC12					ALT_FUN_1	//PC12

//============= ALTERNATE FUNCTION 2 ==================//
#define USB_NOE_PA4						ALT_FUN_2	//PA4
#define TIM1_BKIN_PA6					ALT_FUN_2	//PA6
#define TIM1_CH1N_PA7					ALT_FUN_2	//PA7
#define TIM1_CH1_PA8					ALT_FUN_2	//PA8
#define TIM1_CH2_PA9					ALT_FUN_2	//PA9
#define TIM1_CH3_PA10					ALT_FUN_2	//PA10
#define TIM1_CH4_PA11					ALT_FUN_2	//PA11
#define TIM1_ETR_PA12					ALT_FUN_2	//PA12
#define USB_NOE_PA13					ALT_FUN_2	//PA13

#define TIM1_CH2N_PB0					ALT_FUN_2	//PB0
#define TIM1_CH3N_PB1					ALT_FUN_2	//PB1
#define EVENTOUT_PB4					ALT_FUN_2	//PB4
#define TIM16_BKIN_PB5					ALT_FUN_2	//PB5
#define TIM16_CH1N_PB6					ALT_FUN_2	//PB6
#define TIM17_CH1N_PB7					ALT_FUN_2	//PB7
#define TIM16_CH1_PB8					ALT_FUN_2	//PB8
#define TIM17_CH1_PB9					ALT_FUN_2	//PB9
#define TIM1_BKIN_PB12					ALT_FUN_2	//PB12
#define TIM1_CH1N_PB13					ALT_FUN_2	//PB13
#define TIM1_CH2N_PB14					ALT_FUN_2	//PB14
#define TIM1_CH3N_PB15					ALT_FUN_2	//PB15

//============= ALTERNATE FUNCTION 3 =================//
#define EVENTOUT_PA8					ALT_FUN_3	//PA8
#define EVENTOUT_PA15					ALT_FUN_3	//PA15

#define I2C1_SMBA_PB5					ALT_FUN_3	//PB5
#define EVENTOUT_PB9					ALT_FUN_3	//PB9
#define TIM15_CH1N_PB15					ALT_FUN_3	//PB15

//============= ALTERNATE FUNCTION 4 ==================//
#define USART4_TX_PA0					ALT_FUN_4	//PA0
#define USART4_RX_PA1					ALT_FUN_4	//PA1
#define TIM14_CH1_PA4					ALT_FUN_4	//PA4
#define USART3_CTS_PA6					ALT_FUN_4	//PA6
#define TIM14_CH1_PA7					ALT_FUN_4	//PA7
#define I2C1_SCL_PA9					ALT_FUN_4	//PA9
#define I2C1_SDA_PA10					ALT_FUN_4	//PA10
#define USART4_RTS_PA15					ALT_FUN_4	//PA15

#define USART3_CK_PB0					ALT_FUN_4	//PB0
#define USART3_RTS_PB1					ALT_FUN_4	//PB1
#define USART4_CTS_PB7					ALT_FUN_4	//PB7
#define USART3_TX_PB10					ALT_FUN_4	//PB10
#define USART3_RX_PB11					ALT_FUN_4	//PB11
#define USART3_CK_PB12					ALT_FUN_4	//PB12
#define USART3_CTS_PB13					ALT_FUN_4	//PB13
#define USART3_RTS_PB14					ALT_FUN_4	//PB14

//============= ALTERNATE FUNCTION 5 ==================//
#define TIM15_CH1N_PA1					ALT_FUN_5	//PA1
#define TIM16_CH1_PA6					ALT_FUN_5	//PA6
#define TIM17_CH1_PA7					ALT_FUN_5	//PA7

#define TIM17_BKIN_PB4					ALT_FUN_5	//PB4
#define SPI2_NSS_PB9					ALT_FUN_5	//PB9
#define SPI2_SCK_PB10					ALT_FUN_5	//PB10
#define TIM15_BKIN_PB12					ALT_FUN_5	//PB12
#define I2C2_SCL_PB13					ALT_FUN_5	//PB13
#define I2C2_SDA_PB14					ALT_FUN_5	//PB14

//============= ALTERNATE FUNCTION 6 ==================//
#define EVENTOUT_PA6					ALT_FUN_6	//PA6
#define EVENTOUT_PA7					ALT_FUN_6	//PA7
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
