#include "dht11.h"
#include "dwt_delay.h"

#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_PIN_8

static void DHT11_SetPinOutput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	
	GPIO_InitStruct.Pin=DHT11_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull=GPIO_PULLUP;	
	GPIO_InitStruct.Speed=GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(DHT11_PORT,&GPIO_InitStruct);
}
static void DHT11_SetPinInput(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	
	GPIO_InitStruct.Pin=DHT11_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull=GPIO_PULLUP;

	HAL_GPIO_Init(DHT11_PORT,&GPIO_InitStruct);
}
static uint8_t DHT11_ReadPin(void)
{
	return HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN);

}
static uint8_t DHT11_WaitLevel(uint8_t level,uint16_t timeout_us)
{
	while(DHT11_ReadPin()==level)
	{
		if(timeout_us--==0)
		{
			return 1;
		}
		DWT_Delay_us(1);
	}

	return 0;

}
uint8_t DHT11_Read(uint8_t*temperature,uint8_t*humidity)
{
	uint8_t data[5]={0};
	uint8_t i,j;
	
	DHT11_SetPinOutput();
	
	HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,GPIO_PIN_RESET);
	HAL_Delay(20);
	
	
	HAL_GPIO_WritePin(DHT11_PORT,DHT11_PIN,GPIO_PIN_SET);
	DWT_Delay_us(30);
	DHT11_SetPinInput();
	
	if(DHT11_WaitLevel(GPIO_PIN_SET,100)) return DHT11_ERROR_TIMEOUT;
	if(DHT11_WaitLevel(GPIO_PIN_RESET,100)) return DHT11_ERROR_TIMEOUT;
	if(DHT11_WaitLevel(GPIO_PIN_SET,100)) return DHT11_ERROR_TIMEOUT;
	
	
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			if(DHT11_WaitLevel(GPIO_PIN_RESET,100)) return DHT11_ERROR_TIMEOUT;
			DWT_Delay_us(40);
			
			data[i]<<=1;
			
			if(DHT11_ReadPin()==GPIO_PIN_SET)
			{
				data[i]|=1;
			}
			if(DHT11_WaitLevel(GPIO_PIN_SET,100)) return DHT11_ERROR_TIMEOUT; 
		
		}
	
	}
	if((uint8_t)(data[0]+data[1]+data[2]+data[3])!=data[4])
	{
		return DHT11_ERROR_CHECKSUM;
	}
	*humidity=data[0];
	*temperature=data[2];
	
	return DHT11_OK;
	
}
