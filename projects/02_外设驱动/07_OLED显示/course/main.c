//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL  P10（SCL）
//              SDA  P11（SDA）
//              RES  P12 注：SPI接口显示屏改成IIC接口时需要接RES引脚
//                           IIC接口显示屏用户请忽略
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"


sfr  P3M0 = 0xB2;
sfr  P3M1 = 0xB1;

int main(void)
{	
	u8 t=' ';

	//配置3.2 和 3.3 IO模式
	P3M0 &= ~0x0c; P3M1 &= ~0x0c; 


	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示【黑底白字】，1 反色显示【白底黑字】
   OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	while(1) 
	{	
		//1. 显示数字
		OLED_ShowNum(0,0,2025,4,16);

		//2. 显示英文
		OLED_ShowString(0,2,"HelloWorld",16);

		//3. 显示中文
		OLED_ShowChinese(0 * 18,4,0,16);//黑
		OLED_ShowChinese(1 * 18,4,1,16);//马
		OLED_ShowChinese(2 * 18,4,2,16);//程
		OLED_ShowChinese(3 * 18,4,3,16);//序
		OLED_ShowChinese(4 * 18,4,4,16);//员

		//清屏然后在新的屏显示
		delay_ms(500);
		OLED_Clear();

		//4. 显示图片
		OLED_DrawBMP(0,0,128,64,BMP2);
		delay_ms(500);
		OLED_Clear();

		
		/*
		OLED_DrawBMP(0,0,128,64,BMP1);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//中
		OLED_ShowChinese(18,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(54,0,3,16);//电
		OLED_ShowChinese(72,0,4,16);//子
		OLED_ShowChinese(90,0,5,16);//科
		OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(8,2,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,4,"2014/05/01",16);
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16);
		OLED_ShowChar(48,6,t,16);
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);
		delay_ms(500);
		OLED_Clear();
		*/
	}	  
	
}

	