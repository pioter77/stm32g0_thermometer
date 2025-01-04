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
#include "dma.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "fonts.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, 3);

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  ssd1306_Init(I2C1);
  //SSD1306_ON(I2C1);
  LL_mDelay(100);

  //SSD1306_Clear (I2C1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //1st pixel for oled is at 28,24 position
	  LL_GPIO_TogglePin(LED_OUT_GPIO_Port, LED_OUT_Pin);

//	  ssd1306_Fill(White);
//	  ssd1306_SetCursor(28, 12+8);
//	  SSD1306_DrawLine(56, 15, 60, 15, White);
//	  SSD1306_DrawFilledRectangle(28, 24, 10, 10, White);
	  const char buf1[] = "I:+24.5*C";
	  const char buf2[] = "O:-12.9*C";
	  const char buf3[] = "Temps";
//	  SSD1306_DrawCircle(28+72/2, 14+20+10, 10, White);

//	  ssd1306_DrawText(buf1, Font6x8, 28, 44, White);
	  for(uint8_t i=0; i<5; i++)
	  {
//		  ssd1306_DrawText(buf1, Font6x8, 28, 16+8+(10*i), White);
		//  ssd1306_DrawText(buf1, Font6x8, 28, 16+8+(8*i), White);
	  }
//	  ssd1306_DrawPixel(28, 24, White);
//	  SSD1306_DrawLine(28, 24, 28, 43, White);
//	  SSD1306_DrawLine(30, 44, 30, 63, White);

//	  ssd1306_DrawChar('D', Font6x8, 28, 40, White);
#define LAYOUT_2
#ifdef LAYOUT_1
	  //1st layout example:
	  ssd1306_DrawText(buf1, Font7x13, 28, 24, White);
	  ssd1306_DrawText(buf2, Font7x13, 28, 38, White);
	  ssd1306_DrawText(buf3, Font6x8, 45, 52, White);
	  SSD1306_DrawFilledRectangle(58, 61, 8, 3, White);
	  SSD1306_DrawLine(30, 62, 96, 62, White);
#endif

#ifdef LAYOUT_2
	  ssd1306_DrawText(buf3, Font6x8, 45, 24, White);
	  SSD1306_DrawFilledRectangle(58, 35, 8, 3, White);
	  SSD1306_DrawLine(30, 36, 96, 36, White);
	  ssd1306_DrawText(buf1, Font7x13, 28, 41, White);
	  ssd1306_DrawText(buf2, Font7x13, 28, 54, White);


#endif
	  ssd1306_UpdateScreen(I2C1);
	  LL_mDelay(2000);

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
  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 8, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_4);

  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_Init1msTick(16000000);

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(16000000);
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
