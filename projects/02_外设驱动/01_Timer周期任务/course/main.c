#include "GPIO.h"
#include "NVIC.h"
#include "UART.h"
#include "Switch.h"
#include "Timer.h"

void GPIO_Config(){
	GPIO_InitTypeDef init;
	
	init.Mode = GPIO_OUT_PP;		//IO模式,  		GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	init.Pin  = GPIO_Pin_3;		//要设置的端口	
	
	GPIO_Inilize(GPIO_P5, &init);
	
}

void TIMER_Config(){

	TIM_InitTypeDef init;
	
	init.TIM_Mode = TIM_16BitAutoReload;		//工作模式,  	TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	init.TIM_ClkSource = TIM_CLOCK_1T;	//时钟源		TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	init.TIM_ClkOut = DISABLE;		//可编程时钟输出,	ENABLE,DISABLE
	init.TIM_Value = 65536 - (MAIN_Fosc / 1000);		//装载初值
	init.TIM_Run = ENABLE;		//是否运行		ENABLE,DISABLE
	
	Timer_Inilize(Timer0, &init);
	
	NVIC_Timer0_Init(ENABLE ,Priority_0);
	
}

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

int count = 0 ;
u8 i;


//处理定时器0的中断 :: 每隔 20ms 就去获取串口的数据，然后继续原路写回去！
void Handle_Timer0_Interrupt(){

	count++;
	
	// 20ms过去了！
	if(count == 20){
		
		
		//看看串口有数据吗？如果有，就把数据写回去
		if(COM1.RX_Cnt > 0 && --COM1.RX_TimeOut == 0 ){
		
			for(i = 0 ; i < COM1.RX_Cnt ;i++){
				TX1_write2buff(RX1_Buffer[i]);
			}
			
			COM1.RX_Cnt = 0 ;
		}
		

		count = 0;
	}
	
}


void main(){
	EA = 1 ;
	
	GPIO_Config();
	TIMER_Config();
	UART_Config();

	while(1){
		
	}
}