# DHT11 温湿度采集

## 项目简介

通过 P4.6 完成 DHT11 起始、响应、40-bit 数据读取与校验，将结果写入温湿度结构体并通过 UART 输出。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- DHT11 数据线：P4.6
- 调试输出：UART1，115200 baud

## 软件结构

```text
main.c
  ├─ DHT11_Init()
  ├─ DHT11_GetTempAndHumidity(&th)
  └─ UART output

DHT11.c/.h  时序、位采样、校验与数据转换
Delay.c/.h  起始信号延时
```

## 数据流程

`start low -> sensor response -> 40 pulse widths -> 5 bytes -> checksum -> TH struct`

## 关键实现

驱动按 5×8 位循环读取数据，通过高电平持续时间区分 0 和 1。前四个字节求和后与第五字节比较，失败时返回独立错误码。

## 调试记录

时序窗口由循环计数和 24 MHz 主频共同决定。移植到其他频率后必须重新核对阈值，不能把课程中的计数范围直接当作微秒值复用。

## 来源说明

`course/` 保留课程 DHT11 封装工程。
