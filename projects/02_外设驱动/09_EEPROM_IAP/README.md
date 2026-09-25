# 片内 EEPROM / IAP

## 项目简介

封装 STC8 片内 EEPROM 的扇区擦除、连续写入和连续读取接口，展示 IAP 地址、命令和触发序列。

## 硬件环境

- MCU：STC8H8K64U
- 存储：片内 IAP/EEPROM 区域
- 调试输出：UART1，115200 baud

## 软件结构

```text
main.c
  ├─ EEPROM_SectorErase()
  ├─ EEPROM_write_n()
  └─ EEPROM_read_n()

EEPROM.c/.h
  ├─ IAP enable/address/command
  ├─ 0x5A / 0xA5 trigger
  └─ operation cleanup
```

## 数据流程

`RAM buffer -> IAP address/command -> trigger -> EEPROM`，读取时反向返回到 RAM buffer。

## 关键实现

IAP 操作期间保存并关闭总中断，按顺序写入 0x5A、0xA5 触发命令，结束后关闭 IAP 并恢复中断状态。

## 调试记录

当前 `main.c` 注释了写入和读取调用，却继续打印未初始化的 `str2`。`practice/` 修正版需要启用擦写流程、初始化缓冲区，并用读回数据校验写入结果。

## 来源说明

`course/` 原目录名为 `EEPORM`，仓库主题名统一为 EEPROM，但课程文件内容没有修改。
