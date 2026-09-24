
#include "NVIC.h"
#include "Switch.h"		
#include "UART.h"
#include "RTC.h"
#include "I2C.h"
#include "oled.h"
#include "string.h"



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

	//拼接时间
	//2020-02-03
	char time1[11];
	//11:30:55
	char time2[9];


void main(){
	RTC_Time time = {57, 29, 12, 13, 2, 5, 2025};


	EA = 1;
	UART_Config();
	printf("start...\n");

	// 初始化RTC
	RTC_Init();
	
	//配置OLED屏幕
	OLED_Init();//初始化OLED
	OLED_ColorTurn(0);//0正常显示【黑底白字】，1 反色显示【白底黑字】
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

	// 写入时间
	RTC_WriteTime(&time);

	
	while(1){
	
	
		// 读取时间
		RTC_ReadTime(&time);
		
		//printf() ： 把数据按照一定的格式输出到串口上
		//sprintf() : 把数据按照一定的格式输出到字符串上

	

		//参数一：拼接出来的字符串用什么来收 , 参数二：拼接的格式, 参数三：需要拼接的数据
		sprintf(time1 , "%d-%02d-%02d" , time.year, (int)time.month, (int)time.day);
		time1[10] = '\0';

		sprintf(time2 , "%02d:%02d:%02d" , (int)time.hour, (int)time.minute, (int)time.second);
		time2[8] = '\0';

		//printf("time1.len=%d\n", strlen(time1));
		//printf("time2.len=%d\n", strlen(time2));

		//printf("time1=%s\n", time1);
		//printf("time2=%s\n", time2);

		// 输出到OLED
		OLED_ShowString(0,0,time1,16);
		OLED_ShowString(0,2,time2,16);

		//输出到串口
		//printf("%02d-%02d-%02d %02d:%02d:%02d\n",time.year,(int)time.month,(int)time.day, (int)time.hour,(int)time.minute,(int)time.second);
		//printf("week:%d\n",(int)time.week); // 0 , 248
		//printf("----------22-------------\n");

		delay_ms(20);
	}
}