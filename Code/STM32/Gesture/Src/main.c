/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
/*	Interface definition
		SCL	=	PB8
		SDA = PB9
*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "DEV_Config.h"
#include "PAJ7620U2.h"
unsigned char PAJ7620U2_init()
{
	unsigned char i,State;
	DEV_Set_I2CAddress(PAJ7620U2_I2C_ADDRESS);
	DEV_Delay_ms(5);
	State = DEV_I2C_ReadByte(0x00);												//Read State
	if (State != 0x20) 
		return 0;																						//Wake up failed
	DEV_I2C_WriteByte(PAJ_BANK_SELECT, 0);								//Select Bank 0
	for (i=0;i< Init_Array;i++)
	{
		 DEV_I2C_WriteByte(Init_Register_Array[i][0], Init_Register_Array[i][1]);//Power up initialize
	}
	return 1;
}
void SystemClock_Config(void);
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
  unsigned char i;
	unsigned short  Gesture_Data;

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
  MX_I2C1_Init();
	printf("Gesture Sensor Test Program ...\r\n");
	if(!PAJ7620U2_init())
	{	printf("\nGesture Sensor Error\r\n");
		return 0;
	}
		printf("\nGesture Sensor OK\r\n");
	DEV_I2C_WriteByte(PAJ_BANK_SELECT, 0);																	//Select Bank 0
	for (i = 0; i < Gesture_Array_SIZE; i++)
	{
		DEV_I2C_WriteByte(Init_Gesture_Array[i][0], Init_Gesture_Array[i][1]);//Gesture register initializes
	}
  while (1)
  {
		Gesture_Data = DEV_I2C_ReadWord(PAJ_INT_FLAG1);
		if (Gesture_Data)
		{
			switch (Gesture_Data)
			{
				case PAJ_UP:			  			  printf("Up\r\n");				break;
				case PAJ_DOWN:							printf("Down\r\n");				break;
				case PAJ_LEFT:							printf("Left\r\n");				break;
				case PAJ_RIGHT:							printf("Right\r\n"); 			break;
				case PAJ_FORWARD:						printf("Forward\r\n");			break;
				case PAJ_BACKWARD:					printf("Backward\r\n"); 		break;
				case PAJ_CLOCKWISE:					printf("Clockwise\r\n"); 		break;
				case PAJ_COUNT_CLOCKWISE:		printf("AntiClockwise\r\n"); 	break;
				case PAJ_WAVE:							printf("Wave\r\n"); 			break;
				default: break;
			}
			Gesture_Data=0;
			DEV_Delay_ms(50);
		}
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
