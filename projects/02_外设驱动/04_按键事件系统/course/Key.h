
#ifndef __KEY_H__
#define __KEY_H__

//0. 在头文件的内容，都是我们希望公开给别人用的！
#define KEY1 P51
#define KEY2 P52
#define KEY3 P53
#define KEY4 P54

#define UP   1
#define DOWN 0
#define IS_KEY_DOWN(s , offset)          (s & (1 <<offset)) == 0
#define IS_KEY_UP(s , offset)            (s & (1 <<offset)) != 0
#define SET_KEY_DOWN(s , offset)         s &= ~(1<<offset)
#define SET_KEY_UP(s, offset)            s |= 1 <<offset


//1. 这个模块、功能的配置   （GPIO） | （PWM），配置这个模块的时候，用到了哪些配置，就配哪些内容
void KEY_Init(void);

//2. 这个模块、功能的对外接口(功能、函数)  （GPIO） | （PWM），对外提供哪些接口，就写哪些接口函数


//    函数名：KEY_Scan
//    函数功能：扫描按键
//    函数参数：
//        void (*down)(u8)：按键按下的回调函数
//        void (*up)(u8)：按键弹起的回调函数
//    函数返回值：无
void KEY_Scan( void (*down)(u8) , void (*up)(u8));


#endif // __KEY_H__