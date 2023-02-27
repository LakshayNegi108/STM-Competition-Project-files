/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BML_DEF.h"
#include "uart.h"
#include "MAX30102_n.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG	0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

HAL_StatusTypeDef ret;

max30102_t max30102;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void max30102_startSeq();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	uart_print_config(9600);
	print("Working\n\r");

	max30102_init(&max30102, &hi2c1);
	max30102_reset(&max30102);
	max30102_clear_fifo(&max30102);
	max30102_set_fifo_config(&max30102, max30102_smp_ave_8, 1, 7);

	// Sensor settings
	max30102_set_led_pulse_width(&max30102, max30102_pw_16_bit);
	max30102_set_adc_resolution(&max30102, max30102_adc_8192);
	max30102_set_sampling_rate(&max30102, max30102_sr_800);
	max30102_set_led_current_1(&max30102, 5);
	max30102_set_led_current_2(&max30102, 5);

	// Enter SpO2 mode
	max30102_set_mode(&max30102, max30102_spo2);
//	max30102_set_a_full(&max30102, 1);

	// Initiate 1 temperature measurement
	max30102_set_die_temp_en(&max30102, 1);
	max30102_set_die_temp_rdy(&max30102, 1);

	uint8_t en_reg[2] = { 0 };
	max30102_read(&max30102, 0x00, en_reg, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		max30102_update(&max30102);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
//void max30102_startSeq() {
//
//	uint8_t max30102Ret = true;
//
//	if (max30102_init(&max30102, &hi2c1) != true) {
//		print("Error Init: Max30102\n\r");
//	} else {
//		print("Init Successful: Max30102\n\r");
//	}
//	max30102Ret = max30102_reset(&max30102);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_reset\n\r");
//	} else {
//		print("Error: max30102_reset\n\r");
//	}
//#endif
//	max30102Ret = max30102_clear_fifo(&max30102);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_clear_fifo\n\r");
//	} else {
//		print("Error: max30102_clear_fifo\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_fifo_config(&max30102, max30102_smp_ave_16, 1,
//			7);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_fifo_config\n\r");
//	} else {
//		print("Error: max30102_set_fifo_config\n\r");
//	}
//#endif
//
//	uint8_t tempInt, tempFrac;
//	max30102Ret = max30102_read_temp(&max30102, &tempInt, &tempFrac);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_read_temp\n\r");
//	} else {
//		print("Error: max30102_read_temp\n\r");
//	}
//#endif
//	print("Temp: %d.%d\n\r", tempInt, tempFrac);
//
//	// Sensor settings
//	max30102Ret = max30102_set_led_pulse_width(&max30102, max30102_pw_16_bit);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_led_pulse_width\n\r");
//	} else {
//		print("Error: max30102_set_led_pulse_width\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_adc_resolution(&max30102, max30102_adc_8192);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_adc_resolution\n\r");
//	} else {
//		print("Error: max30102_set_adc_resolution\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_sampling_rate(&max30102, max30102_sr_800);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_sampling_rate\n\r");
//	} else {
//		print("Error: max30102_set_sampling_rate\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_led_current_1(&max30102, 0x20);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_led_current_1\n\r");
//	} else {
//		print("Error: max30102_set_led_current_1\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_led_current_2(&max30102, 0x20);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_led_current_2\n\r");
//	} else {
//		print("Error: max30102_set_led_current_2\n\r");
//	}
//#endif
//
//	// Enter SpO2 mode
//	max30102Ret = max30102_set_mode(&max30102, max30102_spo2);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_mode\n\r");
//	} else {
//		print("Error: max30102_set_mode\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_a_full(&max30102, 1);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_a_full\n\r");
//	} else {
//		print("Error: max30102_set_a_full\n\r");
//	}
//#endif
//
//	// Initiate 1 temperature measurement
//	max30102Ret = max30102_set_die_temp_en(&max30102, 1);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_die_temp_en\n\r");
//	} else {
//		print("Error: max30102_set_die_temp_en\n\r");
//	}
//#endif
//	max30102Ret = max30102_set_die_temp_rdy(&max30102, 1);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_set_die_temp_rdy\n\r");
//	} else {
//		print("Error: max30102_set_die_temp_rdy\n\r");
//	}
//#endif
//
//	uint8_t en_reg[2] = { 0 };
//	max30102Ret = max30102_readByte(&max30102, 0x00, en_reg);
//	Delay(1);
//#if	DEBUG
//	if (max30102Ret) {
//		print("Success: max30102_readByte\n\r");
//	} else {
//		print("Error: max30102_readByte\n\r");
//	}
//#endif
//}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
