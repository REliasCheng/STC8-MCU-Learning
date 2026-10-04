# STC8-MCU-Learning

面向 STC8H8K64U 的增强型 8051 固件架构与板级资源规划文档实验。

**⚙️ Peripheral Driver Progression**

![STC8 firmware learning map](assets/images/architecture/portfolio-overview.svg)

## Firmware Snapshot

| Firmware Focus | Current Scope |
| --- | --- |
| Repository Type | Architecture / Firmware Integration Lab |
| Reference MCU | STC8H8K64U，enhanced 8051 |
| Architecture | Register → Driver → Module → Event → Integration |
| Documentation | GPIO、Timer、UART、I²C、ADC、PWM、RTC、display and resource planning |
| Public Implementation | Not included in the current default branch |
| Verification | Architecture review；build and hardware evidence not provided |

## 📌 Overview

仓库以文档方式整理 STC8 固件从寄存器控制、外设驱动、模块封装到事件协作的结构，并记录引脚复用、定时器占用和通信接口之间的资源约束。

当前默认分支不再分发课程工程、STC 厂商源码、启动文件、USB/HID 示例、RTX51 库或来源不明图片。文档描述技术边界和集成方法，不表示对应固件已经公开实现、成功构建或完成板端验证。

## 🏗️ Architecture

```text
Application State / Command Handling
                  ↓
Modules and Event Boundaries
                  ↓
GPIO / Timer / UART / I²C / ADC / PWM
                  ↓
STC8 Board Resources
```

组合不同模块前，需要重新核对 GPIO alternate function、Timer channel、UART buffer、I²C address 与中断资源。

## ✨ Key Features

| Capability | Documentation Entry |
| --- | --- |
| Board resource planning | [Core Board and Pin Multiplexing](docs/核心板与引脚复用.md) |
| Firmware structure progression | [Engineering Structure Evolution](docs/工程结构演进.md) |
| Toolchain boundary | [Development Environment and Build](docs/开发环境与构建.md) |
| Integration planning | [Smart Terminal Design](docs/smart-terminal-design.md) |
| Evidence boundary | [Debug Notes](docs/调试记录.md) |

## 📂 Project Structure

```text
STC8-MCU-Learning/
├── README.md
├── LICENSE
├── THIRD_PARTY_NOTICES.md
├── assets/images/architecture/  # Repository-authored SVG
└── docs/                        # Architecture and integration documentation
```

## 📚 Documentation

- [Documentation Index](docs/README.md)
- [Core Board and Pin Multiplexing](docs/核心板与引脚复用.md)
- [Engineering Structure Evolution](docs/工程结构演进.md)
- [Development Environment and Build](docs/开发环境与构建.md)
- [Debug Notes](docs/调试记录.md)
- [Smart Terminal Design](docs/smart-terminal-design.md)

## 🧪 Verification

| Verification Layer | Status | Boundary |
| --- | --- | --- |
| Host Test | NOT APPLICABLE | Current branch contains no host-test implementation |
| Build Verification | NOT PROVIDED | No reproducible Keil build evidence for the current branch |
| Hardware Validation | NOT PROVIDED | No reviewable STC8 board test record |
| Runtime Evidence | NOT PROVIDED | No serial log, USB enumeration record, or measurement data |

## License Boundary

根目录 MIT License 仅覆盖当前默认分支中仓库维护者编写的文档、配置与自绘 SVG。课程源码、STC 厂商组件、启动文件、外部图片和二进制库未包含在当前默认分支；历史提交中的旧文件仍需单独评估。详见 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。
