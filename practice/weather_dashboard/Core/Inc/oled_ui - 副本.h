#ifndef __OLED_UI_H
#define __OLED_UI_H

#include "main.h"



typedef enum
{
	COMFORT_GOOD=0,
	COMFORT_NORMAL,
	COMFORT_BAD
}ComfortLevel;
typedef enum
{
	PAGE_REALTIME=0,
	PAGE_COMFORT,
	PAGE_HISTORY,
	PAGE_SETTING,
	PAGE_MAX
}PageType;

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
void OLED_UI_ShowComfort(WeatherData*data);
void OLED_UI_ShowHistory(void);
void OLED_UI_ShowSetting(void);

void OLED_UI_ShowPage(PageType page,WeatherData*data);


#endif
