#include "led_status.h"

#define LED_GOOD_PORT     GPIOB
#define LED_GOOD_PIN      GPIO_PIN_12

#define LED_NORMAL_PORT   GPIOB
#define LED_NORMAL_PIN    GPIO_PIN_13

#define LED_BAD_PORT      GPIOB
#define LED_BAD_PIN       GPIO_PIN_14


static void LED_Status_AllOff(void)
{
	HAL_GPIO_WritePin(LED_GOOD_PORT,LED_GOOD_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_NORMAL_PORT,LED_NORMAL_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_BAD_PORT,LED_BAD_PIN,GPIO_PIN_RESET);

}
void	LED_Status_Init(void)
{
	LED_Status_AllOff();
}
void LED_Status_Update(ComfortLevel comfort)
{
	LED_Status_AllOff();
	if(comfort==COMFORT_GOOD)
	{
	HAL_GPIO_WritePin(LED_GOOD_PORT,LED_GOOD_PIN,GPIO_PIN_SET);
		
	}
	else if(comfort==COMFORT_NORMAL)
	{
			HAL_GPIO_WritePin(LED_NORMAL_PORT,LED_NORMAL_PIN,GPIO_PIN_SET);
	}
	else
	{
				HAL_GPIO_WritePin(LED_BAD_PORT,LED_BAD_PIN,GPIO_PIN_SET);
	}
}
