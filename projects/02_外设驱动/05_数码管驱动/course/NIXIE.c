#include "NIXIE.h"


//1. 初始化数码管
void NIXIE_Init(){
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
}

//用一个数组来存储数码管能够显示的字对应的二进制
u8 code LED_TABLE[] = 
{
	// 0 	1	 2	-> 9	(索引012...9)
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,
	// 0. 1. 2. -> 9.	(索引10,11,12....19)
    0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,
	// . -						(索引20,21)
	0x7F, 0xBF,
	// AbCdEFHJLPqU		(索引22,23,24....33)
	0x88,0x83,0xC6,0xA1,0x86,0x8E,0x89,0xF1,0xC7,0x8C,0x98,0xC1
};


char i ;
//2. 显示数码管
void NIXIE_Show(u8 dat , u8 dig){

    //2. 先处理显示什么字
    for(i=7; i>=0; i--){
                
        //先从高位开始拿数据 :: 不需要右移了，因为超过1的数字，也会被识别成1。
        NIX_DI = dat & (1<<i);

        //需要产生移位的上升沿信号
        NIX_SCK = 0;
        NOP1();
        NIX_SCK = 1;
        NOP1();
    }


    //3.再处理哪个数码管显示
    for ( i=7; i>=0; i--){
        NIX_DI = dig & (1 << i);

        //需要产生移位的上升沿信号
        NIX_SCK = 0;
        NOP1();
        NIX_SCK = 1;
        NOP1();
    }

    //4. 产生锁存信号 [上升沿]
    NIX_RCK = 0;
    NOP1();
    NIX_RCK = 1;
    NOP1();

}


//3. 显示数字
void NIXIE_Show_Number(u8 num , u8 dig){

    //调用显示数码管的函数
    NIXIE_Show( LED_TABLE[num], dig);
}