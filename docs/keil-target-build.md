# STC8H8K64U Keil C51 Target Build Preparation

本文记录当前默认分支建立可复现 Keil C51 目标构建所需的配置、依赖与证据边界。它不是一次构建通过记录：本次审查只能确认本机存在经过数字签名的 Keil 工具文件，不能确认其安装来源、当前使用授权或 STC8 器件支持文件的来源，因此没有调用 C51、BL51/LX51 或 OH51，也没有生成 HEX。

## Current Status

| Item | Status |
| --- | --- |
| Source compatibility review | Completed without changing source |
| Keil C51 target compile | NOT PROVIDED |
| Link | NOT PROVIDED |
| HEX generation | NOT PROVIDED |
| Hardware validation | NOT PROVIDED |
| Runtime evidence | NOT PROVIDED |

只有在合法来源、使用权和器件支持均可验证的 Keil C51 环境中重新执行本页步骤，才可以把目标构建状态改为 `PASS`。

## Target Configuration Boundary

| Field | Current value |
| --- | --- |
| Target MCU | STC8H8K64U |
| Package | NOT PROVIDED |
| System clock | NOT PROVIDED |
| UART1 baud | NOT PROVIDED |
| UART1 route | NOT PROVIDED |
| Board route validation | NOT PROVIDED |
| Memory model | NOT SELECTED；`SMALL` 仅作为最小 C51 应用的候选起点 |
| Startup source | Toolchain-provided when a verified environment is available；not vendored |
| Device header | External toolchain dependency：`<STC/STC8H.H>`；not vendored |

Package、时钟和 UART1 引脚必须依据真实目标板、原理图与已确认的时钟配置填写。芯片支持某个 UART1 route 不等于目标板已经连接或验证该 route。

## Repository Source List

目标工程只应包含仓库维护者编写的以下源码：

- `src/core/ring_buffer.c`
- `platform/stc8/stc8_uart.c`
- `platform/stc8/stc8_uart_c51.c`
- 一个在合法工具链确认后重新编写的最小 target application（当前未创建）

Include 路径至少包含：

- `include/`
- `platform/stc8/`

外部依赖限于合法工具链提供的 STC8H8K64U device header、C51 runtime 和必要 startup。不得从旧提交恢复课程工程、`STARTUP.A51`、厂商源码、RTX51 文件或历史 `main.c`。

## C51 Compatibility Review

当前 `platform/stc8/stc8_uart_c51.c` 使用：

- Keil C51 `interrupt 4` 语法声明 UART1 ISR；
- `P_SW1` 选择 UART1 芯片级 pin route；
- `SCON` / `SBUF` / `ES` / `RI` / `TI` 管理 UART1；
- `AUXR` / `T2H` / `T2L` 配置 Timer2 1T 波特率发生器。

这些符号与本次只读检查到的外部 `STC8H.H` 以及 STC 官方 STC8H 文档的寄存器命名一致，但尚未经过可信环境中的 C51 编译。源码不得仅凭推测修改；后续改动必须对应官方资料或真实编译诊断。

Keil C51 的 generic pointer 通常比 memory-specific pointer 更大：官方 C51 资料将 generic pointer 描述为 3 bytes，而 `data` pointer 为 1 byte、`xdata` pointer 为 2 bytes。当前 `RingBuffer.storage` 的 `unsigned char *` 因此需要在真实构建中检查 DATA/CODE 成本及目标 storage 的可寻址性。本阶段不在缺少构建指标时引入 memory qualifier，也不改变 portable API。

## Baud Calculation

当前 UART1 adapter 按 Timer2 1T 配置计算：

```text
timer_ticks = floor(FOSC / 4 / requested_baud)
reload      = 65536 - timer_ticks
actual_baud = FOSC / 4 / timer_ticks
error_pct   = (actual_baud - requested_baud) / requested_baud * 100
```

实际配置确认后必须记录：

| Field | Required evidence |
| --- | --- |
| `SYSTEM_CLOCK_HZ` | NOT PROVIDED |
| `REQUESTED_BAUD` | NOT PROVIDED |
| `TIMER2_TICKS` | NOT PROVIDED |
| `TIMER2_RELOAD` | NOT PROVIDED |
| `ACTUAL_BAUD` | NOT PROVIDED |
| `BAUD_ERROR_PERCENT` | NOT PROVIDED |

不得只检查 reload 是否落入 16-bit；还要根据整数分频结果判断波特率误差是否适合真实 UART 链路。

## Reproducible Build Procedure

以下步骤只能在来源和使用权可验证、且包含来源明确 STC8H8K64U 支持文件的 Keil C51 环境中执行：

1. 记录 C51、BL51/LX51 与 OH51 的实际版本，不记录 license serial、machine ID 或私人路径。
2. 新建 STC8H8K64U target，不使用旧课程工程或旧提交中的工程文件。
3. 明确 package、系统时钟、UART1 baud、芯片级 route 与板级 route 证据。
4. 选择并记录 C51 memory model；`SMALL` 可作为最小应用的初始候选，但必须以真实配置和内存报告为准。
5. 添加本页列出的仓库源码和 include 路径；device header 与 startup 保持为工具链外部依赖。
6. 新建最小原创 target application：静态分配 RX storage，初始化并 attach ring buffer，配置 UART1，由 application 明确管理全局中断 `EA`，主循环仅消费 RX byte。
7. 编译并保留 errors、warnings、CODE、DATA 与 XDATA 结构化结果；不以关闭 warning 的方式隐藏问题。
8. 链接并生成 HEX；记录文件名、大小和 SHA-256，但默认不提交生成物。
9. 清理目标输出后重新构建，确认 clean rebuild 成功，并比较两次 HEX hash。
10. 重新运行 25 个 ring buffer tests 与 12 个 adapter contract tests，并确认 GitHub Actions 的 GCC/Clang jobs 通过。

## Evidence Required Before PASS

目标构建只有同时具备以下证据时才能标记为 `PASS`：

- 可验证的工具链来源、使用权与 device support 来源；
- 明确的 target、package、memory model、clock、baud 与 route；
- C51 compile 成功；
- BL51/LX51 link 成功；
- warning/error 和 CODE/DATA/XDATA 数值；
- HEX 真实生成及其 hash；
- clean rebuild 成功；
- 现有 37 个 Host Tests 与远端 CI 保持通过。

即使以上全部通过，也只能证明 Target Build，不代表 firmware download、目标板 UART route、Hardware Validation 或 Runtime Evidence。

## Artifact and Security Policy

不要提交 `OBJ`、`LST`、`M51`、`MAP`、HEX、build directory、license 信息、用户名或本机绝对路径。若后续需要保存证据，只提交去除私人信息的纯文本摘要。专有 Keil 工具链不得上传到 GitHub Actions。

## Official References

- [STC8H Series Technical Reference Manual](https://www.stcmicro.com/datasheet/STC8H-en.pdf)
- [STC8H8K64U product page](https://www.stcmicro.com/stc/stc8h8k64u.html)
- [Keil C51 target options](https://www.keil.com/support/man/docs/uv4cl/uv4cl_dg_target51.htm)
- [Keil C51 product manuals](https://www.keil.com/support/man/)
- [Keil C51 pointer and memory-space overview](https://www.keil.com/product/brochures/c51_v6.pdf)
- [Keil C51 interrupt function syntax](https://www.keil.com/support/docs/1217.htm)
- [Keil OH51 object-to-HEX converter](https://www.keil.com/support/index/oh51.htm)
