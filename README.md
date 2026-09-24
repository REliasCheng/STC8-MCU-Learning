# STC8 MCU Embedded Development

基于 STC8H8K64U 的增强型 8051 开发实践。仓库保留 14 个可独立阅读的 Keil C51 工程，覆盖 GPIO、Timer、UART、PWM、ADC、RTC、OLED、传感器、USB HID 和 RTX51 Tiny。

![STC8H8K64U 核心板原理图](assets/images/stc8h8k64u-core-board-schematic.png)

## 工程主线

```text
寄存器控制
   ↓
厂商外设库
   ↓
模块封装（.c / .h）
   ↓
回调接口与状态管理
   ↓
中断、缓冲与周期任务
   ↓
多外设组合
   ↓
RTX51 Tiny 任务协作
```

课程示例保存在各项目的 `course/`，没有改写原始逻辑。后续个人实现使用独立的 `practice/`，不会覆盖课程版本。

## 硬件与工具

| 项目 | 配置 |
| --- | --- |
| MCU | STC8H8K64U，增强型 8051 |
| 主频 | 工程通常配置为 24 MHz |
| 开发板 | STC8H8K64U 教学核心板及外接模块 |
| 编译 | Keil C51 / µVision |
| 下载 | STC-ISP |
| 串口 | CH340N，UART1 常用 P3.0/P3.1、115200 baud |

核心板上的 P3.0/P3.1 同时承担 UART 与 USB D-/D+ 路径，使用 HID 工程时需要切换板载开关。P5.3 连接板载 LED，P3.2 连接板载按键，P2.4/P2.5 连接板载串行存储器。

## 项目

| 分类 | 项目 | 关键实现 |
| --- | --- | --- |
| MCU 基础 | [GPIO 模块](projects/01_MCU基础/01_GPIO模块/) | P5.3、GPIO 模式、延时控制 |
| 外设驱动 | [Timer 周期任务](projects/02_外设驱动/01_Timer周期任务/) | Timer0、1 ms tick、UART 接收超时 |
| 外设驱动 | [PWM 控制](projects/02_外设驱动/02_PWM控制/) | PWM6、P0.1、串口启停电机 |
| 外设驱动 | [ADC 采样](projects/02_外设驱动/03_ADC采样/) | ADC_CH13、NTC 查表 |
| 外设驱动 | [按键事件系统](projects/02_外设驱动/04_按键事件系统/) | 状态位、按下/释放边沿、回调 |
| 外设驱动 | [数码管驱动](projects/02_外设驱动/05_数码管驱动/) | 74HC595、段码、位选、锁存 |
| 外设驱动 | [RTC 时间系统](projects/02_外设驱动/06_RTC时间系统/) | PCF8563、硬件 I²C、闹钟/定时器 |
| 外设驱动 | [OLED 显示](projects/02_外设驱动/07_OLED显示/) | SSD1306、软件 I²C、显示 API |
| 外设驱动 | [DHT11](projects/02_外设驱动/08_DHT11/) | 单总线时序、40-bit 数据、校验 |
| 外设驱动 | [EEPROM/IAP](projects/02_外设驱动/09_EEPROM_IAP/) | 片内 EEPROM 擦写与触发序列 |
| 通信接口 | [UART 通信](projects/03_通信接口/01_UART通信/) | UART1 ISR、接收缓冲、空闲超时 |
| 通信接口 | [USB HID](projects/03_通信接口/02_USB_HID/) | 矩阵键盘、8-byte HID 报告 |
| 软件设计 | [RTX51 Tiny](projects/04_软件设计/01_RTX51_Tiny/) | task、tick、signal、UART 事件 |
| 综合实践 | [RTC + OLED 时间显示](projects/05_综合实践/01_RTC_OLED时间显示/) | 硬件 I²C RTC 与软件 I²C OLED 协同 |

I²C 的工程入口位于 RTC 和综合显示项目。SPI 资料中只有厂商示例，当前未作为独立代表项目迁入。

## 目录

```text
assets/images/              原理图和课程技术图
docs/                       开发环境、硬件、工程结构和调试记录
projects/01_MCU基础/        GPIO 与端口模式
projects/02_外设驱动/       Timer、PWM、ADC、显示、传感器和存储
projects/03_通信接口/       UART 与 USB HID
projects/04_软件设计/       RTX51 Tiny 任务协作
projects/05_综合实践/       多外设组合工程
```

## 打开与构建

1. 进入某个项目的 `course/`。
2. 用 Keil µVision 打开 `.uvproj` 工程。
3. 确认目标为 STC8H8K64U 系列，系统时钟与 `Config.h` 一致。
4. 构建生成 HEX 后，通过 STC-ISP 选择正确芯片、频率和串口下载。
5. UART 与 USB HID 共用相关引脚，下载和运行前检查核心板模式开关。

迁移后的 14 个 Keil 工程共 14 个工程文件，引用路径已经检查；历史 HEX、OBJ、LST、M51 等构建产物没有纳入仓库。板端现象以今后的真实照片、串口记录和波形为准。

进一步说明见 [docs](docs/) 和各项目 README。课程及第三方来源见 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。
