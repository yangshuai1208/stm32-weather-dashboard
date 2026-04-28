# Day4 EC11 旋钮基础读取与页面控制

## 1. 今日目标

在 Day3 DHT11 温湿度采集的基础上，接入 EC11 旋钮，实现 OLED 页面的人机交互控制。

今日目标：

- 识别 EC11 左旋
- 识别 EC11 右旋
- 识别 EC11 按键按下
- 使用旋钮控制 OLED 页面切换

## 2. 硬件连接

| EC11 模块 | STM32F103C8T6 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| C | GND |
| A | PA0 |
| B | PA1 |
| SW | PA2 |

说明：

- A/B 是旋转编码信号
- SW 是按键输入
- C 是编码器公共端，接 GND
- PA0/PA1/PA2 配置为输入上拉

## 3. CubeMX 配置

| 引脚 | 功能 | 模式 |
|---|---|---|
| PA0 | EC11_A | GPIO_Input + Pull-up |
| PA1 | EC11_B | GPIO_Input + Pull-up |
| PA2 | EC11_SW | GPIO_Input + Pull-up |

## 4. 新增文件

```text
Core/Inc/ec11.h
Core/Src/ec11.c
