#include "oled_ui.h"
#include "oled.h"

typedef enum
{
    CN_LAN = 0,
    CN_ZHOU,
    CN_LI,
    CN_GONG,
    CN_WEN,
    CN_DU,
    CN_SHI_WET,
    CN_DU2,

    CN_SHI_REAL,
    CN_SHI_TIME,
    CN_SHU,
    CN_SHI_COMFORT,
    CN_LI_HISTORY,
    CN_SHI_HISTORY,
    CN_SHE,
    CN_ZHI,
    CN_CAI,
    CN_YANG,
    CN_BAO,
    CN_CUN,
    CN_ZI,
    CN_DONG,
    CN_QU,
    CN_XIAN,
    CN_HAO,
    CN_ZHONG,
    CN_CHA,
    CN_ZAN,
    CN_WU,
    CN_SHU_DATA,
    CN_JU,
    CN_YE
} ChineseIndex;




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
    OLED_ShowChinese_F16X16(0, 0, CN_LAN);   // 兰
    OLED_ShowChinese_F16X16(0, 1, CN_ZHOU);  // 州
    OLED_ShowChinese_F16X16(0, 2, CN_LI);    // 理
    OLED_ShowChinese_F16X16(0, 3, CN_GONG);  // 工

    // 第2行：实时数据
    OLED_ShowChinese_F16X16(2, 0, CN_SHI_REAL); // 实
    OLED_ShowChinese_F16X16(2, 1, CN_SHI_TIME); // 时
    OLED_ShowChinese_F16X16(2, 2, CN_SHU_DATA); // 数
    OLED_ShowChinese_F16X16(2, 3, CN_JU);       // 据
}
static void OLED_UI_ShowComfortChinese(uint8_t line, uint8_t offset, ComfortLevel comfort)
{
    if (comfort == COMFORT_GOOD)
    {
        OLED_ShowChinese_F16X16(line, offset, CN_HAO);   // 好
    }
    else if (comfort == COMFORT_NORMAL)
    {
        OLED_ShowChinese_F16X16(line, offset, CN_ZHONG); // 中
    }
    else
    {
        OLED_ShowChinese_F16X16(line, offset, CN_CHA);   // 差
    }
}
void OLED_UI_ShowRealtime(WeatherData *data)
{
    OLED_CLS();

    // 第0行：实时数据
    OLED_ShowChinese_F16X16(0, 0, CN_SHI_REAL); // 实
    OLED_ShowChinese_F16X16(0, 1, CN_SHI_TIME); // 时
    OLED_ShowChinese_F16X16(0, 2, CN_SHU_DATA); // 数
    OLED_ShowChinese_F16X16(0, 3, CN_JU);       // 据

    // 第1行：温度:25C
    OLED_ShowChinese_F16X16(1, 0, CN_WEN);      // 温
    OLED_ShowChinese_F16X16(1, 1, CN_DU);       // 度
    OLED_ShowString_F8X16(1, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(1, 5, data->temperature);
    OLED_ShowString_F8X16(1, 7, (uint8_t *)"C");

    // 第2行：湿度:55%
    OLED_ShowChinese_F16X16(2, 0, CN_SHI_WET);  // 湿
    OLED_ShowChinese_F16X16(2, 1, CN_DU2);      // 度
    OLED_ShowString_F8X16(2, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(2, 5, data->humidity);
    OLED_ShowString_F8X16(2, 7, (uint8_t *)"%");

    // 第3行：舒适:好/中/差
    OLED_ShowChinese_F16X16(3, 0, CN_SHU);          // 舒
    OLED_ShowChinese_F16X16(3, 1, CN_SHI_COMFORT);  // 适
    OLED_ShowString_F8X16(3, 4, (uint8_t *)":");
    OLED_UI_ShowComfortChinese(3, 3, data->comfort);
}

void OLED_UI_ShowComfort(WeatherData *data)
{
    OLED_CLS();

    // 第0行：舒适
    OLED_ShowChinese_F16X16(0, 0, CN_SHU);          // 舒
    OLED_ShowChinese_F16X16(0, 1, CN_SHI_COMFORT);  // 适

    // 第1行：温度:25C
    OLED_ShowChinese_F16X16(1, 0, CN_WEN);          // 温
    OLED_ShowChinese_F16X16(1, 1, CN_DU);           // 度
    OLED_ShowString_F8X16(1, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(1, 5, data->temperature);
    OLED_ShowString_F8X16(1, 7, (uint8_t *)"C");

    // 第2行：湿度:55%
    OLED_ShowChinese_F16X16(2, 0, CN_SHI_WET);      // 湿
    OLED_ShowChinese_F16X16(2, 1, CN_DU2);          // 度
    OLED_ShowString_F8X16(2, 4, (uint8_t *)":");
    OLED_UI_Show2Digit(2, 5, data->humidity);
    OLED_ShowString_F8X16(2, 7, (uint8_t *)"%");

    // 第3行：舒适:好/中/差
    OLED_ShowChinese_F16X16(3, 0, CN_SHU);          // 舒
    OLED_ShowChinese_F16X16(3, 1, CN_SHI_COMFORT);  // 适
    OLED_ShowString_F8X16(3, 4, (uint8_t *)":");
    OLED_UI_ShowComfortChinese(3, 3, data->comfort);
}
void OLED_UI_ShowHistory(void)
{
    OLED_CLS();

    // 第0行：历史
    OLED_ShowChinese_F16X16(0, 0, CN_LI_HISTORY);   // 历
    OLED_ShowChinese_F16X16(0, 1, CN_SHI_HISTORY);  // 史

    // 第1行：曲线
    OLED_ShowChinese_F16X16(1, 0, CN_QU);           // 曲
    OLED_ShowChinese_F16X16(1, 1, CN_XIAN);         // 线

    // 第2行：暂无数据
    OLED_ShowChinese_F16X16(2, 0, CN_ZAN);          // 暂
    OLED_ShowChinese_F16X16(2, 1, CN_WU);           // 无
    OLED_ShowChinese_F16X16(2, 2, CN_SHU_DATA);     // 数
    OLED_ShowChinese_F16X16(2, 3, CN_JU);           // 据
}
void OLED_UI_ShowSetting(void)
{
    OLED_CLS();

    // 第0行：设置
    OLED_ShowChinese_F16X16(0, 0, CN_SHE);  // 设
    OLED_ShowChinese_F16X16(0, 1, CN_ZHI);  // 置

    // 第1行：采样:5s
    OLED_ShowChinese_F16X16(1, 0, CN_CAI);  // 采
    OLED_ShowChinese_F16X16(1, 1, CN_YANG); // 样
    OLED_ShowString_F8X16(1, 4, (uint8_t *)":5s");

    // 第2行：保存:ON
    OLED_ShowChinese_F16X16(2, 0, CN_BAO);  // 保
    OLED_ShowChinese_F16X16(2, 1, CN_CUN);  // 存
    OLED_ShowString_F8X16(2, 4, (uint8_t *)":ON");

    // 第3行：自动
    OLED_ShowChinese_F16X16(3, 0, CN_ZI);   // 自
    OLED_ShowChinese_F16X16(3, 1, CN_DONG); // 动
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
