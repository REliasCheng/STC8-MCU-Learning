/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序            */
/*---------------------------------------------------------------------*/

#include "UART.h"

//========================================================================
// 函数: UART1_ISR_Handler
// 描述: UART1中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
#ifdef UART1


/*
	只要串口1发生中断就会调用这个函数 ----> 由系统自己调用，由芯片自己调用...
*/
void UART1_ISR_Handler (void) interrupt UART1_VECTOR
{
	
	//如果是串口发生了接收中断，那么就执行这个if的代码
	if(RI)
	{
		//进来了之后，要手动置零（软件清零）
		RI = 0;
			
				/*
					1. COM1.RX_Cnt >= COM_RX1_Lenth 如果不断的转移数据到数组，已经把数组装满了！ 就重置 COM1.RX_Cnt = 0 
				*/
        if(COM1.RX_Cnt >= COM_RX1_Lenth)	COM1.RX_Cnt = 0;
		
				/*
					1. RX1_Buffer 是一个u8类型的，长度为128个字节的数组，
					2. SBUF 是接收和发送数据的缓冲区
					3. 这句话的目的就是把SBUF里面的数据转移到数组里面去，防止后面过来的数据覆盖了SBUF原来的数据。
					4. COM1.RX_Cnt++ 目的是为了让下标进行偏移，把数据装到不同的位置去，并且COM1.RX_Cnt 也可以认为是串口用来描述
							到底接受了多少个字节的数据..
				*/
        RX1_Buffer[COM1.RX_Cnt++] = SBUF;
		
				/*
					1. 只要转移了1个字节，就重置COM1.RX_TimeOut的值为5
					2. 一般在处理不定长度的时候，我们可以以此来作为接收结束的评定标准。
						2.1 我们后面在获取串口数据的时候，除了判定串口有数据之外，还顺便做倒数
					  2.2 让这个 COM1.RX_TimeOut  --  ，如果能顺利倒数到0，即表示数据发送方已经把数据发送完毕了。
					
				*/
        COM1.RX_TimeOut = TimeOutSet1;
	}

	//如果是串口发生了发送中断，那么就执行这个if的代码
	if(TI)
	{
		TI = 0;
		
        #if(UART_QUEUE_MODE == 1)   //判断是否使用队列模式
		if(COM1.TX_send != COM1.TX_write)
		{
		 	SBUF = TX1_Buffer[COM1.TX_send];
			if(++COM1.TX_send >= COM_TX1_Lenth)		COM1.TX_send = 0;
		}
		else	COM1.B_TX_busy = 0;
        #else
        COM1.B_TX_busy = 0;     //使用阻塞方式发送直接清除繁忙标志
        #endif
	}
}
#endif

//========================================================================
// 函数: UART2_ISR_Handler
// 描述: UART2中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
#ifdef UART2
void UART2_ISR_Handler (void) interrupt UART2_VECTOR
{
	if(RI2)
	{
		CLR_RI2();

		
        if(COM2.RX_Cnt >= COM_RX2_Lenth)	COM2.RX_Cnt = 0;
        RX2_Buffer[COM2.RX_Cnt++] = S2BUF;
        COM2.RX_TimeOut = TimeOutSet2;
	}

	if(TI2)
	{
		CLR_TI2();
		
        #if(UART_QUEUE_MODE == 1)   //判断是否使用队列模式
		if(COM2.TX_send != COM2.TX_write)
		{
		 	S2BUF = TX2_Buffer[COM2.TX_send];
			if(++COM2.TX_send >= COM_TX2_Lenth)		COM2.TX_send = 0;
		}
		else	COM2.B_TX_busy = 0;
        #else
        COM2.B_TX_busy = 0;     //使用阻塞方式发送直接清除繁忙标志
        #endif
	}
}
#endif

//========================================================================
// 函数: UART3_ISR_Handler
// 描述: UART3中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
#ifdef UART3
void UART3_ISR_Handler (void) interrupt UART3_VECTOR
{
	if(RI3)
	{
		CLR_RI3();

        if(COM3.RX_Cnt >= COM_RX3_Lenth)	COM3.RX_Cnt = 0;
        RX3_Buffer[COM3.RX_Cnt++] = S3BUF;
        COM3.RX_TimeOut = TimeOutSet3;
	}

	if(TI3)
	{
		CLR_TI3();
		
        #if(UART_QUEUE_MODE == 1)   //判断是否使用队列模式
		if(COM3.TX_send != COM3.TX_write)
		{
		 	S3BUF = TX3_Buffer[COM3.TX_send];
			if(++COM3.TX_send >= COM_TX3_Lenth)		COM3.TX_send = 0;
		}
		else	COM3.B_TX_busy = 0;
        #else
        COM3.B_TX_busy = 0;     //使用阻塞方式发送直接清除繁忙标志
        #endif
	}
}
#endif

//========================================================================
// 函数: UART4_ISR_Handler
// 描述: UART4中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2020-09-23
//========================================================================
#ifdef UART4
void UART4_ISR_Handler (void) interrupt UART4_VECTOR
{
	if(RI4)
	{
		CLR_RI4();

        if(COM4.RX_Cnt >= COM_RX4_Lenth)	COM4.RX_Cnt = 0;
        RX4_Buffer[COM4.RX_Cnt++] = S4BUF;
        COM4.RX_TimeOut = TimeOutSet4;
	}

	if(TI4)
	{
		CLR_TI4();
		
        #if(UART_QUEUE_MODE == 1)   //判断是否使用队列模式
		if(COM4.TX_send != COM4.TX_write)
		{
		 	S4BUF = TX4_Buffer[COM4.TX_send];
			if(++COM4.TX_send >= COM_TX4_Lenth)		COM4.TX_send = 0;
		}
		else	COM4.B_TX_busy = 0;
        #else
        COM4.B_TX_busy = 0;     //使用阻塞方式发送直接清除繁忙标志
        #endif
	}
}
#endif
