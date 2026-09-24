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

#ifndef	__TIMER_H
#define	__TIMER_H

#include	"config.h"

//========================================================================
//                             定时器0设置
//========================================================================

/* 0,1: 定时器0用做定时器, 12T/1T; 2: 定时器0用做计数器	*/
#define 	Timer0_CLK_Select(n)	do{if(n == 0) AUXR &= ~(1<<7), TMOD &= ~(1<<2); \
																if(n == 1) AUXR |= (1<<7), TMOD &= ~(1<<2); \
																if(n == 2) TMOD |= (1<<2); \
																}while(0)

#define		Timer0_CLK_Output(n)	INTCLKO = (INTCLKO & ~0x01) | (n)	/* T0 溢出脉冲在T0脚输出使能 */
#define 	Timer0_Run(n)					(n==0?(TR0 = 0):(TR0 = 1))				/* 定时器0计数使能 */
#define 	Timer0_Stop()	 				TR0 = 0				/* 禁止定时器0计数 */

#define		T0_Load(n)					TH0 = (n) / 256,	TL0 = (n) % 256
#define		T0_Load_us_1T(n)			Timer0_AsTimer(),Timer0_1T(), Timer0_16bitAutoReload(),TH0=(65536-((MAIN_Fosc/1000)*(n))/1000)/256, TL0=(65536-((MAIN_Fosc/1000)*(n))/1000)%256
#define		T0_Load_us_12T(n)			Timer0_AsTimer(),Timer0_12T(),Timer0_16bitAutoReload(),TH0=(65536-((MAIN_Fosc/12000)*(n))/1000)/256,TL0=(65536-((MAIN_Fosc/12000)*(n))/1000)%256
#define		T0_Frequency_1T_P35(n)		ET0=0,Timer0_AsTimer(),Timer0_1T(),Timer0_16bitAutoReload(),TH0=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL0=(65536-(n/2+MAIN_Fosc/2)/(n))%256,INTCLKO |= bit0,TR0=1		/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T0_Frequency_12T_P35(n)		ET0=0,Timer0_AsTimer(),Timer0_12T(),Timer0_16bitAutoReload(),TH0=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL0=(65536-(n/2+MAIN_Fosc/24)/(n))%256,INTCLKO |= bit0,TR0=1	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//========================================================================
//                             定时器1设置
//========================================================================

/* 0,1: 定时器1用做定时器, 12T/1T; 2: 定时器1用做计数器	*/
#define 	Timer1_CLK_Select(n)	do{if(n == 0) AUXR &= ~(1<<6), TMOD &= ~(1<<6); \
																if(n == 1) AUXR |= (1<<6), TMOD &= ~(1<<6); \
																if(n == 2) TMOD |= (1<<6); \
																}while(0)

#define		Timer1_CLK_Output(n)	INTCLKO = (INTCLKO & ~0x02) | (n << 1)	/* T1 溢出脉冲在T1脚输出使能 */
#define 	Timer1_Run(n)					(n==0?(TR1 = 0):(TR1 = 1))				/* 定时器1计数使能 */
#define 	Timer1_Stop()	 				TR1 = 0				/* 禁止定时器1计数 */

#define		T1_Load(n)					TH1 = (n) / 256,	TL1 = (n) % 256
#define		T1_Load_us_1T(n)			Timer1_AsTimer(),Timer1_1T(), Timer1_16bitAutoReload(),TH1=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL1=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T1_Load_us_12T(n)			Timer1_AsTimer(),Timer1_12T(),Timer1_16bitAutoReload(),TH1=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL1=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T1_Frequency_1T_P34(n)		ET1=0,Timer1_AsTimer(),Timer1_1T(),Timer1_16bitAutoReload(),TH1=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL1=(65536-(n/2+MAIN_Fosc/2)/(n))%256,INTCLKO |= bit1,TR1=1		/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T1_Frequency_12T_P34(n)		ET1=0,Timer1_AsTimer(),Timer1_12T(),Timer1_16bitAutoReload(),TH1=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL1=(65536-(n/2+MAIN_Fosc/24)/(n))%256,INTCLKO |= bit1,TR1=1	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//========================================================================
//                             定时器2设置
//========================================================================

/* 0,1: 定时器2用做定时器, 12T/1T; 2: 定时器2用做计数器	*/
#define 	Timer2_CLK_Select(n)	do{if(n == 0) AUXR &= ~(1<<2), AUXR &= ~(1<<3); \
																if(n == 1) AUXR |= (1<<2), AUXR &= ~(1<<3); \
																if(n == 2) AUXR |= (1<<3); \
																}while(0)

#define		Timer2_CLK_Output(n)	INTCLKO = (INTCLKO & ~0x04) | (n << 2)	/* T2 溢出脉冲在T2脚输出使能 */
#define 	Timer2_Run(n)					AUXR = (AUXR & ~0x10) | (n << 4)		/* 定时器2计数使能 */
#define 	Timer2_Stop()	 				AUXR &= ~(1<<4)		/* 禁止定时器2计数	*/

#define		T2_Load(n)					T2H = (n) / 256,	T2L = (n) % 256
#define		T2_Load_us_1T(n)			Timer2_AsTimer(),Timer2_1T(), T2H=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, T2L=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T2_Load_us_12T(n)			Timer2_AsTimer(),Timer2_12T(),T2H=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,T2L=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T2_Frequency_1T_P30(n)		Timer2_InterruptDisable(),Timer2_AsTimer(),Timer2_1T(), T2H=(65536-(n/2+MAIN_Fosc/2)/(n))/256, T2L=(65536-(n/2+MAIN_Fosc/2)/(n))%256, Timer2_CLKO_Enable_P30(),Timer2_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T2_Frequency_12T_P30(n)		Timer2_InterruptDisable(),Timer2_AsTimer(),Timer2_12T(),T2H=(65536-(n/2+MAIN_Fosc/24)/(n))/256,T2L=(65536-(n/2+MAIN_Fosc/24)/(n))%256,Timer2_CLKO_Enable_P30(),Timer2_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//========================================================================
//                             定时器3设置
//========================================================================

/* 0,1: 定时器3用做定时器, 12T/1T; 2: 定时器3用做计数器	*/
#define 	Timer3_CLK_Select(n)	do{if(n == 0) T4T3M &= ~(1<<1), T4T3M &= ~(1<<2); \
																if(n == 1) T4T3M |= (1<<1), T4T3M &= ~(1<<2); \
																if(n == 2) T4T3M |= (1<<2); \
																}while(0)

#define		Timer3_CLK_Output(n)	T4T3M = (T4T3M & ~0x01) | (n)				/* T3 溢出脉冲在T3脚输出使能 */
#define 	Timer3_Run(n)					T4T3M = (T4T3M & ~0x08) | (n << 3)	/* 定时器3计数使能 */
#define 	Timer3_Stop()	 				T4T3M &= ~(1<<3)	/* 禁止定时器3计数	*/

#define		T3_Load(n)					T3H = (n) / 256,	T3L = (n) % 256
#define		T3_Load_us_1T(n)			Timer3_AsTimer(),Timer3_1T(), T3H=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, T3L=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T3_Load_us_12T(n)			Timer3_AsTimer(),Timer3_12T(),T3H=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,T3L=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T3_Frequency_1T_P04(n)		Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_1T(), T3H=(65536-(n/2+MAIN_Fosc/2)/(n))/256, T3L=(65536-(n/2+MAIN_Fosc/2)/(n))%256, Timer3_CLKO_P04_Enable,Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T3_Frequency_12T_P04(n)		Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_12T(),T3H=(65536-(n/2+MAIN_Fosc/24)/(n))/256,T3L=(65536-(n/2+MAIN_Fosc/24)/(n))%256,Timer3_CLKO_P04_Enable,Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//========================================================================
//                             定时器4设置
//========================================================================

/* 0,1: 定时器4用做定时器, 12T/1T; 2: 定时器4用做计数器	*/
#define 	Timer4_CLK_Select(n)	do{if(n == 0) T4T3M &= ~(1<<5), T4T3M &= ~(1<<6); \
																if(n == 1) T4T3M |= (1<<5), T4T3M &= ~(1<<6); \
																if(n == 2) T4T3M |= (1<<6); \
																}while(0)

#define		Timer4_CLK_Output(n)	T4T3M = (T4T3M & ~0x10) | (n << 4)	/* T4 溢出脉冲在T4脚输出使能 */
#define 	Timer4_Run(n)					T4T3M = (T4T3M & ~0x80) | (n << 7)	/* 定时器4计数使能 */
#define 	Timer4_Stop()	 				T4T3M &= ~(1<<7)	/* 禁止定时器4计数	*/

#define		T4_Load(n)					T4H = (n) / 256,	T4L = (n) % 256
#define		T4_Load_us_1T(n)			Timer4_AsTimer(),Timer4_1T(), T4H=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, T4L=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T4_Load_us_12T(n)			Timer4_AsTimer(),Timer4_12T(),T4H=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,T4L=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T4_Frequency_1T_P06(n)		Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_1T(), T4H=(65536-(n/2+MAIN_Fosc/2)/(n))/256, T4L=(65536-(n/2+MAIN_Fosc/2)/(n))%256, Timer4_CLKO_P06_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T4_Frequency_12T_P06(n)		Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_12T(),T4H=(65536-(n/2+MAIN_Fosc/24)/(n))/256,T4L=(65536-(n/2+MAIN_Fosc/24)/(n))%256,Timer4_CLKO_P06_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//========================================================================
//                              定义声明
//========================================================================

#define	Timer0						0
#define	Timer1						1
#define	Timer2						2
#define	Timer3						3
#define	Timer4						4

#define	TIM_16BitAutoReload				0
#define	TIM_16Bit						1
#define	TIM_8BitAutoReload				2
#define	TIM_16BitAutoReloadNoMask		3
#define	TIM_T1Stop						3

#define	TIM_CLOCK_12T				0
#define	TIM_CLOCK_1T				1
#define	TIM_CLOCK_Ext				2

typedef struct
{
	u8	TIM_Mode;		//工作模式,  	TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	u8	TIM_ClkSource;	//时钟源		TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	u8	TIM_ClkOut;		//可编程时钟输出,	ENABLE,DISABLE
	u16	TIM_Value;		//装载初值
	u8	TIM_PS;         //8位预分频器 (注意:并非所有系列都有此寄存器,详情请查看数据手册)
	u8	TIM_Run;		//是否运行		ENABLE,DISABLE
} TIM_InitTypeDef;

u8	Timer_Inilize(u8 TIM, TIM_InitTypeDef *TIMx);

#endif
