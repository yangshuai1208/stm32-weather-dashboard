#ifndef __W25Q64_H
#define __W25Q64_H

#include "main.h"

#define W25Q64_OK 0
#define W25Q64_ERROR  1

uint32_t W25Q64_ReadID(void);


void W25Q64_ReadData(uint32_t addr,uint8_t*buf,uint16_t len);
void W25Q64_PageProgram(uint32_t addr,uint8_t *buf,uint16_t len);
void W25Q64_EraseSector(uint32_t addr);

uint8_t W25Q64_Test(void);

#endif
