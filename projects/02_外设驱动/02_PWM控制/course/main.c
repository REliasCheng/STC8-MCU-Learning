
#include "GPIO.h"
#include "Delay.h"
#include "NVIC.h"
#include "UART.h"
#include "STC8H_PWM.h"
#include "Switch.h"


#define MOTOR  P01
#define PERIOD (MAIN_Fosc / 1000)

void GPIO_Config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_1;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
}


void PWM_Config(){
	PWMx_InitDefine init;
	
	init.PWM_Mode = CCMRn_PWM_MODE1;			//模式,   CCMRn_FREEZE,CCMRn_MATCH_VALID,CCMRn_MATCH_INVALID,CCMRn_ROLLOVER,CCMRn_FORCE_INVALID,CCMRn_FORCE_VALID,CCMRn_PWM_MODE1,CCMRn_PWM_MODE2
	init.PWM_Period = PERIOD - 1;		//周期时间,   0~65535
	init.PWM_Duty = 0;			//占空比时间, 0~Period
	init.PWM_DeadTime = 0 ;	//死区发生器设置, 0~255
	init.PWM_EnoSelect = ENO6P;		//输出通道选择,	ENO1P,ENO1N,ENO2P,ENO2N,ENO3P,ENO3N,ENO4P,ENO4N / ENO5P,ENO6P,ENO7P,ENO8P
	init.PWM_CEN_Enable = ENABLE;		//使能计数器, ENABLE,DISABLE
	init.PWM_MainOutEnable = ENABLE;//主输出使能,  ENABLE,DISABLE
	
	PWM_Configuration(PWM6, &init);
	PWM_Configuration(PWMB, &init);
	
	PWM6_SW(PWM6_SW_P01);
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

void stop(){
	
	PWMx_InitDefine init;
	init.PWM_CEN_Enable = DISABLE;		//使能计数器, ENABLE,DISABLE
	init.PWM_MainOutEnable = DISABLE;//主输出使能,  ENABLE,DISABLE
	
	PWM_Configuration(PWM6, &init);
	PWM_Configuration(PWMB, &init);
}


u8 isRun = 1;

void main(){
	PWMx_Duty duty;
	u8 percent = 0 ;
	char direction = 1;
	
	//打开外部扩展寄存器使能
	EA = 1;
	EAXSFR();
	
	GPIO_Config();
	PWM_Config();
	UART_Config();

	while(1){
		
		// ======================== 串口 =================================
		if(COM1.RX_Cnt >0 && --COM1.RX_TimeOut == 0){
			TX1_write2buff(RX1_Buffer[0]);
			//判断串口发过来的消息是什么
			if(RX1_Buffer[0] == 0x00){ //停止震动
				 isRun = 0;
				 stop();
			}else {  // 开始震动
				PWM_Config();
				
				isRun = 1;
			}
			
			COM1.RX_Cnt = 0;
		}
		
		
		
		
		//Keil + VS Code
		
		//Keil + Trae 
		
		// ======================== PWM =================================
		if(isRun){
			percent += direction;
			
			if(percent >= 100){
				direction = -1;
			}else if(percent <= 0){
				direction = 1;
			}
			
			
			duty.PWM6_Duty = PERIOD * percent / 100;
			UpdatePwm(PWM6, &duty);
		}
		
		
		delay_ms(20);
	}
}