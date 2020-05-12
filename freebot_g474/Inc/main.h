/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_pwr.h"

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

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define hrperiod 27200
#define main_loop_period 170e6/10000
#define v_a_Pin GPIO_PIN_0
#define v_a_GPIO_Port GPIOC
#define v_b_Pin GPIO_PIN_1
#define v_b_GPIO_Port GPIOC
#define v_c_Pin GPIO_PIN_2
#define v_c_GPIO_Port GPIOC
#define i_bus_Pin GPIO_PIN_3
#define i_bus_GPIO_Port GPIOC
#define v_bus_Pin GPIO_PIN_3
#define v_bus_GPIO_Port GPIOA
#define spi1cs_Pin GPIO_PIN_4
#define spi1cs_GPIO_Port GPIOA
#define i_a_Pin GPIO_PIN_11
#define i_a_GPIO_Port GPIOB
#define i_b_Pin GPIO_PIN_12
#define i_b_GPIO_Port GPIOB
#define i_c_Pin GPIO_PIN_13
#define i_c_GPIO_Port GPIOB
#define pwm_c_Pin GPIO_PIN_14
#define pwm_c_GPIO_Port GPIOB
#define pwm_cl_Pin GPIO_PIN_15
#define pwm_cl_GPIO_Port GPIOB
#define pwm_b_Pin GPIO_PIN_6
#define pwm_b_GPIO_Port GPIOC
#define pwm_bl_Pin GPIO_PIN_7
#define pwm_bl_GPIO_Port GPIOC
#define pwm_a_Pin GPIO_PIN_8
#define pwm_a_GPIO_Port GPIOC
#define pwm_al_Pin GPIO_PIN_9
#define pwm_al_GPIO_Port GPIOC
#define spi3cs1_Pin GPIO_PIN_2
#define spi3cs1_GPIO_Port GPIOD
#define spi3cs2_Pin GPIO_PIN_4
#define spi3cs2_GPIO_Port GPIOB
#define spi3cs3_Pin GPIO_PIN_5
#define spi3cs3_GPIO_Port GPIOB
#define led_r_Pin GPIO_PIN_6
#define led_r_GPIO_Port GPIOB
#define led_g_Pin GPIO_PIN_7
#define led_g_GPIO_Port GPIOB
#define led_b_Pin GPIO_PIN_8
#define led_b_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
