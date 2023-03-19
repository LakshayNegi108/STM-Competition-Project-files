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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "BML_DEF.h"
#include "uart.h"
#include "MLX90614.h"
#include "max30100.h"
#include "hms.h"
#include "st7783.h"
#include "SDCard.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PRINT_CHAR_LEN	100

#define BEAT_COUNTER_LIMIT	10
#define HR_BEAT_TIMEOUT		60000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

char globalStr[25] = { '\0' };
float globalTemp = 1.00;
int globalHR = 1;
int globalSpO2 = 1;
int globalECG = 1;

int8_t menu_select_flag = -1;

//======== MLX90614 Variables ==========//
float temp = 0;
//======================================//

//=========== ECG Variables ============//
int16_t adcData = 0;
int16_t adcDataPrev = 0;
int ECG_HR = 0;
//======================================//

//======== MAX30100 Variables ==========//
max30100_config_t max30100 = { 0 };
uint8_t beatCounter = 7;
int16_t finalHR = 0;
int16_t finalSpO2 = 0;
uint16_t hrBeatTimer = 0;

bool max30100_off = true;
bool max30100_on = false;
//======================================//

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_ADC1_Init();
	MX_I2C1_Init();
	MX_SPI1_Init();
	MX_FATFS_Init();
	/* USER CODE BEGIN 2 */
	uart_print_config(9600);
	USART_INIT(USART6, PORTA, PORTA, 11, 12, 115200);
//  USART_IT_EN(USART6, 0, 1, 0);
	print("Working\r\n");

	gpio_write(PORTB, 6, LOW);

	sd_mount();
	Delay(500);
	sd_space();

	LCD_Begin();
	LCD_SetRotation(0);
	LCD_FillScreen(BLACK);
	displayPic24bit("hmsbg.bmp", 0, 0);

	HMS_bgdisplay();

	sd_deMount();

	for (int i = 4; i > 0; i--)
		LCD_ECGAnimation();

	gpio_config(PORTC, 9, INPUT_MODE, LOW_SPEED, EN_PU, PHPL);
	gpio_IT_config(PORTC, 9, FALLING_EDGE);
	gpio_IT_EN(9, EXTI9_5_IRQn);

	uint8_t mlx90614_workingFlag = 1, mlx90614_status = 0;

	char print_char[PRINT_CHAR_LEN] = { 0 };

	if (!(mlx90614_init())) {
		print("Error in MLX90614 INIT\n");
		mlx90614_workingFlag = 0;
	}

	uint32_t max30100_startTime = HAL_GetTick(), max30100_lapsedTime = 500;
	uint32_t temp_startTime = HAL_GetTick(), temp_lapsedTime = 500;
	uint32_t ecg_startTime = HAL_GetTick(), ecg_lapsedTime = 60000;
	uint32_t wifi_startTime = HAL_GetTick(), wifi_lapsedTime = 2000;
	max30100_data_t result = { };

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		if (menu_select_flag == 0) {
			if (mlx90614_workingFlag == 1) {
				mlx90614_status = mlx90614_getObject1(&temp);
				if ((mlx90614_status == true)
						&& ((HAL_GetTick() - temp_startTime) > temp_lapsedTime)) {
					print("Temp in Celsius: = %0.2f\n", temp);
					if ((temp > 0) && (temp < 100)) {
						LCD_SetTextColor(WHITE, LCD_Color565(30, 35, 247));
						LCD_SetTextSize(2);
						LCD_SetCursor(30, 145);
						LCD_Printf("%0.1f", temp);
						globalTemp = temp;
					}
					temp_startTime = HAL_GetTick();
					//					Delay(500);
				} else {
					mlx90614_workingFlag = 0;
				}
			} else {
				if (!(mlx90614_init())) {
					print("Error in MLX90614 INIT\n");
					LCD_SetTextColor(WHITE, LCD_Color565(30, 35, 247));
					LCD_SetTextSize(2);
					LCD_SetCursor(30, 145);
					LCD_Printf("....");
					MX_I2C1_Init();
//					Delay(500);
					mlx90614_workingFlag = 0;
				} else {
					mlx90614_workingFlag = 1;
				}
			}
		} else if (menu_select_flag == 1) {
			if (max30100_update(&max30100, &result) != HAL_OK) {
				print("Error Reading...Retrying\n");
				MX_I2C1_Init();
				max30100_init(&max30100, &hi2c1,
				MAX30100_DEFAULT_OPERATING_MODE,
				MAX30100_DEFAULT_SAMPLING_RATE,
				MAX30100_DEFAULT_LED_PULSE_WIDTH,
				MAX30100_DEFAULT_IR_LED_CURRENT,
				MAX30100_DEFAULT_START_RED_LED_CURRENT,
				MAX30100_DEFAULT_MEAN_FILTER_SIZE,
				MAX30100_DEFAULT_PULSE_BPM_SAMPLE_SIZE,
				true,
				false);
				//				max30100_resume(&max30100);
				Delay(1000);
			} else {
				if (max30100_off) {
					max30100_resume(&max30100);
					max30100_off = false;
					max30100_on = true;
				}
				hrBeatTimer = HAL_GetTick();
				if (result.pulse_detected) {
					print("BEAT: %d\n", beatCounter++);
				}
				if (HAL_GetTick() - max30100_startTime > max30100_lapsedTime) {
					if (result.ir_dc_value < 10000.00) {
						print("No finger Detected\n");
						max30100.current_bpm = 0.0;
						max30100.current_spO2 = 0.0;
					} else {
						if (beatCounter < BEAT_COUNTER_LIMIT) {
							print("BPM: %0.2f | SpO2: %0.2f%%\n",
									result.heart_bpm, result.spO2);
						} else if (beatCounter >= BEAT_COUNTER_LIMIT) {
							finalHR = result.heart_bpm;
							finalSpO2 = result.spO2;
							finalSpO2++;
							print(
									"//===============| Final HR: %d | SpO2: %d%%|==============//\n",
									finalHR, finalSpO2);
							beatCounter = 0;

							if (finalHR > 0) {
								globalHR = finalHR;
								LCD_SetTextColor(WHITE, RED);
								LCD_SetTextSize(2);
								LCD_SetCursor(38, 70);
								if(finalHR < 100)	LCD_Printf("%d ", globalHR);
								else 				LCD_Printf("%d", globalHR);
							}
							if ((finalSpO2 > 0) && (finalSpO2 < 100)) {
								globalSpO2 = finalSpO2;
								LCD_SetTextColor(WHITE, LCD_Color565(16, 220, 21));
								LCD_SetCursor(160, 70);
								LCD_Printf("%d", finalSpO2);
							}
						}
					}

					max30100_startTime = HAL_GetTick();
				}
				HAL_Delay(10);

				//				print("%0.2f, %0.2f\n", result.dc_filtered_ir,
				//						result.dc_filtered_red);
			}
		} else if (menu_select_flag == 2) {
			if (max30100_on) {
				max30100_shutdown(&max30100);
				HAL_I2C_DeInit(&hi2c1);
				max30100_off = true;
				max30100_on = false;
			}
			adcDataPrev = adcData;
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, 100);
			adcData = (uint16_t) HAL_ADC_GetValue(&hadc1);
			sprintf(print_char, "Analog Val: %d\n", adcData);
			USART_WRITE(USART2, print_char, strlen(print_char), 10);

			if ((adcData - adcDataPrev) > 800) {
				ECG_HR += 1;
			}

			//			LCD_ECGAnimation();

			if ((HAL_GetTick() - ecg_startTime) > ecg_lapsedTime) {
				if ((ECG_HR > 0) && (ECG_HR < 100))
					globalECG = ECG_HR;

				sprintf(print_char,
						"<===========|The ECG Heart Rate is: %d|=========>\r\n",
						ECG_HR);
				USART_WRITE(USART2, print_char, strlen(print_char), 10);
				LCD_SetTextColor(BLACK, WHITE);
				LCD_SetCursor(180, 270);
				LCD_SetTextSize(2);
				if (globalECG < 10)
					LCD_Printf(" %d", globalECG);
				else if (globalECG < 100)
					LCD_Printf("%d", globalECG);
				ECG_HR = 0;
				LCD_ECGAnimation();
				ecg_startTime = HAL_GetTick();
			}

		} else if ((menu_select_flag < -1) && (menu_select_flag > 3)) {
			menu_select_flag = -1;
			print("Oh no, a Glitch!!\n\r");
			Delay(500);
			print("We're good\n\r");
			Delay(500);
		}

		if ((HAL_GetTick() - wifi_startTime) > wifi_lapsedTime) {
			sprintf(globalStr, "T:%0.2f|H:%d|S:%d|E:%d\r\n", globalTemp,
					globalHR, globalSpO2, globalECG);
			USART_WRITE(USART6, globalStr, strlen(globalStr), 10);
			wifi_startTime = HAL_GetTick();
		}
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

	/* USER CODE BEGIN ADC1_Init 0 */

	/* USER CODE END ADC1_Init 0 */

	ADC_ChannelConfTypeDef sConfig = { 0 };

	/* USER CODE BEGIN ADC1_Init 1 */

	/* USER CODE END ADC1_Init 1 */

	/** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
	 */
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	if (HAL_ADC_Init(&hadc1) != HAL_OK) {
		Error_Handler();
	}

	/** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
	 */
	sConfig.Channel = ADC_CHANNEL_14;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN ADC1_Init 2 */

	/* USER CODE END ADC1_Init 2 */

}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

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
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

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
	if (HAL_UART_Init(&huart2) != HAL_OK) {
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
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : SD_CS_Pin */
	GPIO_InitStruct.Pin = SD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void EXTI9_5_IRQHandler() {
	if (gpio_IT_CHK(9)) {
		for (int i = 2000000; i > 0; i--)
			;
		menu_select_flag = (menu_select_flag + 1) % 3;

//		print("interrupt: %d\r\n", menu_select_flag);
		gpio_IT_CLR(9);
	}
}
/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM11 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM11) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
