/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
void delay_us(uint32_t us_duration);
uint16_t clamp(uint16_t val, uint16_t v_min, uint16_t v_max);
float clampf(float val, float v_min, float v_max);
uint16_t map_uint16(uint16_t x, uint16_t i_min, uint16_t i_max, uint16_t o_min, uint16_t o_max);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PSC_5 96-1
#define ARR_1 500-1
#define PSC_11 96-1
#define PSC_4 1-1
#define ARR_11 1000-1
#define ARR_4 9600-1
#define PSC_1 96-1
#define PSC_2 96-1
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define US_ECHO_Pin GPIO_PIN_1
#define US_ECHO_GPIO_Port GPIOA
#define US_TRIG_Pin GPIO_PIN_2
#define US_TRIG_GPIO_Port GPIOA
#define IMU_SCL_Pin GPIO_PIN_10
#define IMU_SCL_GPIO_Port GPIOB
#define ESC_M_CW_Pin GPIO_PIN_8
#define ESC_M_CW_GPIO_Port GPIOA
#define ESC_M_CCW_Pin GPIO_PIN_9
#define ESC_M_CCW_GPIO_Port GPIOA
#define IMU_SDA_Pin GPIO_PIN_3
#define IMU_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
