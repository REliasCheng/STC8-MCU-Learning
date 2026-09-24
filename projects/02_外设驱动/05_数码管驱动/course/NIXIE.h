#ifndef __NIXIE_H
#define __NIXIE_H

#include "GPIO.h"

#define NIX_DI  P44     // 数据
#define NIX_SCK P42   // 移位
#define NIX_RCK P43   // 锁存

//1. 初始化数码管
void NIXIE_Init();

//2. 显示数码管
// dat: 要显示的数字
// dig: 要显示的数码管
void NIXIE_Show(u8 dat , u8 dig);


//3. 显示数字
// num: 要显示的数字 :: 这里不用传对应的字节，只需要传递显示的字
// dig: 要显示的数码管
void NIXIE_Show_Number(u8 num , u8 dig);

#endif 