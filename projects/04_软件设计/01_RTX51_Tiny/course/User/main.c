

/*
	1. 报错： can't open file 'GPIO.h'
		由于采用目录的方式来组织代码了，所以这些头文件可能会位于不同的目录中，默认找头文件只会在工程的根目录找，
	所以需要在小魔法棒里面配置include path.
	2. *** ERROR L104: MULTIPLE PUBLIC DEFINITIONS       SYMBOL:  MAIN
		2.1 main入口被重复定义了！ 
		2.2 由于现在使用的是操作系统，启动就由操作系统的入口作为启动入口了，我们不能再提供main函数了。
	3. 我们就需要定义出来任务，以任务的方式来组织代码，实现需求。	
	4. 当系统启动成功之后，会自动执行 0 号 任务,其他的任务不会自己启动。
	5. 只要使用了RTX51系统，那么就不能再写以前的main函数了，否则会出现冲突，并且我们还需要提供一个0号任务。
		
		
*/
#include "RTX51TNY.h"  
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"

void GPIO_Config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_PP;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5, &GPIO_InitStructure);//初始化

	P53 = 0 ;
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

// 主任务..
// 1. 完成初始化：：配置
// 2. 负责启动其他的任务
void task_main() _task_ 0 {

	EA = 1 ; 
	
	//1. 初始化
	GPIO_Config();
	UART_Config();
	
	//2. 启动任务
	os_create_task(1);
	os_create_task(2);
	
	//3. 干掉自己
	os_delete_task(0);
	
}

/*
	需求： 有两个任务：A任务亮灯， B任务获取串口数据。
		1. LED灯亮灭交替，交替3次之后，让它进去等待。

		2. 如果串口任务收到了0x01, 则让串口任务发送一个信号出来，让A任务继续往下走！
*/

int count = 0 ;

//LED任务
void task_led1() _task_ 1 {
	while(1){
		P53 = ~P53;
		os_wait2(K_TMO, 200);
		
		//没执行一次，都记录count，如果count 到了4，即表示亮2次，熄灭2次，让它进入等待信号的状态。
		count++;
		if(count == 4){
			
			//不能往下执行了，要等到信号出现才能往下继续执行。
			os_wait1 (K_SIG);
			count = 0 ;
		}
	}
}


//串口任务
void task_uart() _task_ 2 {
	while(1){
		
		if(COM1.RX_Cnt >0 && --COM1.RX_TimeOut == 0 ){
			
			//如果串口发送过来的是 0x01 则在这里发送信号
			if(RX1_Buffer[0] == 0x01){
				
					//给1号任务发信号
					os_send_signal(1);
			}			
			COM1.RX_Cnt = 0;
		}
	}
}


