# Day3 DHT11 温湿度采集接入

## 1. 今日目标

在 Day2 OLED 多页面 UI 框架的基础上，接入 DHT11 温湿度传感器，将原来的模拟温湿度数据替换为真实采集数据。

## 2. 今日完成内容

- 新增 `dht11.h`
- 新增 `dht11.c`
- 新增 `dwt_delay.h`
- 新增 `dwt_delay.c`
- 配置 PB8 作为 DHT11_DATA 引脚
- 使用 DWT 实现微秒级延时
- 实现 DHT11 起始信号发送
- 实现 DHT11 响应等待
- 实现 40bit 数据读取
- 实现校验和判断
- 在 `main.c` 中周期读取 DHT11
- 使用 `WeatherData` 保存真实温湿度数据
- OLED 显示最新温湿度数据

## 3. 硬件连接

| DHT11 引脚 | STM32F103C8T6 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DATA | PB8 |

DHT11 DATA 使用单总线通信，既需要 STM32 输出起始信号，也需要 STM32 切换为输入模式读取传感器返回的数据。

## 4. DHT11 通信流程

DHT11 的通信流程如下：

```text
1. STM32 将 DATA 拉低约 20ms
2. STM32 将 DATA 拉高约 30us
3. STM32 将 DATA 引脚切换为输入模式
4. DHT11 拉低约 80us 作为响应
5. DHT11 拉高约 80us
6. DHT11 发送 40bit 数据
7. STM32 根据高电平持续时间判断 0 或 1
8. 校验数据是否正确
