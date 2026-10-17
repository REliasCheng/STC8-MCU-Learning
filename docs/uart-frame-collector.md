# Portable UART Frame Collector

这个 C89 模块在主循环中把接收字节组合成以调用者指定字节分隔的帧。它不读取寄存器、不定义命令格式，也不执行串口发送。与现有 `RingBuffer` 配合时，ISR 仍只负责入队，主循环取字节后调用 `uart_frame_feed()`。

## 使用合同

1. `uart_frame_init(&collector, delimiter)` 指定分隔字节。帧是二进制字节序列，不要求 NUL 结尾；分隔字节本身不包含在帧内。
2. 每次 `uart_frame_feed()` 返回 `UART_FRAME_READY` 时，`frame` 指向内部存储，`length` 是帧长度。调用者必须在下一次 feed 或 init 前使用或复制该数据。空帧长度为零。
3. 单帧最多 64 字节。超过上限后丢弃该帧剩余字节，到下一处分隔符时返回一次 `UART_FRAME_DROPPED`，随后恢复收集。
4. 如果 UART 队列报告字节丢失，主循环调用 `uart_frame_note_loss()`。当前部分帧和后续字节会被保守丢弃，直到下一处分隔符。调用者应在正确的临界区内读取 UART 错误锁存；本模块自身不管理中断。
5. 该对象仅由主循环使用，不应在 ISR 和主循环之间共享可变状态。

调用者可选择 `\n` 用于文本行，也可选择其他分隔字节；本模块不假定设备协议、字符编码或命令语义。

## Host 验证

```sh
gcc -std=c89 -Wall -Wextra -Werror -pedantic \
  -Iinclude src/core/ring_buffer.c src/core/uart_frame.c tests/test_uart_frame.c \
  -o uart_frame_tests
./uart_frame_tests
```

测试覆盖连续帧、空帧、二进制零值、恰好达到容量、超长帧恢复、传输丢字后的恢复、无效参数，以及实际 `RingBuffer` 到帧收集器的字节流。GitHub Actions 使用 GCC 与 Clang 执行同一组测试。Host 测试不证明 Keil C51 构建、UART 时序或板端运行。
