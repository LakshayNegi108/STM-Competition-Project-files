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
#include "MLX90614.h"
#include "st7783.h"
#include "FakeECG.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define graphRefreshRate_peak			2
#define graphRefreshRate_normal			5

#define ECG_Print		0
#define Temp_Print		0
#define Oximeter_Print	1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define ADC_BUF_LEN	4096
#define PRINT_CHAR_LEN	100

const int graph_x0 = 30;
const int graph_y0 = (2 * (TFTHEIGHT / 3)) + 30;
const int graph_x1 = 30;
const int graph_y1 = graph_y0 + 50;
const int graph_x2 = graph_x0 + TFTWIDTH - 95;
const int graph_y2 = graph_y0;
const int graph_x3 = graph_x0 + TFTWIDTH - 95;
const int graph_y3 = graph_y0 + 50;
int graph_x = graph_x0;
int graph_y = 0;
int graph_y_prevVal = 0;
int graph_refresh_x = graph_x0;

float temp = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void HMS_bgdisplay(void);

uint16_t adc_buf[ADC_BUF_LEN] = { 0 };
char print_char[PRINT_CHAR_LEN] = { 0 };
uint16_t adcData;

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
	LCD_Begin();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_ADC1_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	uart_print_config(9600);
	print("Working\n");
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buf, ADC_BUF_LEN);

	uint8_t mlx90614_wrokingFlag = 1, status = 0;

	if (!(mlx90614_init())) {
		print("Error in MLX90614 INIT\n");
		mlx90614_wrokingFlag = 0;
	}

//	LCD_FillScreen(BLACK);

	HMS_bgdisplay();
//	HAL_ADC_Start(&hadc1);

	uint16_t i = 0, j = 0, start = 0, end = 0;

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
//		end = HAL_GetTick();
//		HAL_ADC_Start(&hadc1);
//		HAL_ADC_PollForConversion(&hadc1, 100);
//		adcData = (uint16_t) HAL_ADC_GetValue(&hadc1);

		adcData = fakeECGData[i];
		i = (i + 1) % 5000;
		LCD_DrawGraph(adcData);

		status = mlx90614_getObject1(&temp);
		if (status == 1) {
			print("Temp in Celsius: = %0.2f\n", temp);
		}

//		sprintf(print_char, "%d\n", adcData);
//		USART_WRITE(USART2, print_char, strlen(print_char), 100);

//		if (HAL_GetTick() - start > 1) {
//			sprintf(print_char, "%d\n", adcData);
//			USART_WRITE(USART2, print_char, strlen(print_char), 100);
//			start = HAL_GetTick();
//		}

//		HAL_Delay(100);
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
void HMS_bgdisplay(void) {

	colorgradient(0, 0, TFTWIDTH, TFTHEIGHT, 50);

	//===================== | Border |===============================//
	for (uint16_t i = 5; i > 0; i--) {
		LCD_DrawRect(i, i, TFTWIDTH - 2 * i, TFTHEIGHT - 2 * i, WHITE);
	}

	LCD_DrawHBorder(0, TFTHEIGHT / 3, TFTWIDTH, WHITE, 5);
	LCD_DrawHBorder(0, 2 * (TFTHEIGHT / 3), TFTWIDTH, WHITE, 5);
	//===============================================================//

	//===================== | Box 1 |===============================//

	LCD_DrawButton1(" ", 20, 20, TFTWIDTH - 40, (TFTHEIGHT / 3) - 40, 10, CYAN,
	WHITE, 0, 2, 2, 2);

	LCD_SetTextSize(2);
	LCD_SetTextColor(WHITE, CYAN);
	LCD_SetCursor(((TFTWIDTH - 20) / 3) + 15, 25);
	LCD_Printf("Health");
	LCD_SetCursor(((TFTWIDTH - 20) / 3) - 10, 45);
	LCD_Printf("Monitoring");
	LCD_SetCursor(((TFTWIDTH - 20) / 3) + 15, 65);
	LCD_Printf("System");

	//===============================================================//

	//===================== | Box 2 |===============================//

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 15, TFTWIDTH - 35, 25, 5, RED);
	LCD_SetTextColor(WHITE, RED);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 20);
	LCD_SetTextSize(2);
	LCD_Printf("H.Rate: ");

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 45, TFTWIDTH - 35, 25, 5, GREEN);
	LCD_SetTextColor(WHITE, GREEN);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 50);
	LCD_SetTextSize(2);
	LCD_Printf("SpO2: ");

	LCD_FillRoundRect(15, (TFTHEIGHT / 3) + 75, TFTWIDTH - 35, 25, 5, BLUE);
	LCD_SetTextColor(WHITE, BLUE);
	LCD_SetCursor(20, (TFTHEIGHT / 3) + 80);
	LCD_SetTextSize(2);
	LCD_Printf("Temp: ");

	//===============================================================//

	//========================= | Box 3 |============================//

	LCD_FillRect(20, (2 * (TFTHEIGHT / 3)) + 20, TFTWIDTH - 35, 70, WHITE);

	//===============================================================//

}

void LCD_DrawGraph(uint16_t value) {

	graph_y_prevVal = graph_y;
//	uint16_t mapVar = adcArr[adcArrTail];
//	adcArrTail = (adcArrTail + 1) % adcArrLen;
	uint16_t mapVar = value;
	uint8_t graphRefreshRate = 0;
	if (mapVar > 3000)
		mapVar = 3000;
	else if (mapVar < 1000)
		mapVar = 1000;
	graph_y = map(mapVar, 0, 4095, 243, 293);

	LCD_DrawLine(graph_x, graph_y0 + graph_y_prevVal,
			graph_x + graphRefreshRate_peak, graph_y0 + graph_y, RED);
	LCD_DrawLine(graph_x, graph_y0 + graph_y_prevVal - 1,
			graph_x + graphRefreshRate_peak, graph_y0 + graph_y - 1, RED);

	int a = graph_x;
	if (a + graphRefreshRate_peak >= graph_x2) {
		LCD_FillRect(20, (2 * (TFTHEIGHT / 3)) + 20, TFTWIDTH - 80, 70, WHITE);
		graph_x = graph_x0;
	} else {
		graph_x += graphRefreshRate_peak;
	}

}

/* USER CODE END 4 */

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
