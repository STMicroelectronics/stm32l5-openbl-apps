/**
  ******************************************************************************
  * @file    stm32l5xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *          This file provides code for the MSP Initialization
  *          and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "interfaces_conf.h"
#include "stm32l5xx_ll_gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/
  /* Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral */
  HAL_PWREx_DisableUCPDDeadBattery();
}

/**
  * @brief  De-Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspDeInit(void)
{
}

/**
  * @brief This function configures FDCAN used hardware resources
  * @param hfdcan: FDCAN handle pointer
  * @retval None
  */
void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef *hfdcan)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* Enable all resources clocks --------------------------------------------*/
  /* Enable used GPIOx clocks */
  FDCANx_GPIO_CLK_TX_ENABLE();
  FDCANx_GPIO_CLK_RX_ENABLE();

  /* Enable FDCANx clock */
  FDCANx_CLK_ENABLE();

  /* Configure peripheral GPIO */
  /* Configure FDCAN TX as alternate function */
  GPIO_InitStruct.Pin        = FDCANx_TX_PIN;
  GPIO_InitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate  = FDCANx_TX_AF;
  LL_GPIO_Init(FDCANx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* Configure FDCAN RX as alternate function */
  GPIO_InitStruct.Pin = FDCANx_RX_PIN;
  LL_GPIO_Init(FDCANx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
* @brief  DeInitializes the FDCAN MSP.
* @param  hfdcan: pointer to an FDCAN_HandleTypeDef structure that contains
*         the configuration information for the specified FDCAN.
* @retval None
*/
void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef *hfdcan)
{
  /* 1- Reset peripherals */
  FDCANx_FORCE_RESET();
  FDCANx_RELEASE_RESET();

  /* 2- Disable peripherals and GPIO Clocks */
  /* Configure FDCANx Tx as alternate function */
  HAL_GPIO_DeInit(FDCANx_TX_GPIO_PORT, FDCANx_TX_PIN);

  /* Configure FDCANx Rx as alternate function */
  HAL_GPIO_DeInit(FDCANx_RX_GPIO_PORT, FDCANx_RX_PIN);
}
