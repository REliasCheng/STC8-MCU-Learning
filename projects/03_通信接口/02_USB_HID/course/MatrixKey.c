#include "MatrixKey.h"


// 矩阵键盘初始化
void MK_Init(){
    GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_3|GPIO_Pin_6|GPIO_Pin_7;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_7;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_4|GPIO_Pin_5;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_0|GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4, &GPIO_InitStructure);//初始化
}

//获取列的状态
u8 Get_Key(u8 i){
	if(i==0) return COL1;
	else if(i==1) return COL2;
	else if(i==2) return COL3;
	else if(i==3) return COL4;
	return COL1;
}

//设置行的状态
void Set_Row(u8 i){
	ROW1 = i == 0 ? 0 : 1;
	ROW2 = i == 1 ? 0 : 1;
	ROW3 = i == 2 ? 0 : 1;
	ROW4 = i == 3 ? 0 : 1;
}

u16 states = 0xFFFF ; // 1111 1111 1111 1111


void MK_Scan02( void(*downFun)(u8 , u8 ) , void (*upFun)(u8 , u8 )){
    u8 i , j ;
    //1.外层循环 处理的是行
    for (i = 0; i < 4; i++){
                
        //每循环一次，就设置对应的这一行的状态
        Set_Row(i);
        
        //2.内层循环 处理的是列
        for (j = 0; j < 4; j++){
            if(Get_Key(j) == 0 && ( (states & (1<< (4 * i + j))) != 0 ) ){
                states  &=  ~(1<<4*i+j);

                downFun(i , j );
            }else if(Get_Key(j) == 1 && ( (states & (1<< (4 * i + j))) == 0 )){
                states |= (1<<4*i+j);

                upFun( i , j );
            }
        }
    }
}
