# RTC + OLED 时间显示

## 项目简介

PCF8563 提供日期和时间，OLED 显示格式化结果。工程把 RTC 硬件 I²C 和 OLED 软件 I²C 放入同一个 Keil 项目，用于分析多驱动整合和引脚资源冲突。

## 硬件环境

- MCU：STC8H8K64U，24 MHz
- RTC：PCF8563
- 显示：0.96 英寸 OLED
- 调试：UART1
- RTC 硬件 I²C：P3.2/P3.3
- OLED 软件 I²C：P3.2/P3.3，RESET 使用 P1.2

![硬件 I²C 与软件 I²C](../../../assets/images/diagram/hardware-software-i2c.png)

## 软件结构

```text
main.c
  ├─ RTC_Init()
  ├─ OLED_Init()
  ├─ RTC_ReadTime()
  ├─ sprintf(date/time)
  └─ OLED_ShowString()

RTC.c + I2C.c   硬件 I²C 时间读取
oled.c          软件 I²C 显示输出
```

## 数据流程

`PCF8563 registers -> hardware I²C -> RTC_Time -> formatted strings -> software I²C -> OLED`

代码中可以追踪 `RTC_Time` 到 OLED 字符串的处理路径，但两个 I²C 实现都操作 P3.2/P3.3。硬件 I²C 外设启用后，OLED 位操作再次改变同一组引脚，构成资源冲突。

## 关键实现

RTC 模块负责 BCD/十进制转换，应用层使用 `sprintf()` 生成日期和时间字符串，再交给 OLED API。驱动文件虽然独立，但物理引脚没有分离；独立实现需要移动 OLED 软件 I²C 引脚，或让两个设备共享一套可寻址的 I²C 总线实现。

## 调试记录

整合过程处理了延时函数、寄存器头文件和整数类型的重复定义。当前源码仍有两项接口冲突：

- RTC 与 OLED 同时占用 P3.2/P3.3。
- `main.c` 每次启动都调用 `RTC_WriteTime()` 写入固定时间。

该工程用于分析多驱动整合与资源冲突；调整后的资源分配见 [STC8 Smart Terminal 设计](../../../docs/smart-terminal-design.md)。

## 来源说明

`course/` 保留课程 OLED 显示时间工程，包含 PCF8563、OLED 和 STC 厂商驱动代码。
