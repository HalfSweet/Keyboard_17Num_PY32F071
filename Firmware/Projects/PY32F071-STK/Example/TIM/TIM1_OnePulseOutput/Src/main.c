/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef    TimHandle;
TIM_OnePulse_InitTypeDef sConfig;

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick */
  HAL_Init();
  
  /* Configure Systemclock */
  APP_SystemClockConfig(); 

  TimHandle.Instance = TIM1;                                           /* Select TIM1 */
  TimHandle.Init.Period            = 0xffff;                           /* Auto reload value： */
  TimHandle.Init.Prescaler         = 1 - 1;                            /* Prescaler:1-1 */
  TimHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           /* Clock division: tDTS=tCK_INT */
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;               /* CounterMode:Up */
  TimHandle.Init.RepetitionCounter = 1 - 1;                            /* repetition counter value:1-1 */
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* TIM1_ARR register is not buffered */
  /* Initializes the TIM1 One Pulse mode */
  if (HAL_TIM_OnePulse_Init(&TimHandle, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  sConfig.OCMode       = TIM_OCMODE_PWM2;                              /* Set as PWM2 mode  */
  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;                          /* OC channel active high */
  sConfig.Pulse        = 16383;                                        /* Pulse:16383 */
  sConfig.ICPolarity   = TIM_ICPOLARITY_RISING;                        /* Capture triggered by rising edge on timer input */
  sConfig.ICSelection  = TIM_ICSELECTION_DIRECTTI;                     /* CC2 channel connect to TI2 */
  sConfig.ICFilter     = 0;                                            /* No filter */
  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;                         /* OCN channel active high */
  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;                        /* OC1 channel idle state is low level */
  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;                       /* OC1N channel idle state is low level */
  /* Initializes the TIM One Pulse Channels */
  if (HAL_TIM_OnePulse_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1, TIM_CHANNEL_2) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Starts the TIM1 One Pulse signal generation. */
  if (HAL_TIM_OnePulse_Start(&TimHandle, TIM_CHANNEL_1) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configure system clock
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* RCC Oscillators configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillators HSE,HSI,LSI,LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* HSI ON */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI clock is not divided */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_16MHz; /* Configure HSI to 16Mhz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* HSE OFF */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* LSI OFF */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* LSE OFF */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                      /* PLL ON */
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;              /* HSI clock selected as PLL entry clock source */
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  /* Initialize RCC Oscillators */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure HCLK,SYSCLK,PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* Select PLL as system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* SYSCLK not divided */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* HCLK not divided */
  /* Configure Clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  Error handling function
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file：Pointer to the source file name
  * @param  line：assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add His own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
