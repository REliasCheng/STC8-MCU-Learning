# OLED 显示

## 项目简介

使用软件 I²C 驱动 0.96 英寸 OLED，提供初始化、清屏、字符、数字、字符串、汉字和位图显示接口。

## 硬件环境

- MCU：STC8H8K64U
- 显示控制器：SSD1306 类 OLED 模块
- 接口：软件 I²C

## 软件结构

```text
main.c
  ├─ OLED_Init()
  ├─ OLED_Clear()
  └─ OLED_ShowString()/OLED_ShowNum()

SRC/oled.c      I²C 时序与显示 API
SRC/oledfont.h  字模
SRC/bmp.h       位图数据
```

## 数据流程

`text/number/bitmap -> display API -> page/column address -> software I²C -> OLED RAM`

## 关键实现

`OLED_WR_Byte()` 区分命令和数据，`OLED_Set_Pos()` 设置页与列地址。字符、汉字和位图接口最终都转换为连续显存字节。

## 调试记录

课程后续整合中记录了 `delay_ms`、`REG51.h` 和 `u8/u16/u32` 的重复定义问题。合并驱动时应保留一套寄存器定义、类型定义和延时实现。

## 来源说明

`course/` 保留课程 OLED API 工程及其厂商驱动文件；字库和底层代码不声明为个人原创。
