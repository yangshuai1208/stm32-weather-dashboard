#ifndef __OLED_UI_H
#define __OLED_UI_H

#include "main.h"



typedef enum
{
	COMFORT_GOOD=0,
	COMFORT_NORMAL,
	COMFORT_BAD
}ComfortLevel;
typedef struct
{
	uint8_t temperature;
	uint8_t humidity;
	ComfortLevel comfort;
	uint32_t sample_id;

}WeatherData;

void OLED_UI_Init(void);
void OLED_UI_ShowBoot(void);
void OLED_UI_ShowRealtime(WeatherData*data);



#endif