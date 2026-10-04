# STC8 Smart Terminal 资源设计

这份设计把 RTC、DHT11、OLED、按键、UART 和片内 EEPROM 接口映射到多页面信息终端，定义应用状态、资源分配、任务周期和板级集成检查点。仓库当前保存的是资源设计，未包含对应的集成工程源码。

## 系统结构

```text
Application
  AppState / PageState / Settings
             │
             ├── ClockService
             ├── SensorService
             ├── InputService
             ├── SettingsService
             ├── CommandService
             └── UIService
                       │
Drivers
  RTC / DHT11 / Key / EEPROM / UART / OLED / Timer0
                       │
Hardware
  PCF8563 / DHT11 / Keys / STC8 IAP / CH340N / SSD1306
```

应用层只维护当前页面、时间、温湿度、设置项和刷新标志。下表是设计接口与资源预算，不表示这些接口已在当前默认分支实现。

## 设计接口映射

| 服务 | 设计职责 | 资源预算 |
| --- | --- | --- |
| ClockService | RTC 初始化、读取与设置接口 | PCF8563 与一个 I²C 控制器或软件 I²C |
| SensorService | 温湿度采样接口 | 一个 GPIO 与周期采样时隙 |
| InputService | 按键扫描与事件生成 | 独立 GPIO，避开显示与通信引脚 |
| SettingsService | 参数持久化接口 | 片内 IAP 区域，地址需按目标芯片确认 |
| CommandService | UART 接收、分帧与命令解析 | UART、波特率定时资源与接收缓冲区 |
| UIService | 页面渲染与差量刷新 | 显示总线、RESET 引脚与显存预算 |
| Scheduler | 周期 tick 与任务标志 | 一个 Timer，中断只维护短路径状态 |

## 数据流

```text
PCF8563 ──> ClockService ──┐
DHT11 ────> SensorService ─┼──> AppState ──> UIService ──> OLED
Keys ─────> InputService ──┤
UART RX ──> CommandService ┘
                              └──> SettingsService ──> IAP EEPROM
```

- RTC 和 DHT11 更新应用数据，UI只在数据或页面变化时刷新。
- 按键边沿和 UART 命令转换为应用事件，不在 ISR 中修改显示。
- 设置变化先更新 RAM，经过延时确认后再写 EEPROM，避免按键连发造成重复擦写。
- UART ISR 只收集字节，命令解析在主循环任务中执行。

## 周期任务

| 周期 | 任务 |
| --- | --- |
| 1 ms（设计值） | Timer tick，仅更新计数和任务标志 |
| 10 ms（设计值） | 扫描按键并产生按下/释放事件 |
| 20 ms（设计值） | 检查 UART 接收空闲超时，提交完整命令 |
| 200 ms（设计值） | 刷新发生变化的显示页面 |
| 1 s（设计值） | 读取 RTC 和环境数据，更新状态 |
| 配置变化后 | 延迟写入 EEPROM，而不是每次循环写入 |

## 页面与事件

```text
PAGE_CLOCK       时间与日期
PAGE_ENVIRONMENT 温度与湿度
PAGE_SETTINGS    页面切换、显示选项和时间设置
```

短按键用于切换页面和选择设置项；UART 提供读取状态、切换页面和更新时间的命令入口。RTC 写入只发生在用户确认时间设置时，不在每次启动时覆盖当前时间。

## 资源冲突

1. RTC 与显示接口不能在未经核对时复用同一组 I²C 或 GPIO 资源。
2. 板载按键、LED 与外接模块可能共享引脚，集成前必须对照实际原理图。
3. UART 波特率发生器与系统 tick 不应争用同一定时器。
4. IAP 地址、扇区大小和保留区域必须以目标芯片手册与实际链接布局为准。

## 集成约束

- 按键回调接入事件层前需核对按下/释放参数语义。
- 显示接口需要分配独立总线或软件 I²C 引脚，并与实际连线一致。
- IAP 接口需要按 STC8H8K64U 手册核对地址、扇区大小和保留区域。
- 集成工程需分别记录 RTC 走时、DHT11 时序、UART 命令和 EEPROM 读回结果。
