#include "ec11.h"

#define EC11_A_PORT  GPIOA 
#define EC11_A_PIN GPIO_PIN_0

#define EC11_B_PORT  GPIOA 
#define EC11_B_PIN GPIO_PIN_1

#define EC11_SW_PORT  GPIOA 
#define EC11_SW_PIN GPIO_PIN_2

static uint8_t last_a=1;
static uint8_t last_sw=1;
static EC11_Event ec11_event=EC11_NONE;

void EC11_Init(void)
{
	last_a=HAL_GPIO_ReadPin(EC11_A_PORT,EC11_A_PIN);
	last_sw=HAL_GPIO_ReadPin(EC11_SW_PORT,EC11_SW_PIN);
	ec11_event=EC11_NONE;
}
void EC11_Update(void)
{
	uint8_t now_a;
	uint8_t now_b;
	uint8_t now_sw;
	
	now_a=HAL_GPIO_ReadPin(EC11_A_PORT,EC11_A_PIN);
	now_b=HAL_GPIO_ReadPin(EC11_B_PORT,EC11_B_PIN);
	now_sw=HAL_GPIO_ReadPin(EC11_SW_PORT,EC11_SW_PIN);
	if(last_a==1&&now_a==0)
	{
		if(now_b==1)
		{
			ec11_event=EC11_RIGHT;
		}
		else
		{
			ec11_event=EC11_LEFT;
		}
	}		
	if(last_sw==1&&now_sw==0)
	{
		ec11_event=EC11_PRESS;
	
	}
	last_a=now_a;
	last_sw=now_sw;
}
EC11_Event EC11_GetEvent(void)
{	
	EC11_Event event=ec11_event;
	ec11_event=EC11_NONE;
	
	return event;
}
