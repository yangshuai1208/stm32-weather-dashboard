#include "history_storage.h" 
#include "w25q64.h"

#define HISTORY_FLASH_BASE_ADDR       0x001000U
#define HISTORY_FLASH_SECTOR_SIZE     4096U
#define HISTORY_MAX_RECORDS           512U


static uint16_t g_history_write_index=0;

static uint32_t HistoryStorage_GetAddress(uint16_t index)
{
	return  HISTORY_FLASH_BASE_ADDR+index*sizeof(HistoryRecord);
}
static uint16_t HistoryStorage_CalcChecksum(HistoryRecord*record)
{
	uint16_t sum=0;
	
	sum+=record->sample_id;
	sum+=record->temperature;
	sum+=record->humidity;
	sum+=record->comfort;
	sum+=record->reserved;
	
	return sum;
}
void HistoryStorage_Init(uint8_t format)
{
	g_history_write_index=0;
	if(format)
	{
		W25Q64_EraseSector(HISTORY_FLASH_BASE_ADDR);
	}
}
uint8_t HistoryStorage_SaveWeather(WeatherData*data)
{
	HistoryRecord record;
	
	if(g_history_write_index>=HISTORY_MAX_RECORDS)
	{
		return HISTORY_ERROR;
	}
	
	record.sample_id=data->sample_id;
	record.temperature=data->temperature;
	record.humidity=data->humidity;
	record.comfort=data->comfort;
	record.reserved=0;
	record.checksum=0;
	
	record.checksum=HistoryStorage_CalcChecksum(&record);
	
	W25Q64_PageProgram(
	HistoryStorage_GetAddress(g_history_write_index),
	(uint8_t*)&record,
	sizeof(HistoryRecord)
		);
	g_history_write_index++;
	return HISTORY_OK;
}
uint8_t HistoryStorage_ReadRecord(uint16_t index,HistoryRecord*record)
{
	uint16_t checksum;
	
	if(index>=g_history_write_index)
	{
		return HISTORY_ERROR;
	}
	W25Q64_ReadData(HistoryStorage_GetAddress(index),
									(uint8_t*)record,	
									sizeof(HistoryRecord)				
									);
	checksum=record->checksum;
	record->checksum=0;
	if(HistoryStorage_CalcChecksum(record)!=checksum)
	{
		return HISTORY_ERROR;
	}
	record->checksum=checksum;
	return HISTORY_OK;
}
uint16_t HistoryStorage_GetCount(void)
{
	return g_history_write_index;

}
