/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
char ESP_INIT(void);
char ESP_Check_AT(void);
char ESP_AT_Send(char *Cmd, char CmdSize, char *rData, char RetSize, char trial);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
/* USER CODE BEGIN PV */
char Uart_RxData[255];
char Uart_TxData[255];
char returnData[25];
char Buffer[255];
char Rx_Cnt=0;
char Rx_Flag=0;
char Rx_Message=0;
char *ptr;
char cnt=0;
char ret;
char button;
int adc_buffer[1];
int adc;
char led1,led2;
unsigned short counter=0;

#define CMD_TRIAL	5

const char ESP_AT_CMD[] = "\r\nAT\r\n\0";
const char ESP_AT_RET[] = "\r\nOK\r\n\0";
const char ESP_MODE_CMD[] = "AT+CWMODE=2\r\n\0";
const char ESP_WIFI_SET_CMD[] = "AT+CWSAP=\"ESP\",\"0123456789\",5,0\r\n\0";
const char ESP_WIFI_IP_CMD[] = "\r\nAT+CIPAP=\"192.168.1.57\",\"192.168.1.57\",\"255.255.255.0\"\r\n\0";
const char ESP_TRSP_CMD[] = "AT+CIPMODE=1\r\n\0";
const char ESP_UDP_CMD[] = "AT+CIPSTART=\"UDP\",\"192.168.1.2\",1001,1001,0\r\n\0";
const char ESP_UDP_RET[] = "\r\nCONNECT\r\n\0";
const char ESP_SEND_CMD[] = "AT+CIPSEND=\r\n\0";
const char ESP_SEND_RET[] = "\r\n>\0";
const char ESP_SEND_OK[] = "SEND OK\r\n\0";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC_Init(void);
static void MX_USART3_UART_Init(void);
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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);

    HAL_ADC_Start_DMA(&hadc, (uint32_t *)adc_buffer, 1);

  start:
    if (ESP_INIT()) goto start;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  	  button = HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13);
	  	  led1    = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6);
	  	  led2    = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);
	  	  adc = adc_buffer[0];

	  	  if(counter>=2000)
	  	  {
	  		  counter = 0;

	  		  ret = ESP_Check_AT();
	  		  if(ret==1) goto start;

	  		  sprintf(Buffer,"MSG:%d ADC:%d BTN:%d LED1:%d LED2:%d\r\n\0",cnt++,adc,button,led1,led2);

	  		  sprintf(Uart_TxData,"AT+CIPSEND=%d\r\n",strlen(Buffer));
	  		  ret = ESP_AT_Send(Uart_TxData, strlen(Uart_TxData), ESP_SEND_RET, sizeof(ESP_SEND_RET), CMD_TRIAL);
	  		  if(ret==1) goto start;

	  		  ret = ESP_AT_Send(Buffer, strlen(Buffer), ESP_SEND_OK, sizeof(ESP_SEND_OK), CMD_TRIAL);

	  		  memset(Uart_RxData,0x00,sizeof(Uart_RxData));
	  	  }

	  	  if(Rx_Message)
	  	  {
	  		  HAL_Delay(50);
	  		  Rx_Cnt = 0;
	  		  Rx_Message = 0;

	  		  if(strstr(Uart_RxData,"LED1 ON") != 0)
	  		  {
	  			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);		// Red LED ON
	  		  }
	  		  if(strstr(Uart_RxData,"LED1 OFF") != 0)
	  		  {
	  			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	// Red LED OFF
	  		  }
	  		  if(strstr(Uart_RxData,"LED2 ON") != 0)
	  		  {
	  			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);		// Green LED ON
	  		  }
	  		  if(strstr(Uart_RxData,"LED2 OFF") != 0)
	  		  {
	  			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	// Green LED OFF
	  		  }

	  		  memset(Uart_RxData,0x00,sizeof(Uart_RxData));

	  	  }


	  	  HAL_Delay(0);
	  	  counter++;
	    }
    /* USER CODE BEGIN 3 */

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ENABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  /* USER CODE END ADC_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Ch1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Ch1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Ch1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|External_Led_Pin|Rst_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Button_Pin */
  GPIO_InitStruct.Pin = Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin External_Led_Pin Rst_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|External_Led_Pin|Rst_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
char ESP_INIT(void)
{
	char ret;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_Delay(500);

	ret = ESP_AT_Send(ESP_AT_CMD, sizeof(ESP_AT_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;
	ret = ESP_AT_Send(ESP_AT_CMD, sizeof(ESP_AT_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;
	HAL_Delay(200);
	ret = ESP_AT_Send(ESP_MODE_CMD, sizeof(ESP_MODE_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;
	HAL_Delay(200);
	ret = ESP_AT_Send(ESP_WIFI_SET_CMD, sizeof(ESP_WIFI_SET_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;
	HAL_Delay(200);
	ret = ESP_AT_Send(ESP_WIFI_IP_CMD, sizeof(ESP_WIFI_IP_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;
	HAL_Delay(200);
	ret = ESP_AT_Send(ESP_UDP_CMD, sizeof(ESP_UDP_CMD), ESP_UDP_RET, sizeof(ESP_UDP_RET), CMD_TRIAL);
	if(ret==1) return 1;
	HAL_Delay(200);



	memset(Uart_RxData,0x00,sizeof(Uart_RxData));

	return 0;
}

char ESP_Check_AT(void)
{
	char ret;

	ret = ESP_AT_Send(ESP_AT_CMD, sizeof(ESP_AT_CMD), ESP_AT_RET, sizeof(ESP_AT_RET), CMD_TRIAL);
	if(ret==1) return 1;

	return 0;
}

char ESP_AT_Send(char *Cmd, char CmdSize, char *rData, char RetSize, char trial)
{
    char cnt=0;

	memset(returnData,0x00,sizeof(returnData));
	memcpy(returnData,rData,RetSize);

    while(cnt<trial)
    {

    	Rx_Cnt = 0;
        memset(Uart_RxData,0x00,sizeof(Uart_RxData));
        HAL_UART_Transmit(&huart3, (uint8_t*)Cmd, CmdSize, 50);
        HAL_Delay(50);
        if(Rx_Flag)
        {
        	Rx_Flag = 0;
        	return 0;
        }
    	cnt++;
    }

    return 1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
