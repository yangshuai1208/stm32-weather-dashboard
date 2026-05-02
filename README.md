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
## 当前进度

### Day1：OLED 基础显示

已完成：

- 使用 STM32CubeMX 创建工程
- 配置 I2C1 外设
- PB6 作为 I2C1_SCL
- PB7 作为 I2C1_SDA
- 移植 OLED 底层驱动
- 添加 OLED 字库文件
- 实现 OLED 启动页
- 实现模拟温湿度数据显示

### Day2：OLED 多页面 UI 框架

已完成：

- 新增 PageType 页面枚举
- 新增 4 个 OLED 页面：
  - 实时数据页
  - 舒适度页
  - 历史页占位
  - 设置页占位
- 新增 OLED_UI_ShowPage() 统一页面显示接口
- 使用测试数据实现页面自动切换
- 为后续 EC11 旋钮切换页面做准备

## 当前页面效果

### 1. 实时数据页

```text
温度:25C
湿度:55%
Comfort:Good


### Day3：DHT11 温湿度采集接入

已完成：

- 新增 DHT11 温湿度传感器驱动
- 使用 PB8 作为 DHT11_DATA 引脚
- 配置 DHT11_DATA 为开漏输出并上拉
- 新增 DWT 微秒级延时模块
- 实现 DHT11 起始信号、响应检测、40bit 数据读取和校验和判断
- 将模拟温湿度数据替换为 DHT11 采集数据
- 使用 WeatherData 结构体统一保存温度、湿度、舒适度和采样编号
- 使用 HAL_GetTick() 实现非阻塞周期采集和页面切换

## Day3 硬件连接

| 模块 | 引脚 | STM32F103C8T6 |
|---|---|---|
| DHT11 | VCC | 3.3V |
| DHT11 | GND | GND |
| DHT11 | DATA | PB8 |

## Day3 软件模块

| 文件 | 作用 |
|---|---|
| dht11.c / dht11.h | DHT11 温湿度采集驱动 |
| dwt_delay.c / dwt_delay.h | DWT 微秒级延时 |
| main.c | 周期读取 DHT11 并更新 WeatherData |
| gpio.c / main.h | DHT11_DATA 引脚初始化 |

## 当前实验现象

程序烧录后，OLED 正常显示多页面 UI。

实时数据页显示 DHT11 采集到的温湿度数据：

```text
实时数据
温度:xxC
湿度:xx%

### Day4：EC11 旋钮交互接入

已完成：

- 新增 `ec11.h / ec11.c` 旋钮驱动模块
- 使用 PA0 作为 EC11_A
- 使用 PA1 作为 EC11_B
- 使用 PA2 作为 EC11_SW
- 配置 PA0/PA1/PA2 为 GPIO 输入上拉
- 实现 EC11 左旋、右旋、按下事件识别
- 使用 `EC11_Update()` 周期扫描旋钮状态
- 使用 `EC11_GetEvent()` 获取并清除旋钮事件
- 实现右旋切下一页、左旋切上一页、按下进入设置页

## Day4 硬件连接

| EC11 模块 | STM32F103C8T6 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| C | GND |
| A | PA0 |
| B | PA1 |
| SW | PA2 |

## Day4 实验现象

程序烧录后，OLED 显示项目页面：

```text
实时数据
温度:xxC
湿度:xx%
舒适:好/中/差

舒适:好/中/差

### Day5：EC11 页面交互优化与消抖

已完成：

- 优化 EC11 旋钮交互逻辑
- 增加旋钮旋转消抖
- 增加 EC11 按键消抖
- 右旋切换到下一页
- 左旋切换到上一页
- 按下进入或退出设置页
- 取消 OLED 无条件高频刷新
- 保留 DHT11 每 2 秒周期采集
- 保留 OLED 每 1 秒刷新当前页面
- 初步形成“旋钮输入 → 页面状态切换 → OLED 显示”的交互链路

## Day5 交互逻辑

| EC11 操作 | 系统响应 |
|---|---|
| 右旋 | 切换到下一页 |
| 左旋 | 切换到上一页 |
| 按下 | 设置页 / 实时数据页切换 |

页面顺序：

```text
实时数据 → 舒适 → 历史 → 设置 → 实时数据

### Day6：LED 舒适度指示

已完成：

- 新增 `led_status.h / led_status.c` LED 状态指示模块
- 使用 PB12 作为舒适状态绿灯输出
- 使用 PB13 作为一般状态黄灯输出
- 使用 PB14 作为不舒适状态红灯输出
- 将 LED 状态与 `ComfortLevel` 舒适度枚举绑定
- DHT11 采集成功后，根据温湿度重新计算舒适度
- OLED 显示舒适度结果，LED 同步显示环境状态
- 保持 EC11 页面切换功能正常运行

## Day6 硬件连接

| LED 功能 | STM32 引脚 | LED 颜色 | 状态含义 |
|---|---|---|---|
| 舒适指示 | PB12 | 绿灯 | 环境舒适 |
| 一般指示 | PB13 | 黄灯 | 环境一般 |
| 不舒适指示 | PB14 | 红灯 | 环境不舒适 |

接线方式：

```text
GPIO → 电阻 → LED 正极
LED 负极 → GND


### Day7：系统主循环整理与裸机任务调度

已完成：

- 整理 `main.c` 主循环结构
- 将 EC11 页面交互逻辑封装为 `App_HandleEC11()`
- 将 DHT11 采集逻辑封装为 `App_UpdateSensor()`
- 将 OLED 刷新逻辑封装为 `App_RefreshOLED()`
- 新增 `g_weather_data` 全局环境数据状态
- 新增 `g_current_page` 当前页面状态
- 新增 `g_oled_need_refresh` OLED 刷新标志
- 使用 `HAL_GetTick()` 实现非阻塞周期调度
- 保持 DHT11、OLED、EC11、LED 功能正常运行
- 为后续 W25Q64 历史数据存储接入预留清晰主循环结构

## Day7 系统调度结构

当前主循环结构：

```c
while (1)
{
    uint32_t now = HAL_GetTick();

    App_HandleEC11();
    App_UpdateSensor(now);
    App_RefreshOLED(now);
}
