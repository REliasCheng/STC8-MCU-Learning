#include "Switch.h"
#include "UART.h"
#include "NVIC.h"
#include "EEPROM.h"
#include "string.h"

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

void main(void) {

    char str[] = "Hello, World!";

    //计算出来长度
    int  len = strlen(str);

    char str2[20];

    EA = 1 ;
    UART_config();

    //1. 格式化、擦除EEPROM
    EEPROM_SectorErase(0x0000);

    //2. 写入数据到EEPROM
    //2.1 获取字符串长度
    //2.2 往EEPROM写入数据，从地址0x0000开始写入
    //参数1：写入的起始地址
    //参数2：写入的数据
    //参数3：写入的字节数
    //EEPROM_write_n(0x0000, str, len);

    //3. 从EEPROM读取数据
    //参数1：读取的起始地址
    //参数2：用什么容器来装数据，写这个容器的首地址
    //参数3：读取的字节数
    //EEPROM_read_n(0x0000, str2, len);

    printf("data6666=%s\n", str2);

    while (1){
        
    }
    
}