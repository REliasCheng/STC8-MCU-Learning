RTC 整合 OLED屏幕：

1. 拷贝进来 oled.c  oled.h , bmp.h  oledfont.h
2. 编译报错：*** ERROR L104: MULTIPLE PUBLIC DEFINITIONS      SYMBOL:  _DELAY_MS
	2.1 有多个地方都出现了 delay_ms的函数定义, dleay.c oled.c
	2.2 解决办法：
		a. 删掉其中一个即可
		b. 我选择直接把Delay.c直接移除了，不要了
		
	2.3 在main.c 里面引入 oled.h， 移除 #include "Delay.h"
	
3. 编译还报错：很多个重复定义！：C:\Keil_v5\C51\Inc\REG51.h(13): error C231: 'P0': redefinition

	3.1 在oled.h 上面还导入了 REG51.h 它和我们使用的STC8H.h冲突了。
	3.2 直接把oled.h 上面的 REG51.h,修改为 STC8H.h
	
4. 编译可能还会出错：  u8 u16 u32的重复定义
	#define  u8 unsigned char 
	#define  u16 unsigned int
	#define  u32 unsigned int
	
	4.1 在oled.h 里面有定义，并且在type_def.h 里面也有定义
	