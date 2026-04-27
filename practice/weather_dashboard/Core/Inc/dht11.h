#ifndef __DHT11_H
#define __DHT11_H
	
#include "main.h"


#define DHT11_OK					0
#define DHT11_ERROR_TIMEOUT 1
#define DHT11_ERROR_CHECKSUM 2

uint8_t DHT11_Read(uint8_t*temperature,uint8_t*humidity);

#endif
