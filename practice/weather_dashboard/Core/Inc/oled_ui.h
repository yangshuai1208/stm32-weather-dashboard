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

void OLED_UI_SetTrendInfo(uint8_t t0,
													uint8_t  t1,
													uint8_t  t2,
													uint8_t  t3,
													uint8_t  trend,
													uint8_t   vaild);

void OLED_UI_Init(void);
void OLED_UI_ShowBoot(void);

void OLED_UI_ShowRealtime(WeatherData*data);
void OLED_UI_ShowComfort(WeatherData*data);
void OLED_UI_ShowHistory(void);
void OLED_UI_SetHistoryInfo(uint16_t count,uint8_t temp,uint8_t humi,uint8_t vaild);
void OLED_UI_ShowSetting(void);

void OLED_UI_ShowPage(PageType page,WeatherData*data);


#endif
