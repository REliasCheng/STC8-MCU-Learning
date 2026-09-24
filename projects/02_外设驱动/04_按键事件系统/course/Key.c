#include "Key.h"
#include "GPIO.h"

//定义一个字节的大小，用来存储按键的状态值 
u8 states = 0xFF; // 1111 1111

void KEY_Init(void){
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);//初始化
}

// 定义一个函数，用来返回按键的值
u8 Get_Key(u8 i){
    if(i ==0 ) return KEY1;
    else if(i ==1) return KEY2;
    else if(i ==2) return KEY3;
    else if(i ==3) return KEY4;
    return KEY1;
 }

 extern void key_up(u8 index);
 extern void key_down(u8 index);


u8 i;
void KEY_Scan( void (*down)(u8) , void (*up)(u8)){

    for (i = 0; i < 4; i++){
        if(Get_Key(i) == UP && IS_KEY_DOWN(states , i)){ //弹起
            SET_KEY_UP(states , i);
            
            if(down != NULL)
                down(i);
        }else if(Get_Key(i) == DOWN  && IS_KEY_UP(states , i)) { //按下
            SET_KEY_DOWN(states ,i);  
            
            if(up != NULL)     
                up(i);
        }
    }
  
}