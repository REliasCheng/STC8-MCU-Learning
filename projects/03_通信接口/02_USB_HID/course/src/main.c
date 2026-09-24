#include "stc.h"
#include "usb.h"
#include "GPIO.h"
#include "NVIC.h"
#include "Switch.h"
#include "UART.h"
#include "Delay.h"
#include "MatrixKey.h"
#include "usb_req_class.h"



void UART_config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}
void up(u8 row , u8 col){
	//printf("%d-%d :: up\r\n" , (int)row + 1 ,  (int)col + 1);
	u8 dat[8]= {0};
	usb_class_in(dat); // 发送数据，dat是一个数组，里面存放的是要发送的数
}

void down(u8 row , u8 col){
	//printf("%d-%d :: down\r\n" , (int)row + 1 ,  (int)col + 1);

	//发送一个简单的按键给PC
	u8 dat[8]= {0};
	

	//第0个字节放：功能组合键，第1个字节保留不用，只能从第2个字节位置开始放普通键位
	dat[2]= 0x14; // 发送Q给PC

	//调用函数发送数据
	usb_class_in(dat); // 发送数据，dat是一个数组，里面存放的是要发送的数
}

//=============================

void main()
{
	
	//矩阵键盘
	EAXSFR();
	EA = 1 ;
	MK_Init();
	UART_config();

	//USB的初始化
    usb_init();

    while (1){
		MK_Scan02(down, up);
		delay_ms(20);
	}
}
