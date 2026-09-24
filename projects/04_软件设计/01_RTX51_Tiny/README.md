# RTX51 Tiny 任务协作

## 项目简介

使用 RTX51 Tiny 定义启动任务、LED 任务和 UART 任务。LED 任务按 tick 周期运行，在达到计数条件后等待 signal；UART 收到 `0x01` 时唤醒该任务。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- LED：P5.3
- UART1：P3.0/P3.1，115200 baud
- 内核：Keil RTX51 Tiny

## 软件结构

```text
task_main (_task_ 0)
  ├─ GPIO_Config()
  ├─ UART_Config()
  ├─ os_create_task(1)
  ├─ os_create_task(2)
  └─ os_delete_task(0)

task_led1 (_task_ 1)  -> K_TMO / K_SIG
task_uart (_task_ 2)  -> UART buffer / os_send_signal(1)
```

## 数据流程

`UART byte 0x01 -> RX buffer -> task_uart -> os_send_signal(1) -> task_led1 resumes`

## 关键实现

普通 `main()` 被 `_task_ 0` 入口替代。`os_wait2(K_TMO, 200)` 让任务在等待期间交还 CPU；`os_wait1(K_SIG)` 与 `os_send_signal()` 实现任务间事件同步。

## 调试记录

课程记录的常见问题包括多目录 Include Path 缺失，以及同时保留普通 `main()` 导致 `MULTIPLE PUBLIC DEFINITIONS: MAIN`。本工程说明的是 RTX51 Tiny 的静态任务与信号协作，不等同于完整 RTOS 项目。

## 来源说明

`course/` 保留课程 K_SIG 工程、RTX51 配置和 STC 驱动文件。
