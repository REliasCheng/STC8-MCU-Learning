# ADC 采样与 NTC 温度换算

## 项目简介

从 P0.5/ADC_CH13 读取 NTC 分压，使用 12-bit ADC 结果进行查表换算，并通过 UART 输出温度信息。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- 模拟输入：P0.5 / ADC_CH13
- 参考电压：课程计算使用 2.5 V
- 传感器：NTC 热敏电阻分压电路

## 软件结构

```text
main.c
  ├─ GPIO_Config()
  ├─ ADC_Config()
  ├─ UART_Config()
  ├─ 读取 ADC
  └─ 查表并输出温度

ADC.c/.h   ADC 电源、通道和转换
UART.c/.h  串口输出
```

## 数据流程

`NTC resistance -> 分压 -> ADC_CH13 -> 12-bit sample -> table lookup -> UART`

## 关键实现

ADC 模块负责通道与转换控制，应用层把采样值与 NTC 表比较后得到温度区间。该方式避免在 8051 上进行复杂浮点曲线计算。

## 调试记录

温度偏差需要从参考电压、分压电阻、ADC 输入模式和 NTC 表型号四处核对。代码输出不能代替万用表或温度基准的实测校准。

## 来源说明

`course/` 保留课程 ADC 热敏电阻工程和原始查表数据。
