# PCF8563 RTC 时间系统

## 项目简介

通过 STC8 硬件 I²C 访问 PCF8563，封装日期时间读写，并处理闹钟 AF、定时器 TF 与外部中断状态。

## 硬件环境

- MCU：STC8H8K64U
- RTC：PCF8563
- I²C：工程选择 P3.2/P3.3 引脚组
- 中断：RTC INT 输出接外部中断输入

## 软件结构

```text
main.c
  ├─ RTC_Init()
  ├─ RTC_WriteTime()/RTC_ReadTime()
  └─ handle_alarm_timer()

RTC.c/.h   BCD、时间、闹钟、定时器
I2C.c/.h   START/STOP、字节收发、ACK
Exti.c/.h  RTC 中断入口
```

## 数据流程

`PCF8563 registers <-> hardware I²C <-> RTC_Time -> UART / application`

中断路径为：`INT active -> EXTI -> read control/status -> distinguish AF/TF -> clear flag`。

## 关键实现

RTC 模块把 BCD 寄存器与十进制时间结构体转换分开。闹钟和定时器共享状态寄存器，需要分别检查和清除 AF、TF，避免重复进入中断。

## 调试记录

课程示例启动时写入固定时间，便于课堂观察；实际应用应只在首次配置或用户设置时写 RTC。P3.2 同时连接板载按键，外接 RTC 时必须核对资源冲突。

## 来源说明

`course/` 保留课程 RTC 定时器封装工程。
