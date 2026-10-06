# STC8-MCU-Learning

面向 STC8H8K64U 的增强型 8051 固件架构实验，包含一个可独立验证的 UART RX 可移植核心。

**⚙️ Peripheral Driver Progression**

![STC8 firmware learning map](assets/images/architecture/portfolio-overview.svg)

## Firmware Snapshot

| Firmware Focus | Current Scope |
| --- | --- |
| Repository Type | Architecture Lab with Original Portable Core |
| Reference MCU | STC8H8K64U，enhanced 8051 |
| Architecture | ISR Byte Input → Portable Ring Buffer → Main-loop Consumer |
| Documentation | GPIO、Timer、UART、I²C、ADC、PWM、RTC、display and resource planning |
| Public Implementation | Partial：C89 UART RX ring buffer core |
| Verification | Host GCC build and 25 behavior tests PASS；target build and hardware evidence not provided |

## 📌 Overview

仓库以文档方式整理 STC8 固件从寄存器控制、外设驱动、模块封装到事件协作的结构，并记录引脚复用、定时器占用和通信接口之间的资源约束。当前公开实现聚焦一个无动态内存、C89 兼容的单生产者/单消费者环形缓冲区，用于隔离 UART RX ISR 与主循环处理逻辑。

当前默认分支不分发课程工程、STC 厂商源码、启动文件、USB/HID 示例、RTX51 库或来源不明图片。可移植核心的主机测试不能替代 STC8 目标构建、Keil 构建或板端运行证据。

## 🏗️ Architecture

```text
UART RX ISR (producer)
          ↓ push byte
Portable Ring Buffer
          ↓ pop byte
Main-loop Consumer
          ↓
Parser / Application State
```

生产者只更新 `head`，消费者只更新 `tail`；缓冲区满时不覆盖未消费数据，并置位溢出锁存标志。该核心不包含 UART 寄存器配置、协议解析或 STC8 工程集成。

## ✨ Key Features

| Capability | Documentation Entry |
| --- | --- |
| Portable UART RX core | [Portable UART RX Core](docs/portable-uart-core.md) |
| Board resource planning | [Core Board and Pin Multiplexing](docs/核心板与引脚复用.md) |
| Firmware structure progression | [Engineering Structure Evolution](docs/工程结构演进.md) |
| Toolchain boundary | [Development Environment and Build](docs/开发环境与构建.md) |
| Integration planning | [Smart Terminal Design](docs/smart-terminal-design.md) |
| Evidence boundary | [Debug Notes](docs/调试记录.md) |

## 📂 Project Structure

```text
STC8-MCU-Learning/
├── .github/workflows/            # GCC and Clang host-test matrix
├── include/ring_buffer.h         # Portable public API
├── src/core/ring_buffer.c        # C89 ring buffer implementation
├── tests/test_ring_buffer.c      # Behavior-focused host tests
├── README.md
├── LICENSE
├── THIRD_PARTY_NOTICES.md
├── assets/images/architecture/  # Repository-authored SVG
└── docs/                        # Core, architecture, and integration documentation
```

## 📚 Documentation

- [Documentation Index](docs/README.md)
- [Portable UART RX Core](docs/portable-uart-core.md)
- [Core Board and Pin Multiplexing](docs/核心板与引脚复用.md)
- [Engineering Structure Evolution](docs/工程结构演进.md)
- [Development Environment and Build](docs/开发环境与构建.md)
- [Debug Notes](docs/调试记录.md)
- [Smart Terminal Design](docs/smart-terminal-design.md)

## 🧪 Verification

| Verification Layer | Status | Boundary |
| --- | --- | --- |
| Portable Core Host Build | PASS | GCC 16.1.0 with `-std=c89 -Wall -Wextra -Werror -pedantic` |
| Host Test | PASS | 25 ring buffer behavior and boundary tests on the portable core |
| Host CI | AUTOMATED | GitHub Actions compiles and runs the same tests with GCC and Clang |
| STC8 / Keil Build Verification | NOT PROVIDED | No target project or reproducible Keil build evidence in the current branch |
| Hardware Validation | NOT PROVIDED | No reviewable STC8 board test record |
| Runtime Evidence | NOT PROVIDED | No serial log, USB enumeration record, or measurement data |

## License Boundary

根目录 MIT License 仅覆盖当前默认分支中仓库维护者编写的可移植核心、测试、文档、配置与自绘 SVG。课程源码、STC 厂商组件、启动文件、外部图片和二进制库未包含在当前默认分支；历史提交中的旧文件仍需单独评估。详见 [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md)。
