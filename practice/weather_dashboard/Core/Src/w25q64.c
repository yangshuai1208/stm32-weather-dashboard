#include "w25q64.h"
#include "spi.h"

#define W25Q64_CS_PORT   GPIOA
#define W25Q64_CS_PIN    GPIO_PIN_4

#define W25Q64_CMD_WRITE_ENABLE        0x06
#define W25Q64_CMD_READ_STATUS_REGE1   0x05 
#define W25Q64_CMD_PAGE_PROGRAM        0x02
#define W25Q64_CMD_READ_DATA           0x03
#define W25Q64_CMD_SECTOR_ERASE        0x20
#define W25Q64_CMD_JEDEC_ID            0x9F

static void W25Q64_CS_Low(void)
{
	HAL_GPIO_WritePin(W25Q64_CS_PORT,W25Q64_CS_PIN,GPIO_PIN_RESET);
}
static void W25Q64_CS_High(void)
{
	HAL_GPIO_WritePin(W25Q64_CS_PORT,W25Q64_CS_PIN,GPIO_PIN_SET);
}
static void W25Q64_WriteEnable(void)
{
	uint8_t cmd=W25Q64_CMD_WRITE_ENABLE;
	
	W25Q64_CS_Low();
	HAL_SPI_Transmit(&hspi1,&cmd,1,HAL_MAX_DELAY);
	W25Q64_CS_High();
}
static uint8_t W25Q64_ReadStatusReg1(void)
{
	uint8_t	cmd=W25Q64_CMD_READ_STATUS_REGE1;
	uint8_t status=0;
	
	W25Q64_CS_Low();
	HAL_SPI_Transmit(&hspi1,&cmd,1,HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1,&status,1,HAL_MAX_DELAY);
	W25Q64_CS_High();
	
	return status;
}
static void W25Q64_WaitBusy(void)
{
	while(W25Q64_ReadStatusReg1()&0x01)
	{
		HAL_Delay(1);
	}
}
uint32_t W25Q64_ReadID(void)
{
	uint8_t cmd=W25Q64_CMD_JEDEC_ID;
	uint8_t id[3]={0};
	
	W25Q64_CS_Low();
	
	HAL_SPI_Transmit(&hspi1,&cmd,1,HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1,id,3,HAL_MAX_DELAY);
	
	W25Q64_CS_High();
	
	return ((uint32_t)id[0]<<16)|((uint32_t)id[1]<<8)|id[2];
}
void W25Q64_EraseSector(uint32_t addr)
{
	uint8_t cmd[4];
	
	W25Q64_WriteEnable();
	
	cmd[0]=W25Q64_CMD_SECTOR_ERASE;
	cmd[1]=(addr>>16)&0xFF;
	cmd[2]=(addr>>8)&0xFF;
	cmd[3]=addr&0xFF;
	
	W25Q64_CS_Low();
	HAL_SPI_Transmit(&hspi1,cmd,4,HAL_MAX_DELAY);
	W25Q64_CS_High();
	
	W25Q64_WaitBusy();
}
void W25Q64_PageProgram(uint32_t addr,uint8_t *buf,uint16_t len)	
{
	uint8_t cmd[4];
	
	if(len>256)
	{
		len=256;
	}
	
	W25Q64_WriteEnable();
	
	cmd[0]=W25Q64_CMD_PAGE_PROGRAM;
	cmd[1]=(addr>>16)&0xFF;
	cmd[2]=(addr>>8)&0xFF;
	cmd[3]=addr&0xFF;
	
	W25Q64_CS_Low();
	HAL_SPI_Transmit(&hspi1,cmd,4,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi1,buf,len,HAL_MAX_DELAY);
	W25Q64_CS_High();
	
	W25Q64_WaitBusy();
}
void W25Q64_ReadData(uint32_t addr,uint8_t*buf,uint16_t len)
{
	uint8_t cmd[4];
	
	cmd[0]=W25Q64_CMD_READ_DATA;
	cmd[1]=(addr>>16)&0xFF;
	cmd[2]=(addr>>8)&0xFF;
	cmd[3]=addr&0xFF;

	W25Q64_CS_Low();
	HAL_SPI_Transmit(&hspi1,cmd,4,HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1,buf,len,HAL_MAX_DELAY);
	W25Q64_CS_High();
}
uint8_t W25Q64_Test(void)
{
	uint8_t write_buf[4]={0x12,0x34,0x56,0x78};
	uint8_t read_buf[4]={0};
	uint8_t i;

	W25Q64_EraseSector(0x000000);
	W25Q64_PageProgram(0x000000,write_buf,4);
	W25Q64_ReadData(0x000000,read_buf,4);
	
	for(i=0;i<4;i++)
	{
		if(write_buf[i]!=read_buf[i])
		{
			return W25Q64_ERROR;
		}
	}
	
	return W25Q64_OK;
}
