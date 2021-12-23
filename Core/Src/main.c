/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define ESP_Buffer_Boyutu 5000
#define ESP_Buffer_Boyutu1 5000
/* USER CODE END Includes */
#define ESP1ID = 192.168.1.72
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


char *renk  ="<select name='renk'>"
				"<option value='beyaz'>Beyaz</option>"
				"<option value='kirmizi'>Kirmizi</option>"
				"<option value='siyah'>Siyah</option>"
				"</select>";
char *Varlik = "'<input type='checkbox' name='araba_var' value='araba'> Arabam var. <br />";
char *Varlik1 ="'<input type='checkbox' name='ev' value='ev'> evim var. <br />";
char *LED_ON = "<br><a href=\" ?pin=on\"><button type='button'>ON</button></a>";
char *LED_OFF = "<br><a href=\" ?pin=off\"><button type='button'>OFF</button></a>";
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
static uint8_t Case = 0;
char datatosend[2000] = {0};
char TX_Buffer[500];

char RX_Buffer[100];
char RX_Buffer1[100];
uint16_t ESP_Buffer_index=0;
char ID[] = "Luhmann";
char PW[] = "Luhmann2995@";
uint16_t Sayac = 0;
int baglanti_sayaci = 0;
int len = 0;
char ESP_Gelen_Data1[ESP_Buffer_Boyutu1];
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
char Luhmann[4] = "FF\r\n";
char TxBuffer1[4]="AT\r\n";
char TxBuffer2[12]="AT+CWMODE?\r\n";
char TxBuffer3[13]="AT+CWMODE=1\r\n";
char TxBuffer4[10]="AT+CIFSR\r\n";
char TxBuffer5[13]="AT+CIPMUX=1\r\n";
char TxBuffer6[21]="AT+CIPSERVER=1,3333\r\n";
char TxBuffer7[39]="AT+CIPSTART=\"TCP\",\"192.168.1.72\",3333\r\n";
char TxBuffer8[14]="AT+CIPSEND=4\r\n";
char TxBuffer9[35]= "AT+CWJAP=\"Luhmann\",\"Luhmann2995@\"\r\n";

char ESP_Gelen_Data[ESP_Buffer_Boyutu];
uint8_t RxBufferCnt=0;
uint8_t RxBufferCnt1=0;
/*UART2*/
uint8_t RxData=0;
uint8_t RxData1 =0;
uint16_t count=0;
uint16_t count1;
uint8_t pData[2];
/* USER CODE END PM */
/*UART2*/

uint16_t count1;
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void DataConnectionControl(char *SSID, char *PW);
void ESP_Gelen_Data_Clear();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart2)
	{
		ESP_Gelen_Data[RxBufferCnt++]= RxData;
		if(RxBufferCnt>254)
			RxBufferCnt=0;

		HAL_UART_Receive_IT(&huart2,&RxData,1);
	}
	else if(huart == &huart1)
	{

		ESP_Gelen_Data1[RxBufferCnt1++]= RxData1;
		if(RxBufferCnt1>254)
			RxBufferCnt1=0;

		HAL_UART_Receive_IT(&huart1,&RxData1,1);
	}

}
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
	MX_USART2_UART_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1,&RxData1,1);
	HAL_UART_Receive_IT(&huart2,&RxData,1);
	/* USER CODE END 2 */
	//HAL_UART_Transmit(&huart1,(uint8_t*)TX_Buffer, sprintf(TX_Buffer,"AT\r\n"), 1000);
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		DataConnectionControl(ID,PW);
		HAL_Delay(1);

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
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void)
{

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA5 PA6 */
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void DataConnectionControl(char *SSID,char *PW)
{

	switch(Case){
	case 0:
		HAL_UART_Transmit(&huart2,(uint8_t*)TxBuffer1, 4, 1000);
		Case = 1;
		HAL_Delay(1000);
		HAL_Delay(1);
		break;
	case 1:
		if(strstr(ESP_Gelen_Data,"OK"))
		{
			ESP_Gelen_Data_Clear();
			Case =2;
		}
		else
		{
			Case=0;
		}
		break;
	case 2:
		HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer2,12, 1000);
		Case = 3;
		HAL_Delay(1000);
		break;
	case 3:
		if(strstr(ESP_Gelen_Data,"+CWMODE:1"))
		{
			ESP_Gelen_Data_Clear();
			Case= 4;
		}
		else

		{
			ESP_Gelen_Data_Clear();
			HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer3,13, 100);
			Case = 3;

		}
		break;
		case 4:
		HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer9, 35, 100);
		Case = 5;
		HAL_Delay(5000);
		break;

	case 5:
		if(strstr(ESP_Gelen_Data,"OK"))
		{
			ESP_Gelen_Data_Clear();
			Case = 6;
		}
		else
		{
			ESP_Gelen_Data_Clear();
			Case = 6;

		}
			break;

	case 6:
		HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer4, 10, 100);
		HAL_Delay(1000);
		Case=7;
		break;
	case 7:
		if(strstr(ESP_Gelen_Data,"OK"))
		{
			ESP_Gelen_Data_Clear();
			Case =8;
		}
		else
		 {
			ESP_Gelen_Data_Clear();
			Case=8;
		  }
        break;
	case 8:
		HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer5, 13, 100);
		HAL_Delay(1000);
		Case =9;
		break;
	case 9:
		if(strstr(ESP_Gelen_Data,"OK"))
		{
			ESP_Gelen_Data_Clear();
			Case =10;
		}
		else
		{
			ESP_Gelen_Data_Clear();
			Case =10;
		}
		break;
	case 10:
		HAL_UART_Transmit(&huart2, (uint8_t*)TxBuffer6, 21,100);
		HAL_Delay(1000);
		HAL_Delay(1);
	    Case =11;
	case 11:
		if(strstr(ESP_Gelen_Data,"OK"))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			ESP_Gelen_Data_Clear();
			Case =12;
		}
		else
		{
			ESP_Gelen_Data_Clear();
			Case =12;
		}
		break;

	case 12:

	    HAL_UART_Transmit(&huart1, (uint8_t*)TxBuffer7, 39,100);
	    HAL_Delay(2000);
		HAL_Delay(1);
	    Case = 13;
	case 13:

		HAL_UART_Transmit(&huart1, (uint8_t*)TxBuffer8, 14, 100);
		HAL_Delay(2000);
	    Case=14;
	    break;
	case 14:
		 HAL_UART_Transmit(&huart1, (uint8_t*)Luhmann, 4, 100);
		 HAL_Delay(2000);
		 HAL_Delay(1);
		 Case=15;
		break;
	case 15:
		if(strstr(ESP_Gelen_Data,"FF"))
		{
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_Delay(5000);
			HAL_Delay(1);
			HAL_UART_Transmit(&huart2, (uint8_t*)TX_Buffer, sprintf(TX_Buffer, "AT+CIPCLOSE=0\r\n"), 100);
			ESP_Gelen_Data_Clear();
		    Case = 12;
		}
		break;
	}

}

void ESP_Gelen_Data_Clear()
	{
		uint16_t xxx = 0;

		for(xxx=0; xxx < ESP_Buffer_Boyutu; xxx++)
		{
			ESP_Gelen_Data[xxx] = 0;
			ESP_Gelen_Data1[xxx] = 0;
		}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
