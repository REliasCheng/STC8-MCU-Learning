# Timer 周期任务

## 项目简介

Timer0 产生 1 ms 周期中断，ISR 调用 `Handle_Timer0_Interrupt()`。应用每 20 ms 检查 UART 接收超时并处理缓冲区，把持续轮询改为固定节拍任务。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- Timer0：1T、16-bit 自动重装
- UART1：P3.0/P3.1，115200 baud
- LED：P5.3

## 软件结构

```text
main.c
  ├─ GPIO_Config()
  ├─ TIMER_Config()
  ├─ UART_Config()
  └─ Handle_Timer0_Interrupt()

Timer_Isr.c -> Handle_Timer0_Interrupt()
UART_Isr.c  -> RX1_Buffer / RX_TimeOut
```

## 数据流程

`Timer0 overflow -> ISR -> 1 ms 计数 -> 20 ms 周期 -> 检查 UART 接收 -> 处理数据`

## 关键实现

定时初值使用 `65536 - MAIN_Fosc / 1000` 计算。中断文件只负责进入回调，具体周期计数和串口处理位于 `main.c`，避免把完整业务写进 ISR。

## 调试记录

Timer0 周期依赖 1T/12T 设置和主频。UART1 使用 Timer1 生成波特率，两者的定时器资源应分开检查。

## 来源说明

`course/` 来自课程“定时获取串口数据”工程，已排除历史构建产物。
