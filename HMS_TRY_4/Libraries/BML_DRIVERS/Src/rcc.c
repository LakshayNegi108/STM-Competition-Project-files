/*
 * rcc.c
 *
 *  Created on: 29-Oct-2022
 *      Author: hp
 */
#include "rcc.h"

//*****************************| RCC CLOCK FUNCTION |****************************//
BML_StatusTypeDef RCC_CONFIG_84MHZ() {
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
//	    Error_Handler();
		  while(1);
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
//	    Error_Handler();
		  while(1);
	  }
}

BML_StatusTypeDef RCC_CONFIG_84MHZ_BML(uint16_t timeout) {
//	uint16_t i = timeout;
//	if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL) /* (1) */
//	{
//		RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW); /* (2) */
//		while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI) /* (3) */
//		{
//			RCC_Delay(1);
//			i--;
//			if (i <= 0) {
//				return BML_ERROR;
//			}
//		}
//	}
//	i = timeout;
//	RCC->CR &= (uint32_t) (~RCC_CR_PLLON);/* (4) */
//	while ((RCC->CR & RCC_CR_PLLRDY) != 0) /* (5) */
//	{
//		RCC_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_ERROR;
//		}
//	}
//	i = timeout;
//	RCC->CFGR = (RCC->CFGR & (~RCC_CFGR_PLLMUL)) | (RCC_CFGR_PLLMUL6); /* (6) */
//	//	RCC->CFGR = RCC_CFGR_PPRE_DIV1;
//	RCC->CR |= RCC_CR_PLLON; /* (7) */
//	while ((RCC->CR & RCC_CR_PLLRDY) == 0) /* (8) */
//	{
//		RCC_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_ERROR;
//		}
//	}
//	i = timeout;
//	RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL); /* (9) */
//	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) /* (10) */
//	{
//		RCC_Delay(1);
//		i--;
//		if (i <= 0) {
//			return BML_ERROR;
//		}
//	}
//
//	SystemCoreClockUpdate();
//	SysTick_Config(SystemCoreClock / 1000);
}

//*******************************************************************************//
