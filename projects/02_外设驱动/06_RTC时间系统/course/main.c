#include "Delay.h"
#include "NVIC.h"
#include "Switch.h"		
#include "UART.h"
#include "RTC.h"
#include "I2C.h"
#include "Exti.h"


void UART_Config(void) {
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

//处理闹钟和定时器中断
void handle_alarm_timer(){

	if( RTC_IS_ALARM_INT()){ // 闹钟

		printf("Alarm!\n");
		RTC_Alarm_Clear_Flag(); // 清除标志位

	}
	
	if(RTC_IS_TIMER_INT()){ //定时器

		printf("Timer!\n");
		RTC_Timer_Clear_Flag(); // 清除标志位

	}
}


void main(){
	u8 dat;
	RTC_Time time = {55, 29, 12, 13, 2, 5, 2025};

	EA = 1;
	UART_Config();
	printf("start...\n");
	// 初始化RTC
	RTC_Init();


	// ========================= 写入时间 ========================================
	RTC_WriteTime(&time);

	// ========================= 配置闹钟 ========================================
	//RTC_Alarm_On(30 ,12 ,13 ,5);

	// ========================= 配置定时器 ========================================
	//RTC_Timer_On( HZ_1 ,  1  ); 

	while(1){
	
	
		/*
		//  ========================= 读取时间 ========================================
		RTC_ReadTime(&time);

		printf("%02d-%02d-%02d %02d:%02d:%02d\n",time.year,(int)time.month,(int)time.day, (int)time.hour,(int)time.minute,(int)time.second);
		printf("week:%d\n",(int)time.week); // 0 , 248
		printf("----------22-------------\n");

		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);*/
	}
}