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
    if (data->comfort == COMFORT_GOOD)
    {
        OLED_ShowString_F8X16(3, 0, (uint8_t *)"Comfort:Good");
    }
    else if (data->comfort == COMFORT_NORMAL)
    {
        OLED_ShowString_F8X16(3, 0, (uint8_t *)"Comfort:Normal");
    }
    else
    {
        OLED_ShowString_F8X16(3, 0, (uint8_t *)"Comfort:Bad");
    }
}