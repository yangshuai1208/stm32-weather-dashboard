#include "oled_ui.h"
#include "oled.h"

static void OLED_UI_Show2Digit(uint8_t line, uint8_t offset, uint8_t num)
{
    char buf[3];

    buf[0] = num / 10 + '0';
    buf[1] = num % 10 + '0';
    buf[2] = '\0';

    OLED_ShowString_F8X16(line, offset, (uint8_t *)buf);
}

void OLED_UI_Init(void)
{
    OLED_Init();
    OLED_CLS();
}

void OLED_UI_ShowBoot(void)
{
    OLED_CLS();

    // 第0行：兰州理工
    OLED_ShowChinese_F16X16(0, 0, 0);  // 兰
    OLED_ShowChinese_F16X16(0, 1, 1);  // 州
    OLED_ShowChinese_F16X16(0, 2, 2);  // 理
    OLED_ShowChinese_F16X16(0, 3, 3);  // 工

    // 第2行：Weather
    OLED_ShowString_F8X16(2, 0, (uint8_t *)"Weather");
}
static	void OLED_UI_ShowComfortText(uint8_t line,uint8_t offset,ComfortLevel comfort)
{
		if(comfort==COMFORT_GOOD)
		{
			OLED_ShowString_F8X16(line,offset,(uint8_t*)"Good");
		}
		else if(comfort==COMFORT_NORMAL)
		{
			OLED_ShowString_F8X16(line,offset,(uint8_t*)"Normal");
		
		}
		else
		{
		
			OLED_ShowString_F8X16(line,offset,(uint8_t*)"Bad");
		}
}
void OLED_UI_ShowRealtime(WeatherData *data)
{
    OLED_CLS();

    // 第0行：温度:25C
    OLED_ShowChinese_F16X16(0, 0, 4);  // 温
    OLED_ShowChinese_F16X16(0, 1, 5);  // 度
    OLED_ShowString_F8X16(0, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(0, 5, data->temperature);
    OLED_ShowString_F8X16(0, 7, (uint8_t *)"C");

    // 第1行：湿度:55%
    OLED_ShowChinese_F16X16(1, 0, 6);  // 湿
    OLED_ShowChinese_F16X16(1, 1, 7);  // 度
    OLED_ShowString_F8X16(1, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(1, 5, data->humidity);
    OLED_ShowString_F8X16(1, 7, (uint8_t *)"%");

    // 第3行：舒适度英文状态
	OLED_ShowString_F8X16(3,0,(uint8_t*)"Comfort:");
	OLED_UI_ShowComfortText(3,8,data->comfort);
 
}

void 	OLED_UI_ShowComfort(WeatherData *data)
{
	OLED_CLS();
	OLED_ShowString_F8X16(0,0,(uint8_t*)"Comfort:");
	
	OLED_ShowString_F8X16(1,0,(uint8_t*)"Temp:");
	OLED_UI_Show2Digit(1,5,data->temperature);
	OLED_ShowString_F8X16(1,7,(uint8_t*)"C");
	
	OLED_ShowString_F8X16(2,0,(uint8_t*)"Humi:");
	OLED_UI_Show2Digit(2,5,data->humidity);
	OLED_ShowString_F8X16(2,7,(uint8_t*)"%");
	
	OLED_ShowString_F8X16(3,0,(uint8_t*)"Level:");
	OLED_UI_ShowComfortText(3,6,data->comfort);
	
	
}
void OLED_UI_ShowHistory(void)
{
	OLED_CLS();

	OLED_ShowString_F8X16(0,0,(uint8_t*)"History");
	OLED_ShowString_F8X16(1,0,(uint8_t*)"Temp Curve");
	OLED_ShowString_F8X16(2,0,(uint8_t*)"No Flash Yet:");
	OLED_ShowString_F8X16(3,0,(uint8_t*)"Day2 Test");
	
}	
void OLED_UI_ShowSetting(void)
{
	OLED_CLS();
	
	OLED_ShowString_F8X16(0,0,(uint8_t*)"Setting:");
	OLED_ShowString_F8X16(1,0,(uint8_t*)"Sample:5s");
	OLED_ShowString_F8X16(2,0,(uint8_t*)"Save:ON");
	OLED_ShowString_F8X16(3,0,(uint8_t*)"LED:Auto");
}
void OLED_UI_ShowPage(PageType page,WeatherData*data)
{
	switch(page)
	{
		case 	PAGE_REALTIME:
		OLED_UI_ShowRealtime(data);
		break;
		
		case	PAGE_COMFORT:
		OLED_UI_ShowComfort(data);
		break;
	
		case	PAGE_HISTORY:
		OLED_UI_ShowHistory();
		break;
	
		case		PAGE_SETTING:
		OLED_UI_ShowSetting();
		break;
		
		default	:
			OLED_UI_ShowRealtime(data);
			break;
	}
}
