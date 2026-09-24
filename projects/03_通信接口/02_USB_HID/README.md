# USB HID 键盘

## 项目简介

将 4×4 矩阵键盘的按下/释放事件转换为 8-byte USB HID 键盘报告。课程示例按下任意已处理键位时发送 HID key code `0x14`，释放时发送全零报告。

## 硬件环境

- MCU：STC8H8K64U，24 MHz IRC
- USB：板载 Type-C 与 MCU USB 外设
- 输入：4×4 矩阵键盘
- 板卡模式：开关切换到 USB/HID 路径

## 软件结构

```text
main.c
  ├─ MK_Init()
  ├─ usb_init()
  ├─ MK_Scan02(down, up)
  └─ usb_class_in(report)

MatrixKey.c/.h  行列扫描和状态位图
src/usb*.c      USB 枚举、请求和端点处理
src/usb_desc.c  HID 描述符
```

## 数据流程

`row drive -> column sample -> key edge -> 8-byte HID report -> USB IN endpoint -> host`

## 关键实现

HID 报告第 0 字节用于修饰键，第 1 字节保留，第 2-7 字节放普通键码。按键释放必须发送全零报告，否则主机会保持按键按下状态。

## 调试记录

USB 枚举需要 24 MHz 配置、正确描述符和板卡模式开关。整合矩阵键盘时课程记录了 `u8/u16/u32`、`STC8H.h` 和 `config.h` 重复定义，需要明确公共类型和配置文件的唯一来源。

## 来源说明

`course/` 包含课程矩阵键盘代码与 STC USB HID 示例组件，第三方 USB 协议源码不由本仓库重新授权。
