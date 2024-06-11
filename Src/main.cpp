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

#include "KeyboardScan.h"
#include "hid_keyboard.h"
#include "py32f071_ll_bus.h"
#include "py32f071_ll_gpio.h"
#include "py32f071_ll_spi.h"
#include "py32f071_ll_utils.h"
#include "py32f0xx_hal.h"

#include "ws2812.h"

#include <vector>

#include <ranges>

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
WS2812 ws2812(SPI1, 17);
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

extern "C" void usb_dc_low_level_init(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    __HAL_RCC_USB_CLK_ENABLE();

    HAL_NVIC_SetPriority(USB_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_IRQn);
}

static void SPIInit()
{
    // PB5 MOSI
    LL_SPI_InitTypeDef SPI_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 48M主频，SPI需要6M
    SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
    SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
    SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
    SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
    SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
    SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
    SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
    SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
    SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
    LL_SPI_Init(SPI1, &SPI_InitStruct);
    LL_SPI_Enable(SPI1);
}

/**
 * @brief  Main program.
 * @retval int
 */
int main(void)
{
    /* Reset of all peripherals, Initializes the Systick. */
    HAL_Init();

    /* System clock configuration */
    APP_SystemClockConfig();

    hid_keyboard_init(0, USBD_BASE);

    KeyboardInit();
    SPIInit();
    ws2812.Init();

    for (int i = 0; i < 17; i++)
    {
        if (i < 4)
        {
            ws2812.SetColor(i, 10, 10, 77);
        }
        else if (i < 8)
        {
            ws2812.SetColor(i, 89, 64, 69);
        }
        else if (i < 11)
        {
            ws2812.SetColor(i, 89, 89, 89);
        }
        else if (i < 14)
        {
            ws2812.SetColor(i, 89, 64, 69);
        }
        else
        {
            ws2812.SetColor(i, 10, 10, 77);
        }
    }
    while (1)
    {
        KeyboardSend();
        ws2812.Update();
    }
}

/**
 * @brief  System clock configuration function
 * @param  None
 * @retval None
 */
static void APP_SystemClockConfig(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Oscillator configuration */
    RCC_OscInitStruct.OscillatorType =
        RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI |
        RCC_OSCILLATORTYPE_LSE;              /* Select oscillators HSE, HSI, LSI, LSE */
    RCC_OscInitStruct.HSIState = RCC_HSI_ON; /* Enable HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1; /* HSI not divided */
    RCC_OscInitStruct.HSICalibrationValue =
        RCC_HSICALIBRATION_24MHz;             /* Configure HSI clock as 16MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_OFF; /* Disable HSE */
    /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF; /* Disable LSI */
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF; /* Disable LSE */
    /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;         /* Enable PLL */
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI; /* PLL source is HSI */
    RCC_OscInitStruct.PLL.PLLMUL =
        RCC_PLL_MUL2; /* PLL multiplication factor is 3 */
    /* Configure oscillator */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        APP_ErrorHandler();
    }

    /* Clock source configuration */
    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1; /* Configure clock types HCLK, SYSCLK, PCLK1 */
    RCC_ClkInitStruct.SYSCLKSource =
        RCC_SYSCLKSOURCE_PLLCLK;                       /* Select PLL as system clock */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; /* AHB clock not divided */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  /* APB clock not divided */
    /* Configure clock source */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        APP_ErrorHandler();
    }
}

/**
 * @brief  Error executing function.
 * @param  None
 * @retval None
 */
void APP_ErrorHandler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles System tick timer.
 */
extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
    KeyboardScan_Callback();
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* Users can add their own printing information as needed,
       for example: printf("Wrong parameters value: file %s on line %d\r\n", file,
       line) */
    /* Infinite loop */
    while (1)
    {
    }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
