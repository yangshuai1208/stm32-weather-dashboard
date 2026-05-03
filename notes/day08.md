# Day8 W25Q64 SPI Flash 驱动接入

## 1. 今日目标

在前 7 天已经完成 OLED、DHT11、EC11、LED 和系统主循环整理的基础上，接入 W25Q64 外部 SPI Flash。

今日目标不是直接实现历史曲线，而是先验证 STM32 是否能通过 SPI 稳定访问 W25Q64。

## 2. 今日完成内容

- CubeMX 开启 SPI1
- 配置 PA4 作为 W25Q64_CS
- 新增 `w25q64.h`
- 新增 `w25q64.c`
- 实现 W25Q64 读取 JEDEC ID
- 实现写使能命令
- 实现状态寄存器读取
- 实现 BUSY 等待
- 实现扇区擦除
- 实现页编程
- 实现普通数据读取
- 实现 Flash 读写自测试
- OLED 显示 `ID OK / RW OK`

## 3. 硬件连接

| W25Q64 | STM32F103C8T6 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| CS | PA4 |
| SCK | PA5 |
| MISO / DO | PA6 |
| MOSI / DI | PA7 |

注意：

W25Q64 必须使用 3.3V 供电，不建议接 5V。

## 4. SPI1 配置

SPI1 使用主机模式：

```text
Mode：Full-Duplex Master
Data Size：8 Bits
First Bit：MSB First
CPOL：Low
CPHA：1 Edge
NSS：Software
