/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled_ui.h"
#include "dht11.h"
#include "dwt_delay.h"
#include "ec11.h"



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
static  ComfortLevel App_CalcComfort(uint8_t temp,uint8_t humi)
{
	if(temp>=20&&temp<=28&&humi>=40&&humi<=70)
	{
		return COMFORT_GOOD;
	}
	else	if(temp>=15&&temp<=32&&humi>=30&&humi<=80)
	{
		return COMFORT_NORMAL;
	}
	else
	{
		return COMFORT_BAD;
	}

}



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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	WeatherData test_data;
	PageType current_page=PAGE_REALTIME;
	
	
	
	test_data.temperature=25;
	test_data.humidity=55;
	test_data.comfort=COMFORT_GOOD;
	test_data.sample_id=1;
	
		DWT_Delay_Init();
		EC11_Init();
		
		OLED_UI_Init();
		OLED_UI_ShowBoot();
		HAL_Delay(1000);
		
		OLED_UI_ShowPage(current_page,&test_data);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  
        static uint32_t last_dht_time=0;
        static uint32_t last_page_time=0;
        
            uint8_t temp=0;
            uint8_t humi=0;
            EC11_Event event;
        
            EC11_Update();
            event=EC11_GetEvent();
        
        if(event==EC11_RIGHT)
        {
            current_page++;
            if(current_page>=PAGE_MAX)
            {
                current_page=PAGE_REALTIME;
            }
        OLED_UI_ShowPage(current_page,&test_data);
        }
         else if (event == EC11_LEFT)
    {
        if (current_page == PAGE_REALTIME)
        {
            current_page = PAGE_SETTING;
        }
        else
        {
            current_page--;
        }

      OLED_UI_ShowPage(current_page, &test_data);
    }
            else if (event == EC11_PRESS)
{
    if (current_page == PAGE_SETTING)
    {
        current_page = PAGE_REALTIME;
    }
    else
    {
        current_page = PAGE_SETTING;
    }

    OLED_UI_ShowPage(current_page, &test_data);
}
        if(HAL_GetTick()-last_dht_time>=2000)
        {
            last_dht_time=HAL_GetTick();
            
        
            if(DHT11_Read(&temp,&humi)==DHT11_OK)
            {
                test_data.temperature=temp;
                test_data.humidity=humi;
                test_data.comfort=App_CalcComfort(temp,humi);
                test_data.sample_id++;
            }
            
        }
        
        if(HAL_GetTick()-last_page_time>=1000)
        {
            last_page_time=HAL_GetTick();
            OLED_UI_ShowPage(current_page,&test_data);
            
        
        }
		
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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
