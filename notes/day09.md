# Day9 温湿度历史记录存储

## 1. 今日目标

在 Day8 已经打通 W25Q64 基础读写的基础上，设计温湿度历史记录格式，并将 DHT11 采集到的数据保存到 W25Q64 中。

Day9 的重点是打通“采集数据 → 生成历史记录 → 写入 Flash”的链路，暂时不做历史页显示。

## 2. 今日完成内容

- 新增 `history_storage.h`
- 新增 `history_storage.c`
- 设计 `HistoryRecord` 历史记录结构体
- 规划历史记录存储起始地址 `0x001000`
- 实现历史记录地址计算
- 实现历史记录 checksum 校验
- 实现历史记录保存函数
- 实现历史记录读取函数
- 实现历史记录数量统计函数
- 在 DHT11 采集成功后保存当前温湿度记录

## 3. 历史记录结构体

```c
typedef __packed struct
{
    uint16_t sample_id;
    uint8_t  temperature;
    uint8_t  humidity;
    uint8_t  comfort;
    uint8_t  reserved;
    uint16_t checksum;
} HistoryRecord;
