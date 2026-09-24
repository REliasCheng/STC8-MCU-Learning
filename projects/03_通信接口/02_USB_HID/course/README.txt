1. 整合HID键盘和矩阵键盘代码

	1.1 拷贝进来之后编译直接报错：
		 'u8': redefinition  、  u16 、 u32 都由问题
		 
	1.2 解决步骤：
		
		a. 把src目录中的STC8H.h 给删除掉，不用它的。
		
		b. 把src目录中的config.h 改名为：usb_config
		
		c. 在stc.h头文件里面修改引入 #include "config.h" ---> #include "usb_config.h"
		
		d. 把stc.h 里面的关于u8 、u16、u32的定义注释掉，因为我们拷贝进来的Config里面已经有了。
			
			//typedef unsigned char u8;
			//typedef unsigned int u16;
			//typedef unsigned long u32;
	
	1.3 继续编译，不会报错了！但是会报警告： type_def.h(39): warning C317: attempt to redefine macro 'NULL'
		a. 注释掉 stc.h 里面 //#include <stdio.h>