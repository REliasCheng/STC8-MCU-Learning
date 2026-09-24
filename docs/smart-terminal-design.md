# STC8 Smart Terminal 设计

这份设计把仓库中已经存在的 RTC、DHT11、OLED、按键、UART 和片内 EEPROM 接口组织成一个多页面信息终端。当前完成的是接口核对、资源分配和任务设计；仓库中还没有对应的 Keil 工程或板端运行记录。

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

应用层只维护当前页面、时间、温湿度、设置项和刷新标志。各服务调用已经存在的驱动接口，不直接操作外设寄存器。

## 现有接口映射

| 服务 | 已有接口 | 原工程 | 计划资源 |
| --- | --- | --- | --- |
| ClockService | `RTC_Init()`、`RTC_ReadTime()`、`RTC_WriteTime()` | [RTC 时间系统](../projects/02_外设驱动/06_RTC时间系统/) | PCF8563，硬件 I²C P3.2/P3.3 |
| SensorService | `DHT11_Init()`、`DHT11_GetTempAndHumidity()` | [DHT11](../projects/02_外设驱动/08_DHT11/) | P4.6，1 s 采样周期 |
| InputService | `KEY_Init()`、`KEY_Scan()` | [按键事件系统](../projects/02_外设驱动/04_按键事件系统/) | P5.1/P5.2/P5.4，避开板载 LED 的 P5.3 |
| SettingsService | `EEPROM_SectorErase()`、`EEPROM_write_n()`、`EEPROM_read_n()` | [EEPROM/IAP](../projects/02_外设驱动/09_EEPROM_IAP/) | 片内 IAP，无外部引脚 |
| CommandService | `UART_Configuration()`、`RX1_Buffer` | [UART 通信](../projects/03_通信接口/01_UART通信/) | UART1 P3.0/P3.1，Timer1，115200 baud |
| UIService | `OLED_Init()`、`OLED_ShowString()`、`OLED_ShowNum()` | [OLED 显示](../projects/02_外设驱动/07_OLED显示/) | 软件 I²C 改用 P1.4/P1.5，RESET P1.2 |
| Scheduler | `Timer_Inilize()`、`Handle_Timer0_Interrupt()` | [Timer 周期任务](../projects/02_外设驱动/01_Timer周期任务/) | Timer0，1 ms tick |

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
| 1 ms | Timer0 tick，仅更新计数和任务标志 |
| 10 ms | 扫描按键并产生按下/释放事件 |
| 20 ms | 检查 UART 接收空闲超时，提交完整命令 |
| 200 ms | 刷新发生变化的 OLED 页面 |
| 1 s | 读取 RTC 和 DHT11，更新状态 |
| 配置变化后 | 延迟写入 EEPROM，而不是每次循环写入 |

## 页面与事件

```text
PAGE_CLOCK       时间与日期
PAGE_ENVIRONMENT 温度与湿度
PAGE_SETTINGS    页面切换、显示选项和时间设置
```

短按键用于切换页面和选择设置项；UART 提供读取状态、切换页面和更新时间的命令入口。RTC 写入只发生在用户确认时间设置时，不在每次启动时覆盖当前时间。

## 资源冲突

1. 课程 `RTC + OLED` 工程同时把硬件 I²C 和 OLED 软件 I²C 映射到 P3.2/P3.3。终端设计保留 RTC 的硬件 I²C，并把 OLED 软件 I²C 移到 P1.4/P1.5。
2. P3.2 还连接板载按键，使用 PCF8563 时不再把该按键作为应用输入。
3. P5.3 连接板载 LED，独立按键只选用 P5.1、P5.2、P5.4，或在个人工程中重新分配引脚。
4. UART1 使用 Timer1 生成波特率；Timer0 留给系统 tick，避免两个功能争用同一定时器。
5. 课程 EEPROM 头文件把 `MCU_Type` 配置为 `STC8X1K08`。建立 STC8H8K64U 工程前必须依据目标芯片手册重新确认 IAP 地址和保留区域。

## 实现检查点

- 建立独立 `practice/` 工程并保留原课程项目不变。
- 修正按键回调的按下/释放语义后再接入事件层。
- 为 OLED 分配新的软件 I²C 引脚并核对外接连线。
- 确认 STC8H8K64U 的 IAP 区域、扇区大小和掉电保存策略。
- 通过 Keil 构建后，再进行 RTC 走时、DHT11 时序、UART 命令和 EEPROM 读回测试。

在这些检查点完成前，本页只描述可实施的系统设计，不代表已经完成板端联调。
