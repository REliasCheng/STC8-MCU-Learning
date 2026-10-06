# Portable UART RX Core

当前默认分支提供一个仓库维护者独立编写的 C89 环形缓冲区，用于隔离 UART RX 中断入口与主循环数据处理。该模块不依赖 STC8 寄存器、厂商头文件、动态内存或第三方库。

## Design Boundary

```text
UART RX ISR                         Main Loop
    │                                  │
    └── ring_buffer_push_isr()         └── ring_buffer_pop()
                  │                    │
                  └──── RingBuffer ────┘
```

| State | Ownership |
| --- | --- |
| `head` | Producer：UART RX ISR writes；consumer reads |
| `tail` | Consumer：main loop writes；producer reads |
| `overflow` | ISR sets the latch；main loop reads and clears it |

`head`、`tail` 和 `overflow` 使用 `volatile unsigned char`，适合 8-bit 目标上的单字节访问假设。`ring_buffer_take_overflow()` 是读取后清除操作；目标集成时若 UART RX ISR 可能同时置位，应在调用端用短临界区保护该操作。`ring_buffer_reset()` 也只能在初始化阶段或生产者暂停时调用。

## Concurrency Contract

- UART RX ISR 是唯一 producer，只调用 `ring_buffer_push_isr()` 并只修改 `head`。
- 主循环是唯一 consumer，只调用 `ring_buffer_pop()` 并只修改 `tail`。
- `ring_buffer_take_overflow()` 的读取与清零不是相对于 ISR 的原子操作；调用端必须在整个调用期间暂停 producer。
- `ring_buffer_reset()` 同时改变 producer 与 consumer 状态，只能在 UART RX 中断关闭、producer 尚未启动或已经停止时调用。
- `volatile` 用于保留跨 ISR/主循环的实际字节访问，不替代临界区，也不提供通用线程同步语义。

STC8 UART1 集成层通过短暂关闭 UART1 中断使错误读取和缓冲区复位满足以上契约；可移植核心本身不依赖任何平台级中断控制。

## Capacity Model

模块使用 one-slot-empty 规则区分空与满：存储区长度为 `N` 时，可用容量为 `N - 1`。`capacity` 的有效范围是 2 到 255；满缓冲区上的 push 返回 `RING_BUFFER_FULL`、置位溢出锁存，并保留所有尚未消费的数据。

## Public API

| API | Responsibility |
| --- | --- |
| `ring_buffer_init()` | 绑定静态存储并清零索引与锁存状态 |
| `ring_buffer_reset()` | 仅复位索引与锁存，不清空存储区 |
| `ring_buffer_push_isr()` | ISR 侧 O(1) 写入一个字节 |
| `ring_buffer_pop()` | 主循环侧取出一个字节 |
| `ring_buffer_available()` | 返回当前可读字节数 |
| `ring_buffer_is_empty()` | 查询空状态 |
| `ring_buffer_is_full()` | 查询满状态 |
| `ring_buffer_take_overflow()` | 读取并清除溢出锁存 |

公开函数检查空指针和未初始化对象。状态返回值明确区分 `OK`、`EMPTY`、`FULL` 与 `INVALID_ARGUMENT`。

## Host Verification

GCC 与 Clang 使用同一组严格 C89 选项：

```sh
gcc -std=c89 -Wall -Wextra -Werror -pedantic \
  -Iinclude src/core/ring_buffer.c tests/test_ring_buffer.c \
  -o ring_buffer_tests
./ring_buffer_tests
```

将首个命令的 `gcc` 替换为 `clang` 可执行另一矩阵项。测试仅使用仓库内的小型断言工具，覆盖 25 项行为和边界条件。

## Target Integration Boundary

该模块没有配置 UART、没有提供 Keil 工程，也没有声明 STC8 构建或板端运行成功。目标集成仍需补充 UART ISR 适配、临界区策略、内存模型核对、Keil C51 编译记录和实际硬件证据。
