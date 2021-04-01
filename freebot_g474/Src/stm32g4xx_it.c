/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../motorlib/system.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void ADC5_IRQHandler(void) __attribute__((section (".ccmram")));
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define INTERRUPT_PROFILE_GLOBALS(loop) uint32_t t_exec_##loop __attribute__((used));\
                                        uint32_t t_period_##loop __attribute__((used));
#define INTERRUPT_PROFILE_START static uint32_t last_start = 0; \
                                      uint32_t t_start = get_clock();
#define INTERRUPT_PROFILE_END(loop) t_exec_##loop = get_clock()-t_start; \
                                      t_period_##loop = t_start - last_start; \
                                      last_start = t_start;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "../../motorlib/util.h"
INTERRUPT_PROFILE_GLOBALS(fastloop);
INTERRUPT_PROFILE_GLOBALS(mainloop);
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
// extern ADC_HandleTypeDef hadc5;
// extern DMA_HandleTypeDef hdma_spi1_tx;
// extern DMA_HandleTypeDef hdma_spi1_rx;
// extern TIM_HandleTypeDef htim1;
// extern PCD_HandleTypeDef hpcd_USB_FS;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
// void DMA1_Channel1_IRQHandler(void)
// {
//   /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

//   /* USER CODE END DMA1_Channel1_IRQn 0 */
//   HAL_DMA_IRQHandler(&hdma_spi1_tx);
//   /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

//   /* USER CODE END DMA1_Channel1_IRQn 1 */
// }

// /**
//   * @brief This function handles DMA1 channel2 global interrupt.
//   */
// void DMA1_Channel2_IRQHandler(void)
// {
//   /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

//   /* USER CODE END DMA1_Channel2_IRQn 0 */
//   HAL_DMA_IRQHandler(&hdma_spi1_rx);
//   /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

//   /* USER CODE END DMA1_Channel2_IRQn 1 */
// }

/**
  * @brief This function handles USB low priority interrupt remap.
  */
void USB_LP_IRQHandler(void)
{
  /* USER CODE BEGIN USB_LP_IRQn 0 */
  scope3_GPIO_Port->BSRR |= scope3_Pin; 
  usb_interrupt();
#if 0
  /* USER CODE END USB_LP_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
  /* USER CODE BEGIN USB_LP_IRQn 1 */
#endif
  scope3_GPIO_Port->BSRR |= scope3_Pin << 16; 
  /* USER CODE END USB_LP_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM16 global interrupt.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */
  //scope2_GPIO_Port->BSRR |= scope2_Pin; 
  INTERRUPT_PROFILE_START;
  main_loop_interrupt();
#if 0
  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */
#endif
  TIM1->SR = 0;
  INTERRUPT_PROFILE_END(mainloop);
  //scope2_GPIO_Port->BSRR |= scope2_Pin << 16; 
  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles ADC5 global interrupt.
  */
void ADC5_IRQHandler(void)
{
  /* USER CODE BEGIN ADC5_IRQn 0 */
  //scope1_GPIO_Port->BSRR |= scope1_Pin; 
  INTERRUPT_PROFILE_START;
  fast_loop_interrupt();
#if 0
  /* USER CODE END ADC5_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc5);
  /* USER CODE BEGIN ADC5_IRQn 1 */
#endif
  ADC5->ISR = ADC_ISR_JEOC;
  INTERRUPT_PROFILE_END(fastloop)
  //scope1_GPIO_Port->BSRR |= scope1_Pin << 16; 
  /* USER CODE END ADC5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
