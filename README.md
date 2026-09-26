# STC8 MCU Embedded Development

## 👋 项目简介 | Overview

基于 STC8H8K64U 的增强型 8051 工程实践，包含 GPIO、Timer、UART、PWM、ADC、RTC、OLED、传感器、USB HID 和 RTX51 Tiny。14 个 Keil C51 工程按硬件接口和软件结构分类，可直接从各项目的 `course/` 打开。

![STC8H8K64U 核心板原理图](assets/images/hardware/stc8h8k64u-core-board-schematic.png)

## 🧠 系统架构 | Architecture

后期工程逐步将应用流程、功能模块和底层外设分开：

```text
Application
  页面状态 / 命令处理 / 数据格式化 / 任务入口
                         ↓
Modules
  Key / RTC / OLED / DHT11 / EEPROM / UART buffer
                         ↓
Drivers
  GPIO / Timer / UART / I²C / ADC / PWM / USB
                         ↓
STC8 Hardware
  STC8H8K64U / PCF8563 / SSD1306 / 74HC595 / CH340N
```

代码组织从端口寄存器控制逐步扩展到厂商库、独立模块、事件回调、中断缓冲、多外设组合和 RTX51 Tiny 任务协作。

```text
寄存器控制 → 外设驱动 → 模块封装 → 事件管理 → 多外设系统 → 任务调度
```

| UART 帧结构 | 硬件与软件 I²C 路径 |
| --- | --- |
| ![UART 帧结构](assets/images/diagram/uart-frame.png) | ![硬件与软件 I²C 路径](assets/images/diagram/hardware-software-i2c.png) |

## 🚀 核心项目 | Featured Projects

| 项目 | 实现 |
| --- | --- |
| [GPIO 模块](projects/01_MCU基础/01_GPIO模块/) | P5.3 输出、GPIO 模式和端口寄存器 |
| [按键事件系统](projects/02_外设驱动/04_按键事件系统/) | 四键状态位、按下/释放边沿和函数指针回调 |
| [RTC 时间系统](projects/02_外设驱动/06_RTC时间系统/) | PCF8563、硬件 I²C、BCD 转换和中断标志 |
| [OLED 显示](projects/02_外设驱动/07_OLED显示/) | SSD1306、软件 I²C、字符与位图接口 |
| [UART 通信](projects/03_通信接口/01_UART通信/) | UART1 ISR、128-byte 接收缓冲和空闲超时 |
| [RTX51 Tiny](projects/04_软件设计/01_RTX51_Tiny/) | task、tick、signal 和 UART 事件协作 |
| [RTC + OLED 整合](projects/05_综合实践/01_RTC_OLED时间显示/) | 时间读取、格式化显示及 I²C 资源冲突分析 |

## ⚙ 外设示例 | Peripheral Examples

| 项目 | 实现 |
| --- | --- |
| [Timer 周期任务](projects/02_外设驱动/01_Timer周期任务/) | Timer0 1 ms tick、20 ms 串口处理周期 |
| [PWM 控制](projects/02_外设驱动/02_PWM控制/) | PWM6、P0.1 和串口启停控制 |
| [ADC 采样](projects/02_外设驱动/03_ADC采样/) | P0.5/ADC_CH13、NTC 查表换算 |
| [74HC595 数码管](projects/02_外设驱动/05_数码管驱动/) | 串行移位、段选/位选和锁存更新 |
| [DHT11](projects/02_外设驱动/08_DHT11/) | 单总线时序、40-bit 数据和校验 |
| [EEPROM/IAP](projects/02_外设驱动/09_EEPROM_IAP/) | 片内 IAP 擦除、写入和读取接口 |
| [USB HID](projects/03_通信接口/02_USB_HID/) | 4×4 矩阵键盘和 8-byte HID 报告 |

## 🛠 硬件与工具 | Hardware and Tools

| 项目 | 配置 |
| --- | --- |
| MCU | STC8H8K64U，工程通常配置为 24 MHz |
| 开发板 | STC8H8K64U 教学核心板及外接模块 |
| 编译 | Keil C51 / µVision |
| 下载 | STC-ISP |
| 串口 | CH340N，UART1 使用 P3.0/P3.1，常用 115200 baud |

P3.0/P3.1 通过板载开关在 UART 与 USB D-/D+ 路径之间切换。P5.3 连接板载 LED，P3.2 连接板载按键，P2.4/P2.5 连接板载串行存储器；组合工程需要先核对引脚复用和定时器占用。

## 📂 工程结构 | Repository Structure

```text
projects/01_MCU基础/   GPIO 与寄存器入口
projects/02_外设驱动/ Timer、PWM、ADC、按键、显示、RTC、传感器与 IAP
projects/03_通信接口/ UART 与 USB HID
projects/04_软件设计/ RTX51 Tiny 任务协作
projects/05_综合实践/ RTC 与 OLED 组合工程
docs/                 架构、引脚复用、构建和调试说明
assets/images/        原理图、数据流和工程结构图
```

## 🔧 构建与下载 | Build / Run

1. 进入项目的 `course/`，使用 Keil µVision 打开 `.uvproj`。
2. 确认目标器件、`Config.h` 主频和 C51 Include Paths。
3. 构建 HEX，通过 STC-ISP 选择对应芯片、IRC 频率和串口下载。
4. UART 或 USB HID 运行前，切换核心板上的通信模式开关。

仓库不跟踪 HEX、OBJ、LST、M51 等日常构建产物。

## 📖 技术文档 | Documentation

- [文档索引](docs/README.md)
- [核心板与引脚复用](docs/核心板与引脚复用.md)
- [工程结构演进](docs/工程结构演进.md)
- [开发环境与构建](docs/开发环境与构建.md)
- [调试记录](docs/调试记录.md)
- [Smart Terminal 资源设计](docs/smart-terminal-design.md)
- [来源与许可](THIRD_PARTY_NOTICES.md)

## 🔗 相关项目 | Related Projects

- [C51-Board-Lab](https://github.com/REliasCheng/C51-Board-Lab)：51 开天开发板硬件资源与板级实验。
- [stc89c52-learning](https://github.com/REliasCheng/stc89c52-learning)：STC89C52RC 外设驱动与多外设应用。
- [BlueBridgeCup-MCU](https://github.com/REliasCheng/BlueBridgeCup-MCU)：CT107D 资源分配与竞赛综合工程。
