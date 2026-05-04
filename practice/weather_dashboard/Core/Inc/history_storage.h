#ifndef __HISTORY_STORAGE_H
#define __HISTORY_STORAGE_H

#include "main.h"
#include "oled_ui.h"

#define HISTORY_OK    0
#define HISTORY_ERROR 1

typedef __packed struct
{
	uint16_t sample_id;
	uint8_t  temperature;
	uint8_t  humidity;
	uint8_t  comfort;
	uint8_t  reserved;
	uint16_t  checksum;
}HistoryRecord;


void HistoryStorage_Init(uint8_t format);
uint8_t HistoryStorage_SaveWeather(WeatherData*data);
uint8_t HistoryStorage_ReadRecord(uint16_t index,HistoryRecord*record);
uint16_t HistoryStorage_GetCount(void);



#endif
