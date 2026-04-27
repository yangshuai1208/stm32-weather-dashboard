#include "dwt_delay.h"

void DWT_Delay_Init(void)
{
	CoreDebug->DEMCR|=CoreDebug_DEMCR_TRCENA_Msk;
	
	DWT->CYCCNT=0;
	DWT->CTRL|=DWT_CTRL_CYCCNTENA_Msk;
	
}
void DWT_Delay_us(uint32_t us)
{
	uint32_t start_tick=DWT->CYCCNT;
	uint32_t delay_tick=us*(HAL_RCC_GetHCLKFreq()/1000000);
	
	while(DWT->CYCCNT-start_tick<delay_tick)
	{
		
	}
	
}
