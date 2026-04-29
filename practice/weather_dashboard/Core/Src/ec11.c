#include "ec11.h"

#define EC11_A_PORT   GPIOB
#define EC11_A_PIN    GPIO_PIN_0

#define EC11_B_PORT   GPIOB
#define EC11_B_PIN    GPIO_PIN_1

#define EC11_SW_PORT  GPIOA
#define EC11_SW_PIN   GPIO_PIN_2

#define EC11_ROTATAE_DEBOUNCE_MS 25
#define EC11_KEY_DEBOUNCE_MS    80

static uint8_t last_a=1;
static uint8_t last_sw=1;

static uint32_t last_rotate_time=0;
static uint32_t last_key_time=0;

static EC11_Event ec11_event=EC11_NONE;

void EC11_Init(void)
{
	last_a=HAL_GPIO_ReadPin(EC11_A_PORT,EC11_A_PIN);
	last_sw=HAL_GPIO_ReadPin(EC11_SW_PORT,EC11_SW_PIN);
	
	last_rotate_time=HAL_GetTick();
	last_key_time=HAL_GetTick();
	
	ec11_event=EC11_NONE;
}
void EC11_Update(void)
{
    uint8_t now_a;
    uint8_t now_b;
    uint8_t now_sw;
		uint32_t now_time;

    now_a = HAL_GPIO_ReadPin(EC11_A_PORT, EC11_A_PIN);
    now_b = HAL_GPIO_ReadPin(EC11_B_PORT, EC11_B_PIN);
    now_sw = HAL_GPIO_ReadPin(EC11_SW_PORT, EC11_SW_PIN);
		now_time=HAL_GetTick();
		if(last_a==1&&now_a==0)
		{
				if(now_time-last_rotate_time>=EC11_ROTATAE_DEBOUNCE_MS)
				{
					last_rotate_time=now_time;
					if(now_b==1)
					{
					ec11_event=EC11_RIGHT;
					}
					else
					{
					ec11_event=EC11_LEFT;
					}
				}
			}	
  /*
			按键检测：
			SW输入上拉，按下时为0.
			检测下降沿，加入80ms消抖
			*/
    if (last_sw == 1 && now_sw == 0)
    {
			if(now_time-last_key_time>=EC11_KEY_DEBOUNCE_MS)
			{
				last_key_time=now_time;
				ec11_event=EC11_PRESS;
			}	
    }

    last_a = now_a;
    last_sw = now_sw;
}
EC11_Event EC11_GetEvent(void)
{	
	EC11_Event event=ec11_event;
	ec11_event=EC11_NONE;
	
	return event;
}
