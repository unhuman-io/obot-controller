/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../motorlib/system.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
// ADC_HandleTypeDef hadc1;
// ADC_HandleTypeDef hadc3;
// ADC_HandleTypeDef hadc4;
// ADC_HandleTypeDef hadc5;

// HRTIM_HandleTypeDef hhrtim1;

// I2C_HandleTypeDef hi2c1;
// I2C_HandleTypeDef hi2c2;

// OPAMP_HandleTypeDef hopamp3;
// OPAMP_HandleTypeDef hopamp4;
// OPAMP_HandleTypeDef hopamp6;

// SPI_HandleTypeDef hspi1;
// SPI_HandleTypeDef hspi3;
// DMA_HandleTypeDef hdma_spi1_tx;
// DMA_HandleTypeDef hdma_spi1_rx;

// TIM_HandleTypeDef htim1;
// TIM_HandleTypeDef htim2;
// TIM_HandleTypeDef htim4;

// PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
uint32_t go_to_bootloader = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
// static void MX_GPIO_Init(void);
// static void MX_DMA_Init(void);
// static void MX_ADC1_Init(void);
// static void MX_ADC3_Init(void);
// static void MX_ADC4_Init(void);
// static void MX_ADC5_Init(void);
// static void MX_HRTIM1_Init(void);
// static void MX_OPAMP3_Init(void);
// static void MX_OPAMP4_Init(void);
// static void MX_OPAMP6_Init(void);
// static void MX_SPI1_Init(void);
// static void MX_SPI3_Init(void);
// static void MX_TIM1_Init(void);
// static void MX_TIM2_Init(void);
// static void MX_TIM4_Init(void);
// static void MX_USB_PCD_Init(void);
// static void MX_I2C1_Init(void);
// static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  // DWC CYCCNT used as main counter
  // CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  // DWT->CYCCNT = 0;
  // DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  //HAL_Init();

  /* USER CODE BEGIN Init */
  // todo: check if necessary
  //RCC->APB1ENR1 |= RCC_APB1ENR1_CRSEN;      // Clock recovery circuit, enables syncing HSI48 MHz to USB clock
  /* USER CODE END Init */

  /* Configure the system clock */
  //SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  //DWT->CTRL = 0x400003FF | (1ul<<17) | DWT_CTRL_LSUEVTENA_Msk | DWT_CTRL_FOLDEVTENA_Msk;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  //MX_GPIO_Init();
  //MX_DMA_Init();
  //MX_ADC1_Init();
  // MX_ADC3_Init();
  // MX_ADC4_Init();
  //MX_ADC5_Init();
  // //MX_HRTIM1_Init();
  // MX_OPAMP3_Init();
  // MX_OPAMP4_Init();
  // MX_OPAMP6_Init();
  //MX_SPI1_Init();
  //MX_SPI3_Init();
  //MX_TIM1_Init();
  //MX_TIM2_Init();
  //MX_TIM4_Init();
  //MX_USB_PCD_Init();
  //MX_I2C1_Init();
  //MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  //USB->CNTR &= ~(USB_CNTR_SOFM | USB_CNTR_ESOFM); // Don't need these interrupts
  //SPI1->CR1 |= SPI_CR1_SPE;
  //SPI3->CR1 |= SPI_CR1_SPE; // enable spi
  //SPI1->CR2 |= SPI_CR2_RXDMAEN | SPI_CR2_TXDMAEN;
  system_init();

  //HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);

  //HAL_ADC_Start(&hadc1);
  // HAL_ADC_Start(&hadc3);
  // HAL_ADC_Start(&hadc4);
  // HAL_ADC_Start(&hadc5);

  // HAL_OPAMP_Start(&hopamp3);
  // HAL_OPAMP_Start(&hopamp4);
  // OPAMP6->CSR |= OPAMP_CSR_OPAMPINTEN; // bug in cubemx
  // HAL_OPAMP_Start(&hopamp6);

  //HRTIM1->sCommonRegs.CR2 |= HRTIM_CR2_TESWU;
  //HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN + HRTIM_MCR_TECEN + HRTIM_MCR_TFCEN;

  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1);
  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD2);
  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_OUTPUT_TE2);
  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_OUTPUT_TF1);
  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_OUTPUT_TE1);
  // HAL_HRTIM_SimplePWMStart(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_OUTPUT_TF2);
  // hadc5.Instance->CR |= ADC_CR_JADSTART;
  // hadc5.Instance->IER |= ADC_IER_JEOCIE;
  // hadc4.Instance->CR |= ADC_CR_JADSTART;
  // hadc3.Instance->CR |= ADC_CR_JADSTART;
  //hadc1.Instance->CR |= ADC_CR_ADSTART;

  
  //HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
  //USB_DevConnect(USB);
  //htim1.Instance->DIER |= TIM_DIER_UIE;
  //HAL_TIM_Base_Start(&htim1);
//  HAL_TIM_Base_Start(&htim4);
  //HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_3); // encoder index
  // HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_1);
  // HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_2);
  // HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_3);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    system_run();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  HAL_Init();

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  RCC_CRSInitTypeDef pInit = {0};

  /** Configure the main internal regulator output voltage 
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
#ifdef USE_HSI
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
#else
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
#endif
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_8) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks 
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC12
                              |RCC_PERIPHCLK_ADC345;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
  PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configures CRS 
  */
  pInit.Prescaler = RCC_CRS_SYNC_DIV1;
  pInit.Source = RCC_CRS_SYNC_SOURCE_USB;
  pInit.Polarity = RCC_CRS_SYNC_POLARITY_RISING;
  pInit.ReloadValue = __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000,1000);
  pInit.ErrorLimitValue = 34;
  pInit.HSI48CalibrationValue = 32;

  HAL_RCCEx_CRSConfig(&pInit);
}

// /**
//   * @brief ADC1 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_ADC1_Init(void)
// {

//   /* USER CODE BEGIN ADC1_Init 0 */

//   /* USER CODE END ADC1_Init 0 */

//   ADC_MultiModeTypeDef multimode = {0};
//   ADC_ChannelConfTypeDef sConfig = {0};
//   ADC_InjectionConfTypeDef sConfigInjected = {0};

//   /* USER CODE BEGIN ADC1_Init 1 */

//   /* USER CODE END ADC1_Init 1 */
//   /** Common config 
//   */
//   hadc1.Instance = ADC1;
//   hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//   hadc1.Init.Resolution = ADC_RESOLUTION_12B;
//   hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//   hadc1.Init.GainCompensation = 0;
//   hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
//   hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//   hadc1.Init.LowPowerAutoWait = DISABLE;
//   hadc1.Init.ContinuousConvMode = DISABLE;
//   hadc1.Init.NbrOfConversion = 1;
//   hadc1.Init.DiscontinuousConvMode = DISABLE;
//   hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
//   hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
//   hadc1.Init.DMAContinuousRequests = DISABLE;
//   hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
//   hadc1.Init.OversamplingMode = DISABLE;
//   if (HAL_ADC_Init(&hadc1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure the ADC multi-mode 
//   */
//   multimode.Mode = ADC_MODE_INDEPENDENT;
//   if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Regular Channel 
//   */
//   sConfig.Channel = ADC_CHANNEL_13;
//   sConfig.Rank = ADC_REGULAR_RANK_1;
//   sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
//   sConfig.SingleDiff = ADC_SINGLE_ENDED;
//   sConfig.OffsetNumber = ADC_OFFSET_NONE;
//   sConfig.Offset = 0;
//   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_16;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
//   sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_640CYCLES_5;
//   sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
//   sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
//   sConfigInjected.InjectedOffset = 0;
//   sConfigInjected.InjectedNbrOfConversion = 3;
//   sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
//   sConfigInjected.AutoInjectedConv = DISABLE;
//   sConfigInjected.QueueInjectedContext = DISABLE;
//   sConfigInjected.ExternalTrigInjecConv = ADC_INJECTED_SOFTWARE_START;
//   sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_NONE;
//   sConfigInjected.InjecOversamplingMode = DISABLE;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_18;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_2;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_8;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_3;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc1, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN ADC1_Init 2 */

//   /* USER CODE END ADC1_Init 2 */

// }

// /**
//   * @brief ADC3 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_ADC3_Init(void)
// {

//   /* USER CODE BEGIN ADC3_Init 0 */

//   /* USER CODE END ADC3_Init 0 */

//   ADC_MultiModeTypeDef multimode = {0};
//   ADC_InjectionConfTypeDef sConfigInjected = {0};

//   /* USER CODE BEGIN ADC3_Init 1 */

//   /* USER CODE END ADC3_Init 1 */
//   /** Common config 
//   */
//   hadc3.Instance = ADC3;
//   hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//   hadc3.Init.Resolution = ADC_RESOLUTION_12B;
//   hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//   hadc3.Init.GainCompensation = 0;
//   hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
//   hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//   hadc3.Init.LowPowerAutoWait = DISABLE;
//   hadc3.Init.ContinuousConvMode = DISABLE;
//   hadc3.Init.NbrOfConversion = 1;
//   hadc3.Init.DiscontinuousConvMode = DISABLE;
//   hadc3.Init.DMAContinuousRequests = DISABLE;
//   hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//   hadc3.Init.OversamplingMode = DISABLE;
//   if (HAL_ADC_Init(&hadc3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure the ADC multi-mode 
//   */
//   multimode.Mode = ADC_MODE_INDEPENDENT;
//   if (HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_VOPAMP3_ADC3;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
//   sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_12CYCLES_5;
//   sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
//   sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
//   sConfigInjected.InjectedOffset = 0;
//   sConfigInjected.InjectedNbrOfConversion = 1;
//   sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
//   sConfigInjected.AutoInjectedConv = DISABLE;
//   sConfigInjected.QueueInjectedContext = DISABLE;
//   sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJEC_HRTIM_TRG1;
//   sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
//   sConfigInjected.InjecOversamplingMode = DISABLE;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc3, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN ADC3_Init 2 */

//   /* USER CODE END ADC3_Init 2 */

// }

// /**
//   * @brief ADC4 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_ADC4_Init(void)
// {

//   /* USER CODE BEGIN ADC4_Init 0 */

//   /* USER CODE END ADC4_Init 0 */

//   ADC_InjectionConfTypeDef sConfigInjected = {0};

//   /* USER CODE BEGIN ADC4_Init 1 */

//   /* USER CODE END ADC4_Init 1 */
//   /** Common config 
//   */
//   hadc4.Instance = ADC4;
//   hadc4.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//   hadc4.Init.Resolution = ADC_RESOLUTION_12B;
//   hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//   hadc4.Init.GainCompensation = 0;
//   hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
//   hadc4.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//   hadc4.Init.LowPowerAutoWait = DISABLE;
//   hadc4.Init.ContinuousConvMode = DISABLE;
//   hadc4.Init.NbrOfConversion = 1;
//   hadc4.Init.DiscontinuousConvMode = DISABLE;
//   hadc4.Init.DMAContinuousRequests = DISABLE;
//   hadc4.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//   hadc4.Init.OversamplingMode = DISABLE;
//   if (HAL_ADC_Init(&hadc4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_VOPAMP6;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
//   sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_12CYCLES_5;
//   sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
//   sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
//   sConfigInjected.InjectedOffset = 0;
//   sConfigInjected.InjectedNbrOfConversion = 1;
//   sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
//   sConfigInjected.AutoInjectedConv = DISABLE;
//   sConfigInjected.QueueInjectedContext = DISABLE;
//   sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJEC_HRTIM_TRG1;
//   sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
//   sConfigInjected.InjecOversamplingMode = DISABLE;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc4, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN ADC4_Init 2 */

//   /* USER CODE END ADC4_Init 2 */

// }

// /**
//   * @brief ADC5 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_ADC5_Init(void)
// {

//   /* USER CODE BEGIN ADC5_Init 0 */

//   /* USER CODE END ADC5_Init 0 */

//   ADC_InjectionConfTypeDef sConfigInjected = {0};

//   /* USER CODE BEGIN ADC5_Init 1 */

//   /* USER CODE END ADC5_Init 1 */
//   /** Common config 
//   */
//   hadc5.Instance = ADC5;
//   hadc5.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//   hadc5.Init.Resolution = ADC_RESOLUTION_12B;
//   hadc5.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//   hadc5.Init.GainCompensation = 0;
//   hadc5.Init.ScanConvMode = ADC_SCAN_DISABLE;
//   hadc5.Init.EOCSelection = ADC_EOC_SEQ_CONV;
//   hadc5.Init.LowPowerAutoWait = DISABLE;
//   hadc5.Init.ContinuousConvMode = DISABLE;
//   hadc5.Init.NbrOfConversion = 1;
//   hadc5.Init.DiscontinuousConvMode = DISABLE;
//   hadc5.Init.DMAContinuousRequests = DISABLE;
//   hadc5.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//   hadc5.Init.OversamplingMode = DISABLE;
//   if (HAL_ADC_Init(&hadc5) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Injected Channel 
//   */
//   sConfigInjected.InjectedChannel = ADC_CHANNEL_VOPAMP4;
//   sConfigInjected.InjectedRank = ADC_INJECTED_RANK_1;
//   sConfigInjected.InjectedSamplingTime = ADC_SAMPLETIME_12CYCLES_5;
//   sConfigInjected.InjectedSingleDiff = ADC_SINGLE_ENDED;
//   sConfigInjected.InjectedOffsetNumber = ADC_OFFSET_NONE;
//   sConfigInjected.InjectedOffset = 0;
//   sConfigInjected.InjectedNbrOfConversion = 1;
//   sConfigInjected.InjectedDiscontinuousConvMode = DISABLE;
//   sConfigInjected.AutoInjectedConv = DISABLE;
//   sConfigInjected.QueueInjectedContext = DISABLE;
//   sConfigInjected.ExternalTrigInjecConv = ADC_EXTERNALTRIGINJEC_HRTIM_TRG1;
//   sConfigInjected.ExternalTrigInjecConvEdge = ADC_EXTERNALTRIGINJECCONV_EDGE_RISING;
//   sConfigInjected.InjecOversamplingMode = DISABLE;
//   if (HAL_ADCEx_InjectedConfigChannel(&hadc5, &sConfigInjected) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN ADC5_Init 2 */

//   /* USER CODE END ADC5_Init 2 */

// }

// /**
//   * @brief HRTIM1 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_HRTIM1_Init(void)
// {

//   /* USER CODE BEGIN HRTIM1_Init 0 */

//   /* USER CODE END HRTIM1_Init 0 */

//   HRTIM_ADCTriggerCfgTypeDef pADCTriggerCfg = {0};
//   HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg = {0};
//   HRTIM_TimerCtlTypeDef pTimerCtl = {0};
//   HRTIM_SimpleOCChannelCfgTypeDef pSimpleOCChannelCfg = {0};

//   /* USER CODE BEGIN HRTIM1_Init 1 */

//   /* USER CODE END HRTIM1_Init 1 */
//   hhrtim1.Instance = HRTIM1;
//   hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
//   hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
//   if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_DLLCalibrationStart(&hhrtim1, HRTIM_CALIBRATIONRATE_3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_PollForDLLCalibration(&hhrtim1, 10) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pADCTriggerCfg.UpdateSource = HRTIM_ADCTRIGGERUPDATE_TIMER_F;
//   pADCTriggerCfg.Trigger = HRTIM_ADCTRIGGEREVENT13_TIMERF_PERIOD;
//   if (HAL_HRTIM_ADCTriggerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, &pADCTriggerCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, 0x0) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pTimeBaseCfg.Period = hrperiod;
//   pTimeBaseCfg.RepetitionCounter = 0x00;
//   pTimeBaseCfg.PrescalerRatio = HRTIM_PRESCALERRATIO_MUL32;
//   pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
//   if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimeBaseCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pTimerCtl.UpDownMode = HRTIM_TIMERUPDOWNMODE_UPDOWN;
//   pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;

//   if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, &pTimerCtl) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_RollOverModeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_TIM_FEROM_BOTH|HRTIM_TIM_BMROM_BOTH
//                               |HRTIM_TIM_ADROM_BOTH|HRTIM_TIM_OUTROM_BOTH
//                               |HRTIM_TIM_ROM_BOTH) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pSimpleOCChannelCfg.Mode = HRTIM_BASICOCMODE_ACTIVE;
//   pSimpleOCChannelCfg.Pulse = 0;
//   pSimpleOCChannelCfg.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
//   pSimpleOCChannelCfg.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
//   if (HAL_HRTIM_SimpleOCChannelConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_D, HRTIM_OUTPUT_TD1, &pSimpleOCChannelCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, &pTimeBaseCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;

//   if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, &pTimerCtl) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_RollOverModeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_TIM_FEROM_BOTH|HRTIM_TIM_BMROM_BOTH
//                               |HRTIM_TIM_ADROM_BOTH|HRTIM_TIM_OUTROM_BOTH
//                               |HRTIM_TIM_ROM_BOTH) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_SimpleOCChannelConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_E, HRTIM_OUTPUT_TE1, &pSimpleOCChannelCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimeBaseCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;

//   if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, &pTimerCtl) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_RollOverModeConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_TIM_FEROM_BOTH|HRTIM_TIM_BMROM_BOTH
//                               |HRTIM_TIM_ADROM_VALLEY|HRTIM_TIM_OUTROM_BOTH
//                               |HRTIM_TIM_ROM_BOTH) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   if (HAL_HRTIM_SimpleOCChannelConfig(&hhrtim1, HRTIM_TIMERINDEX_TIMER_F, HRTIM_OUTPUT_TF1, &pSimpleOCChannelCfg) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN HRTIM1_Init 2 */

//   /* USER CODE END HRTIM1_Init 2 */
//   HAL_HRTIM_MspPostInit(&hhrtim1);

// }

// /**
//   * @brief I2C1 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_I2C1_Init(void)
// {

//   /* USER CODE BEGIN I2C1_Init 0 */

//   /* USER CODE END I2C1_Init 0 */

//   /* USER CODE BEGIN I2C1_Init 1 */

//   /* USER CODE END I2C1_Init 1 */
//   hi2c1.Instance = I2C1;
//   hi2c1.Init.Timing = 0x30A0A7FB;
//   hi2c1.Init.OwnAddress1 = 0;
//   hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c1.Init.OwnAddress2 = 0;
//   hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//   hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Analogue filter 
//   */
//   if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Digital filter 
//   */
//   if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN I2C1_Init 2 */

//   /* USER CODE END I2C1_Init 2 */

// }

// /**
//   * @brief I2C2 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_I2C2_Init(void)
// {

//   /* USER CODE BEGIN I2C2_Init 0 */

//   /* USER CODE END I2C2_Init 0 */

//   /* USER CODE BEGIN I2C2_Init 1 */

//   /* USER CODE END I2C2_Init 1 */
//   hi2c2.Instance = I2C2;
//   hi2c2.Init.Timing = 0x30A0A7FB;
//   hi2c2.Init.OwnAddress1 = 0;
//   hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//   hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//   hi2c2.Init.OwnAddress2 = 0;
//   hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//   hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//   hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//   if (HAL_I2C_Init(&hi2c2) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Analogue filter 
//   */
//   if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /** Configure Digital filter 
//   */
//   if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN I2C2_Init 2 */

//   /* USER CODE END I2C2_Init 2 */

// }

// /**
//   * @brief OPAMP3 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_OPAMP3_Init(void)
// {

//   /* USER CODE BEGIN OPAMP3_Init 0 */

//   /* USER CODE END OPAMP3_Init 0 */

//   /* USER CODE BEGIN OPAMP3_Init 1 */

//   /* USER CODE END OPAMP3_Init 1 */
//   hopamp3.Instance = OPAMP3;
//   hopamp3.Init.PowerMode = OPAMP_POWERMODE_HIGHSPEED;
//   hopamp3.Init.Mode = OPAMP_FOLLOWER_MODE;
//   hopamp3.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO1;
//   hopamp3.Init.InternalOutput = ENABLE;
//   hopamp3.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
//   hopamp3.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
//   if (HAL_OPAMP_Init(&hopamp3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN OPAMP3_Init 2 */

//   /* USER CODE END OPAMP3_Init 2 */

// }

// /**
//   * @brief OPAMP4 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_OPAMP4_Init(void)
// {

//   /* USER CODE BEGIN OPAMP4_Init 0 */

//   /* USER CODE END OPAMP4_Init 0 */

//   /* USER CODE BEGIN OPAMP4_Init 1 */

//   /* USER CODE END OPAMP4_Init 1 */
//   hopamp4.Instance = OPAMP4;
//   hopamp4.Init.PowerMode = OPAMP_POWERMODE_HIGHSPEED;
//   hopamp4.Init.Mode = OPAMP_FOLLOWER_MODE;
//   hopamp4.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO2;
//   hopamp4.Init.InternalOutput = ENABLE;
//   hopamp4.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
//   hopamp4.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
//   if (HAL_OPAMP_Init(&hopamp4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN OPAMP4_Init 2 */

//   /* USER CODE END OPAMP4_Init 2 */

// }

// /**
//   * @brief OPAMP6 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_OPAMP6_Init(void)
// {

//   /* USER CODE BEGIN OPAMP6_Init 0 */

//   /* USER CODE END OPAMP6_Init 0 */

//   /* USER CODE BEGIN OPAMP6_Init 1 */

//   /* USER CODE END OPAMP6_Init 1 */
//   hopamp6.Instance = OPAMP6;
//   hopamp6.Init.PowerMode = OPAMP_POWERMODE_HIGHSPEED;
//   hopamp6.Init.Mode = OPAMP_FOLLOWER_MODE;
//   hopamp6.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
//   hopamp6.Init.InternalOutput = DISABLE;
//   hopamp6.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
//   hopamp6.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
//   if (HAL_OPAMP_Init(&hopamp6) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN OPAMP6_Init 2 */

//   /* USER CODE END OPAMP6_Init 2 */

// }

// /**
//   * @brief SPI1 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_SPI1_Init(void)
// {

//   /* USER CODE BEGIN SPI1_Init 0 */

//   /* USER CODE END SPI1_Init 0 */

//   /* USER CODE BEGIN SPI1_Init 1 */

//   /* USER CODE END SPI1_Init 1 */
//   /* SPI1 parameter configuration*/
//   hspi1.Instance = SPI1;
//   hspi1.Init.Mode = SPI_MODE_MASTER;
//   hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//   hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//   hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//   hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
//   hspi1.Init.NSS = SPI_NSS_SOFT;
//   hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//   hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//   hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//   hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//   hspi1.Init.CRCPolynomial = 7;
//   hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//   hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
//   if (HAL_SPI_Init(&hspi1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN SPI1_Init 2 */

//   /* USER CODE END SPI1_Init 2 */

// }

// /**
//   * @brief SPI3 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_SPI3_Init(void)
// {

//   /* USER CODE BEGIN SPI3_Init 0 */

//   /* USER CODE END SPI3_Init 0 */

//   /* USER CODE BEGIN SPI3_Init 1 */

//   /* USER CODE END SPI3_Init 1 */
//   /* SPI3 parameter configuration*/
//   hspi3.Instance = SPI3;
//   hspi3.Init.Mode = SPI_MODE_MASTER;
//   hspi3.Init.Direction = SPI_DIRECTION_2LINES;
//   hspi3.Init.DataSize = SPI_DATASIZE_16BIT;
//   hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
//   hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
//   hspi3.Init.NSS = SPI_NSS_SOFT;
//   hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//   hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
//   hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
//   hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//   hspi3.Init.CRCPolynomial = 7;
//   hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//   hspi3.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//   if (HAL_SPI_Init(&hspi3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN SPI3_Init 2 */

//   /* USER CODE END SPI3_Init 2 */

// }

// /**
//   * @brief TIM1 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_TIM1_Init(void)
// {

//   /* USER CODE BEGIN TIM1_Init 0 */

//   /* USER CODE END TIM1_Init 0 */

//   TIM_ClockConfigTypeDef sClockSourceConfig = {0};
//   TIM_MasterConfigTypeDef sMasterConfig = {0};
//   TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

//   /* USER CODE BEGIN TIM1_Init 1 */

//   /* USER CODE END TIM1_Init 1 */
//   htim1.Instance = TIM1;
//   htim1.Init.Prescaler = 0;
//   htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
//   htim1.Init.Period = main_loop_period-1;
//   htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//   htim1.Init.RepetitionCounter = 0;
//   htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//   if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
//   if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//   sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
//   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//   if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
//   sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
//   if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN TIM1_Init 2 */

//   /* USER CODE END TIM1_Init 2 */

// }

// /**
//   * @brief TIM2 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_TIM2_Init(void)
// {

//   /* USER CODE BEGIN TIM2_Init 0 */

//   /* USER CODE END TIM2_Init 0 */

//   TIM_Encoder_InitTypeDef sConfig = {0};
//   TIM_MasterConfigTypeDef sMasterConfig = {0};
//   TIM_IC_InitTypeDef sConfigIC = {0};

//   /* USER CODE BEGIN TIM2_Init 1 */

//   /* USER CODE END TIM2_Init 1 */
//   htim2.Instance = TIM2;
//   htim2.Init.Prescaler = 0;
//   htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
//   htim2.Init.Period = 0xffffffff;
//   htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//   htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//   if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
//   sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
//   sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
//   sConfig.IC1Filter = 1;
//   sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
//   sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
//   sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
//   sConfig.IC2Filter = 1;
//   if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//   if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
//   sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
//   sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
//   sConfigIC.ICFilter = 2;
//   if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN TIM2_Init 2 */

//   /* USER CODE END TIM2_Init 2 */

// }

// /**
//   * @brief TIM4 Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_TIM4_Init(void)
// {

//   /* USER CODE BEGIN TIM4_Init 0 */

//   /* USER CODE END TIM4_Init 0 */

//   TIM_MasterConfigTypeDef sMasterConfig = {0};
//   TIM_OC_InitTypeDef sConfigOC = {0};

//   /* USER CODE BEGIN TIM4_Init 1 */

//   /* USER CODE END TIM4_Init 1 */
//   htim4.Instance = TIM4;
//   htim4.Init.Prescaler = 0;
//   htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
//   htim4.Init.Period = 65535;
//   htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//   htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
//   if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
//   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//   if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfigOC.OCMode = TIM_OCMODE_PWM1;
//   sConfigOC.Pulse = 0;
//   sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//   sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
//   if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfigOC.Pulse = 0;
//   if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   sConfigOC.Pulse = 0;
//   if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN TIM4_Init 2 */

//   /* USER CODE END TIM4_Init 2 */
//   HAL_TIM_MspPostInit(&htim4);

// }

// /**
//   * @brief USB Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_USB_PCD_Init(void)
// {

//   /* USER CODE BEGIN USB_Init 0 */

//   /* USER CODE END USB_Init 0 */

//   /* USER CODE BEGIN USB_Init 1 */

//   /* USER CODE END USB_Init 1 */
//   hpcd_USB_FS.Instance = USB;
//   hpcd_USB_FS.Init.dev_endpoints = 8;
//   hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
//   hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
//   hpcd_USB_FS.Init.Sof_enable = DISABLE;
//   hpcd_USB_FS.Init.low_power_enable = DISABLE;
//   hpcd_USB_FS.Init.lpm_enable = DISABLE;
//   hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
//   if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN USB_Init 2 */

//   /* USER CODE END USB_Init 2 */

// }

// /** 
//   * Enable DMA controller clock
//   */
// static void MX_DMA_Init(void) 
// {

//   /* DMA controller clock enable */
//   __HAL_RCC_DMAMUX1_CLK_ENABLE();
//   __HAL_RCC_DMA1_CLK_ENABLE();

//   /* DMA interrupt init */
//   /* DMA1_Channel1_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
//   /* DMA1_Channel2_IRQn interrupt configuration */
//   HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

// }

// /**
//   * @brief GPIO Initialization Function
//   * @param None
//   * @retval None
//   */
// static void MX_GPIO_Init(void)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};

//   // /* GPIO Ports Clock Enable */
//   // __HAL_RCC_GPIOC_CLK_ENABLE();
//   // __HAL_RCC_GPIOF_CLK_ENABLE();
//   // __HAL_RCC_GPIOA_CLK_ENABLE();
//   // __HAL_RCC_GPIOB_CLK_ENABLE();
//   // __HAL_RCC_GPIOD_CLK_ENABLE();

//   // /*Configure GPIO pin Output Level */
//   // HAL_GPIO_WritePin(drv_en_GPIO_Port, drv_en_Pin, GPIO_PIN_SET);

//   // /*Configure GPIO pin Output Level */
//   // HAL_GPIO_WritePin(spi1cs_GPIO_Port, spi1cs_Pin, GPIO_PIN_RESET);

//   // /*Configure GPIO pin Output Level */
//   // HAL_GPIO_WritePin(scope3_GPIO_Port, scope3_Pin, GPIO_PIN_RESET);

//   // /*Configure GPIO pin Output Level */
//   // HAL_GPIO_WritePin(spi3cs1_GPIO_Port, spi3cs1_Pin, GPIO_PIN_RESET);

//   // /*Configure GPIO pin Output Level */
//   // HAL_GPIO_WritePin(GPIOB, spi3cs2_Pin|spi3cs3_Pin, GPIO_PIN_RESET);

//   // /*Configure GPIO pin : drv_en_Pin */
//   // GPIO_InitStruct.Pin = drv_en_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   // HAL_GPIO_Init(drv_en_GPIO_Port, &GPIO_InitStruct);

//   // /*Configure GPIO pin : drv_fault_Pin */
//   // GPIO_InitStruct.Pin = drv_fault_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // HAL_GPIO_Init(drv_fault_GPIO_Port, &GPIO_InitStruct);

//   // /*Configure GPIO pin : spi1cs_Pin */
//   // GPIO_InitStruct.Pin = spi1cs_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   // HAL_GPIO_Init(spi1cs_GPIO_Port, &GPIO_InitStruct);

//   // /*Configure GPIO pin : scope3_Pin */
//   // GPIO_InitStruct.Pin = scope3_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   // HAL_GPIO_Init(scope3_GPIO_Port, &GPIO_InitStruct);

//   /*Configure GPIO pin : PA10 */
//   GPIO_InitStruct.Pin = GPIO_PIN_10;
//   GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//   // /*Configure GPIO pin : spi3cs1_Pin */
//   // GPIO_InitStruct.Pin = spi3cs1_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   // HAL_GPIO_Init(spi3cs1_GPIO_Port, &GPIO_InitStruct);

//   // /*Configure GPIO pins : spi3cs2_Pin spi3cs3_Pin */
//   // GPIO_InitStruct.Pin = spi3cs2_Pin|spi3cs3_Pin;
//   // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   // GPIO_InitStruct.Pull = GPIO_NOPULL;
//   // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   // HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// }

// /* USER CODE BEGIN 4 */

// /* USER CODE END 4 */

// /**
//   * @brief  This function is executed in case of error occurrence.
//   * @retval None
//   */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1);
  /* USER CODE END Error_Handler_Debug */
}

// #ifdef  USE_FULL_ASSERT
// /**
//   * @brief  Reports the name of the source file and the source line number
//   *         where the assert_param error has occurred.
//   * @param  file: pointer to the source file name
//   * @param  line: assert_param error line source number
//   * @retval None
//   */
// void assert_failed(uint8_t *file, uint32_t line)
// { 
//   /* USER CODE BEGIN 6 */
//   /* User can add his own implementation to report the file name and line number,
//      tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
//   /* USER CODE END 6 */
// }
// #endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
