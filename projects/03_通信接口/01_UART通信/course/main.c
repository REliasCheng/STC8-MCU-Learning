#include "UART.h"
#include "GPIO.h"
#include "Delay.h"
#include "Switch.h"
#include "NVIC.h"

void GPIO_Config(){
	GPIO_InitTypeDef init;
	
	init.Mode = GPIO_PullUp;		//IO模式,  		GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	init.Pin  = GPIO_Pin_0 | GPIO_Pin_1;		//要设置的端口	
	
	GPIO_Inilize(GPIO_P3, &init);
}

void UART_Config(){
	
	//1. 创建结构体变量
	COMx_InitDefine init;
	
	//1.1 给结构体成员赋值 
	init.UART_Mode       =  UART_8bit_BRTx  ;		//模式,         UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	init.UART_BRT_Use    =  BRT_Timer1  ;		//波特率发生器,   BRT_Timer1,BRT_Timer2,BRT_Timer3,BRT_Timer4
	init.UART_BaudRate	 =  115200  ;		//波特率, 	   一般 110 ~ 115200
	init.UART_RxEnable   =  ENABLE  ;		//允许接收,   ENABLE,DISABLE
	init.BaudRateDouble  =  DISABLE  ;		//波特率加倍, ENABLE,DISABLE
	
	//2. 配置
	UART_Configuration(UART1, &init);
	
	//3. 配置中断优先级... :: 允许中断，并且配置了中断优先级 0
	NVIC_UART1_Init(ENABLE, Priority_0);
	
	//4. 配置 (切换) 引脚
	UART1_SW(UART1_SW_P30_P31);
}


void main(){
  u8 i ;
	
	//0. 允许全局中断
	EA = 1 ;
	
	
	//1. 配置GPIO
	GPIO_Config();
	
	//2. 配置串口
	UART_Config();
	
	

	while(1){
	
		
		//1. 先判断串口有数据吗？并且如果有数据，还要倒数，从5倒数到0，再去拿数据
		if(COM1.RX_Cnt > 0 && --COM1.RX_TimeOut == 0 ){
			
			
			//2. 就可以操作RX1_Buffer这个数组了。
			for(i = 0 ; i < COM1.RX_Cnt ; i++){
				
				//3. 可以取出来每一个数据
				u8 dat = RX1_Buffer[i];
				
				//4. 继续把数据写回去
				TX1_write2buff(dat);
			}
			
			//5. 当我们已经把数据拿走了之后，就要重置这个RX_Cnt = 0 
			COM1.RX_Cnt  = 0 ;
		}
		
		
		delay_ms(20);
		
	}
	
}