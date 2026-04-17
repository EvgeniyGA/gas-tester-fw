/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PhotIn_2_Pin GPIO_PIN_6
#define PhotIn_2_GPIO_Port GPIOA
#define spi2_cs_a_Pin GPIO_PIN_2
#define spi2_cs_a_GPIO_Port GPIOB
#define spi2_cs_b_Pin GPIO_PIN_11
#define spi2_cs_b_GPIO_Port GPIOB
#define one_wire3_tx_Pin GPIO_PIN_8
#define one_wire3_tx_GPIO_Port GPIOD
#define one_wire3_rx_Pin GPIO_PIN_9
#define one_wire3_rx_GPIO_Port GPIOD
#define one_wire2_tx_Pin GPIO_PIN_6
#define one_wire2_tx_GPIO_Port GPIOC
#define one_wire2_rx_Pin GPIO_PIN_7
#define one_wire2_rx_GPIO_Port GPIOC
#define one_wire1_tx_Pin GPIO_PIN_5
#define one_wire1_tx_GPIO_Port GPIOD
#define one_wire1_rx_Pin GPIO_PIN_6
#define one_wire1_rx_GPIO_Port GPIOD
#define uart_tx_Pin GPIO_PIN_6
#define uart_tx_GPIO_Port GPIOB
#define uart_rx_Pin GPIO_PIN_7
#define uart_rx_GPIO_Port GPIOB
#define spi1_cs_a_Pin GPIO_PIN_8
#define spi1_cs_a_GPIO_Port GPIOB
#define spi1_cs_b_Pin GPIO_PIN_9
#define spi1_cs_b_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void setup(void);
void loop(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
