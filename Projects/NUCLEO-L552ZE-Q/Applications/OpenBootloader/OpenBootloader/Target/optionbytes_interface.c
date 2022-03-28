/**
  ******************************************************************************
  * @file    optionbytes_interface.c
  * @author  MCD Application Team
  * @brief   Contains Option Bytes access functions
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
#include "platform.h"
#include "openbl_mem.h"
#include "app_openbootloader.h"
#include "common_interface.h"
#include "optionbytes_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef OB1_Descriptor =
{
  OB1_START_ADDRESS,
  OB1_END_ADDRESS,
  OB1_SIZE,
  OB_AREA,
  OPENBL_OB_Read,
  OPENBL_OB_Write,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Launch the option byte loading.
  * @retval None.
  */
void OPENBL_OB_Launch(void)
{
  /* Set the option start bit */
  HAL_FLASH_OB_Launch();

  /* Set the option lock bit and Lock the flash */
  HAL_FLASH_OB_Lock();
  HAL_FLASH_Lock();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_OB_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in Option bytes.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_OB_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  /* Unlock the FLASH & Option Bytes Registers access */
  HAL_FLASH_Unlock();
  HAL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  /* Write USER OPT + RDP level */
  if (DataLength >= 1U)
  {
    WRITE_REG(FLASH->OPTR, (*(Data) | (*(Data + 1U) << 8U) | (*(Data + 2U) << 16U) | (*(Data + 3U) << 24U)));
  }

  /* Write NSBOOTADD0R */
  if (DataLength >= 4U)
  {
    WRITE_REG(FLASH->NSBOOTADD0R, ((*(Data + 5U) << 8U) | (*(Data + 6U) << 16U) | (*(Data + 7U) << 24U)));
  }

  /* Write NSBOOTADD1R */
  if (DataLength >= 8U)
  {
    WRITE_REG(FLASH->NSBOOTADD1R, ((*(Data + 9U) << 8U) | (*(Data + 10U) << 16U) | (*(Data + 11U) << 24U)));
  }

  /* Write SECBOOTADD0R */
  if (DataLength >= 12U)
  {
    WRITE_REG(FLASH->SECBOOTADD0R, (*(Data + 12U) | (*(Data + 13U) << 8U) | (*(Data + 14U) << 16U) | (*
                                    (Data + 15U) << 24U)));
  }

  /* Write SECWM1R1 */
  if (DataLength >= 16U)
  {
    WRITE_REG(FLASH->SECWM1R1, (*(Data + 16U) | (*(Data + 18U) << 16U)));
  }

  /* Write SECWM1R2 */
  if (DataLength >= 20U)
  {
    WRITE_REG(FLASH->SECWM1R2, (*(Data + 22U) << 16U | (*(Data + 23U) << 24U)));
  }

  /* Write  WRP1AR*/
  if (DataLength >= 24U)
  {
    FLASH->WRP1AR = ((*(Data + 24U) | (*(Data + 26U) << 16U)));
  }

  /* Write  WRP1BR*/
  if (DataLength >= 28U)
  {
    FLASH->WRP1BR = ((*(Data + 28U) | (*(Data + 30U) << 16U)));
  }

  /* Write  SECWM2R1*/
  if (DataLength >= 32U)
  {
    WRITE_REG(FLASH->SECWM2R1, (*(Data + 32U) | (*(Data + 34U) << 16U)));
  }

  /* Write  SECWM2R2*/
  if (DataLength >= 36U)
  {
    WRITE_REG(FLASH->SECWM2R2, (*(Data + 38U) << 16U | (*(Data + 39U) << 24U)));
  }

  /* Write  WRP2AR*/
  if (DataLength >= 40)
  {
    FLASH->WRP2AR = ((*(Data + 40U) | (*(Data + 42U) << 16U)));
  }

  /* Write  WRP2BR*/
  if (DataLength >= 44U)
  {
    FLASH->WRP2BR = ((*(Data + 44U) | (*(Data + 46U) << 16U)));
  }

  /* Trigger options bytes programming operation */
  SET_BIT(FLASH->NSCR, FLASH_NSCR_OPTSTRT);
  
  /* Register system reset callback */
  Common_SetPostProcessingCallback(OPENBL_OB_Launch);
}
