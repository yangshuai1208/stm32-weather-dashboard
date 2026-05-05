# 旋钮天气仪表盘

## 1. 项目简介

本项目基于 STM32F103C8T6，使用 HAL 库和 STM32CubeMX 开发，实现一个带旋钮交互、OLED 显示、温湿度采集、舒适度指示和历史数据存储的桌面天气仪表盘。

系统通过 DHT11 采集温湿度，OLED 显示实时数据和历史趋势，EC11 旋钮用于页面切换，LED 用于显示环境舒适度，W25Q64 用于保存历史温湿度记录。

## 2. 项目功能

- DHT11 温湿度采集
- OLED 中文/英文混合显示
- EC11 旋钮页面切换
- EC11 按键进入/退出设置页
- LED 舒适度状态指示
- W25Q64 SPI Flash 基础读写
- 温湿度历史记录存储
- 历史页显示最新记录
- 最近 4 条温度趋势显示

## 3. 硬件组成

| 模块 | 功能 | 引脚 |
|---|---|---|
| STM32F103C8T6 | 主控 | - |
| OLED 0.96寸 I2C | 页面显示 | PB6/PB7 |
| DHT11 | 温湿度采集 | PB8 |
| EC11 | 页面切换 | PB0/PB1/PA2 |
| LED | 舒适度指示 | PB12/PB13/PB14 |
| W25Q64 | 历史数据存储 | PA4/PA5/PA6/PA7 |

## 4. 软件架构

```text
main.c
├── App_HandleEC11()          // 旋钮事件处理
├── App_UpdateSensor()        // DHT11 采集
├── App_UpdateHistoryTrend()  // 历史趋势分析
└── App_RefreshOLED()         // OLED 刷新调度

drivers/modules
├── dht11.c
├── ec11.c
├── oled_ui.c
├── led_status.c
├── w25q64.c
├── history_storage.c
└── dwt_delay.c
- 在 Day10 历史页显示最新记录的基础上，增加最近 4 条温度趋势显示
- 新增 `OLED_UI_SetTrendInfo()` 接口
- 在 OLED UI 层增加趋势缓存变量
- 从 W25Q64 中读取最近 4 条历史记录
- 提取最近 4 条温度值
- 根据首尾温度判断趋势：上升 / 下降 / 稳定
- 历史页显示 `Cnt`、最近 4 次温度和 `Trend`
- 保持 DHT11 采集、EC11 切页、LED 舒适度指示、W25Q64 存储功能正常

## Day11 历史页显示效果

少于 4 条记录时：

```text
历史
Cnt:003
Temp:26C
Humi:55%





