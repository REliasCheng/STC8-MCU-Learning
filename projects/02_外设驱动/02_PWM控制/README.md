# PWM 控制

## 项目简介

使用 PWMB/PWM6 驱动 P0.1 上的振动电机，通过 UART 命令更新 PWM 状态，实现串口控制启停。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- PWM：PWM6，映射到 P0.1
- 控制输入：UART1，115200 baud
- 负载：课程振动电机模块

## 软件结构

```text
main.c
  ├─ GPIO_Config()
  ├─ PWM_Config()
  ├─ UART_Config()
  ├─ UpdatePwm()
  └─ stop()

STC8H_PWM.c/.h  PWM 寄存器配置
UART_Isr.c      接收串口命令
```

## 数据流程

`UART byte -> 接收缓冲 -> 命令判断 -> PWM6 enable/duty -> 电机状态`

## 关键实现

`PWM6_SW(PWM6_SW_P01)` 选择 P0.1 输出。`PWM_Configuration()` 分别配置 PWM6 通道和 PWMB 公共周期、主输出使能与计数器使能。

## 调试记录

仅修改占空比不足以产生输出，还要确认 PWMB 主输出、计数器和引脚复用全部使能。外接电机不能直接依赖 MCU 引脚供电能力，应按课程模块的驱动电路连接。

## 来源说明

`course/` 保留课程 UART 控制 PWM 电机工程。
