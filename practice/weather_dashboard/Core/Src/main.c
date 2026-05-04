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
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled_ui.h"
#include "dht11.h"
#include "dwt_delay.h"
#include "ec11.h"
#include "led_status.h"
#include "w25q64.h"
#include "oled.h"
#include "history_storage.h" 

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define  APP_DHT_PERIOD_MS  2000U
#define  APP_OLED_PERIOD_MS  1000U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static WeatherData g_weather_data;
static PageType g_current_page=PAGE_REALTIME;

static uint8_t g_oled_need_refresh=1;

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
static PageType App_GetNextPage(PageType page)
{
	switch(page)
	{
		case PAGE_REALTIME:
			return 	PAGE_COMFORT;
		
		case PAGE_COMFORT: 
			return PAGE_HISTORY;
		
		case PAGE_HISTORY:
			return PAGE_SETTING;
		
		case PAGE_SETTING:
		default:
			return PAGE_REALTIME;
	}
}	
static PageType App_GetPrevPage(PageType page)
{
	switch(page)
	{
		case PAGE_REALTIME:
			return 	PAGE_SETTING;
		
		case PAGE_COMFORT: 
			return PAGE_REALTIME;
		
		case PAGE_HISTORY:
			return PAGE_COMFORT;
		
		case PAGE_SETTING:
		default:
			return PAGE_HISTORY;
	}
}	
static void App_HandleEC11(void)
{
	EC11_Event event;
	
	EC11_Update();
	event=EC11_GetEvent();
	
	if(event==EC11_RIGHT)
	{
		g_current_page=App_GetNextPage(g_current_page);
		g_oled_need_refresh=1;
	}
	else if(event==EC11_LEFT)
	{
		g_current_page=App_GetPrevPage(g_current_page);
		g_oled_need_refresh=1;
	}
	else if(event==EC11_PRESS)
	{
		if(g_current_page==PAGE_SETTING)
		{
			g_current_page=PAGE_REALTIME;
		}
		else
		{
			g_current_page=PAGE_SETTING;
		}
		g_oled_need_refresh=1;
	}
}
static void App_UpdateSensor(uint32_t now)
{
	static uint32_t last_dht_time=0;

	uint8_t temp=0;
	uint8_t humi=0;
	
	if(now-last_dht_time>=APP_DHT_PERIOD_MS)
	{
		last_dht_time=now;
		if(DHT11_Read(&temp,&humi)==DHT11_OK)
		{
			g_weather_data.temperature=temp;
			g_weather_data.humidity=humi;
			g_weather_data.comfort=App_CalcComfort(temp,humi);
			g_weather_data.sample_id++;
			
			LED_Status_Update(g_weather_data.comfort);
			
			if(HistoryStorage_SaveWeather(&g_weather_data)==HISTORY_OK)
			{
				HistoryRecord record;
				uint16_t count;
				
				count=HistoryStorage_GetCount();
				
				if(count>0)
				{
					if(HistoryStorage_ReadRecord(count-1,&record)==HISTORY_OK)
					{
						OLED_UI_SetHistoryInfo(
																		count,
																		record.temperature,
																		record.humidity,
																		1);
					}
				}	
			}
			
						g_oled_need_refresh=1;			
						
		}
	}		
}			
static void App_RefreshOLED(uint32_t now)
{
	static uint32_t last_oled_time=0;
	
	if(g_oled_need_refresh||(now-last_oled_time>=APP_OLED_PERIOD_MS))
	{
		last_oled_time=now;
	
	OLED_UI_ShowPage(g_current_page,&g_weather_data);
	
	g_oled_need_refresh=0;
	}
}
static void App_DataInit(void)
{
	g_weather_data.temperature=25;
	g_weather_data.humidity=55;
	g_weather_data.comfort=COMFORT_GOOD;
	g_weather_data.sample_id=1;

	g_current_page=PAGE_REALTIME;
	g_oled_need_refresh=1;
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
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
		App_DataInit();
	

		DWT_Delay_Init();
		EC11_Init();
		
		OLED_UI_Init();
		LED_Status_Init();
		
		OLED_UI_ShowBoot();
		HAL_Delay(1000);
		
		LED_Status_Update(g_weather_data.comfort);

		uint32_t flash_id;
		uint8_t flash_test_result;
		
		flash_id=W25Q64_ReadID();
		flash_test_result=W25Q64_Test();
		
		HistoryStorage_Init(1);
		OLED_UI_SetHistoryInfo(0,0,0,0);
		
		OLED_CLS();
		OLED_ShowString_F8X16(0,0,(uint8_t*)"W25Q64");
	
		if(flash_id!=0)
		{
			OLED_ShowString_F8X16(1,0,(uint8_t*)"ID OK");
		}
		else
		{
			OLED_ShowString_F8X16(1,0,(uint8_t*)"ID ERR");
		}
		if(flash_test_result==W25Q64_OK)
		{
				OLED_ShowString_F8X16(2,0,(uint8_t*)"RW OK");
		}
		else
		{
				OLED_ShowString_F8X16(2,0,(uint8_t*)"RW ERR");
		}
			HAL_Delay(1500);
			OLED_UI_ShowPage(g_current_page,&g_weather_data);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		uint32_t now=HAL_GetTick();
		
		App_HandleEC11();
		App_UpdateSensor(now);
		App_RefreshOLED(now);
		
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
