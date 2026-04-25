# stm32-weather-dashboard
**# STM32 旋钮式环境天气仪表盘

## 1. 项目简介

本项目基于 STM32F103C8T6 开发板，使用 STM32CubeMX + HAL 库进行开发，计划实现一个带有 OLED 显示、DHT11 温湿度采集、EC11 旋钮交互、LED 舒适度指示和 W25Q64 历史数据存储功能的环境天气仪表盘。

当前项目采用 12 天迭代开发方式，从 OLED 基础显示开始，逐步完成传感器采集、页面切换、数据存储和历史曲线显示。

## 2. 当前进度

### Day1：OLED 基础显示

已完成：

- 使用 STM32CubeMX 创建 STM32F103C8T6 工程
- 配置 I2C1 外设
- 使用 PB6 作为 I2C1_SCL
- 使用 PB7 作为 I2C1_SDA
- 移植 OLED 显示驱动
- 添加 16x16 中文字库和 ASCII 字库
- 新增 oled_ui 页面显示模块
- 实现 OLED 启动页显示
- 实现模拟温湿度数据显示

## 3. 当前显示效果

启动页：

```text
兰州理工
Weather**
