# STC8 UART Integration

该集成层把 STC8H8K64U UART1 接收中断连接到可移植环形缓冲区，同时把硬件寄存器代码与可由 GCC/Clang 验证的适配器合同分离。范围仅包括 UART1 RX；不包含 TX queue、parser、command dispatcher 或应用协议。

## Target and References

| Field | Value |
| --- | --- |
| Target MCU | STC8H8K64U |
| UART instance | UART1 |
| C51 device header | Toolchain-provided `STC/STC8H.H`；not stored in this repository |
| Target build | NOT PROVIDED |
| Board pin routing | NOT VERIFIED |

实现依据是 STC 官方 [STC8H8K64U product page](https://www.stcmicro.com/stc/stc8h8k64u.html) 与 [STC8H Series Manual](https://www.stcmicro.com/datasheet/STC8H-en.pdf)。官方资料定义了 UART1 的 `SCON`、`SBUF`、`ES`、`S1ST2`、Timer2 波特率路径和 `P_SW1.S1_S` 引脚选择。

## Architecture

```text
STC8H8K64U UART1 RX interrupt
              ↓ read SBUF, clear RI
stc8_uart_rx_handle_byte_isr()
              ↓
ring_buffer_push_isr()
              ↓
Main-loop ring_buffer_pop()
```

真实 ISR 位于 `stc8_uart_c51.c`，使用 Keil C51 的 `interrupt 4` 语法。Host CI 不编译该文件；`stc8_uart.c` 中的字节转发、挂载、错误锁存和受控复位逻辑由独立 Host Test 验证。

## UART1 Configuration Boundary

`Stc8Uart1Config` 要求调用者明确提供系统时钟、波特率和 UART1 引脚路由。当前实现选择 UART mode 1 与 Timer2 1T 波特率发生器，reload 计算为 `65536 - FOSC / 4 / baud`；无效或超出 16-bit Timer2 范围的配置会被拒绝。

| Route | RXD | TXD |
| --- | --- | --- |
| `STC8_UART1_ROUTE_P30_P31` | P3.0 | P3.1 |
| `STC8_UART1_ROUTE_P36_P37` | P3.6 | P3.7 |
| `STC8_UART1_ROUTE_P16_P17` | P1.6 | P1.7 |
| `STC8_UART1_ROUTE_P43_P44` | P4.3 | P4.4 |

这些是芯片支持的选择，不表示当前目标板已经确认使用其中任何一组。必须依据实际原理图、封装和板载切换状态确定配置。`stc8_uart1_start()` 只启用 UART1 中断 `ES`，不会替应用启用全局中断 `EA`。

## Lifecycle and Ownership

1. 在 UART1 RX 中断关闭时初始化 `RingBuffer`。
2. 调用 `stc8_uart_rx_attach_buffer()` 挂载唯一静态缓冲区。
3. 调用 `stc8_uart1_init()` 配置 UART1 与 Timer2。
4. 由应用按系统启动顺序管理全局中断，再调用 `stc8_uart1_start()` 启用 UART1 中断。
5. ISR 是唯一 producer；主循环是唯一 consumer。
6. detach、error take 和 reset 都必须在 producer 暂停时执行。

`stc8_uart1_take_error()` 与 `stc8_uart1_reset_rx_buffer()` 会保存 `ES`、关闭 UART1 中断、完成操作后恢复原状态，从而满足 portable core 的读后清零与 reset 契约。

## Overflow and Fault Handling

- 未挂载缓冲区时，ISR 读取并清除硬件接收状态，丢弃字节，并锁存 `STC8_UART_NOT_ATTACHED`。
- 环形缓冲区满时，不覆盖旧数据，并锁存 `STC8_UART_RX_OVERFLOW`。
- ISR 中不执行日志、字符串处理、协议解析、延时或动态分配。
- `stc8_uart_rx_take_error_quiesced()` 是 Host 可测的底层读后清零接口，只允许在 producer 已暂停时调用。

## Verification Boundary

| Evidence | Status |
| --- | --- |
| Existing ring buffer tests | PASS：25 cases |
| Adapter contract tests | PASS：12 cases |
| GCC strict C89 host build | PASS |
| Clang strict C89 host build | Verified by GitHub Actions |
| C51-specific source review | Completed against documented register contract |
| Keil C51 target compile/link | NOT PROVIDED |
| HEX generation | NOT PROVIDED |
| Hardware UART validation | NOT PROVIDED |
| Serial or logic-analyzer evidence | NOT PROVIDED |

本阶段检测到本机存在 C51 工具文件，但其安装来源与许可状态未建立可公开复现的证据，因此未把该环境用于 Target Build 声明，也未提交 `.uvprojx`、启动文件或厂商头文件。
