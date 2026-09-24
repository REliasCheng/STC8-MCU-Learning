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

#include	"NVIC.h"

//========================================================================
// 函数: NVIC_Timer0_Init
// 描述: Timer0嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_Timer0_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	Timer0_Interrupt(State);
	Timer0_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_Timer1_Init
// 描述: Timer1嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_Timer1_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	Timer1_Interrupt(State);
	Timer1_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_Timer2_Init
// 描述: Timer2嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_Timer2_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	Timer2_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_Timer3_Init
// 描述: Timer3嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_Timer3_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	Timer3_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_Timer4_Init
// 描述: Timer4嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_Timer4_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	Timer4_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_INT0_Init
// 描述: INT0嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_INT0_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	INT0_Interrupt(State);
	INT0_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_INT1_Init
// 描述: INT1嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_INT1_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	INT1_Interrupt(State);
	INT1_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_INT2_Init
// 描述: INT2嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_INT2_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	INT2_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_INT3_Init
// 描述: INT3嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_INT3_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	INT3_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_INT4_Init
// 描述: INT4嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, NULL.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_INT4_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	INT4_Interrupt(State);
	Priority = NULL;
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_ADC_Init
// 描述: ADC嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_ADC_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	ADC_Interrupt(State);
	ADC_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_CMP_Init
// 描述: 比较器嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, RISING_EDGE/FALLING_EDGE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_CMP_Init(u8 State, u8 Priority)
{
	if(Priority > Priority_3) return FAIL;
	if(State & RISING_EDGE)	CMPCR1 |= PIE;			//允许上升沿中断
	else	CMPCR1 &= ~PIE;			//禁止上升沿中断
	if(State & FALLING_EDGE)	CMPCR1 |= NIE;		//允许下降沿中断
	else	CMPCR1 &= ~NIE;			//禁止上升沿中断
	CMP_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_I2C_Init
// 描述: I2C嵌套向量中断控制器初始化.
// 参数: Mode:     模式, I2C_Mode_Master/I2C_Mode_Slave.
// 参数: State:    中断使能状态, I2C_Mode_Master: ENABLE/DISABLE.
//                              I2C_Mode_Slave: I2C_ESTAI/I2C_ERXI/I2C_ETXI/I2C_ESTOI/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_I2C_Init(u8 Mode, u8 State, u8 Priority)
{
	if(Mode > I2C_Mode_Master) return FAIL;
	if(Priority > Priority_3) return FAIL;
	if(Mode == I2C_Mode_Master)
	{
		I2C_Master_Inturrupt(State);
	}
	else if(Mode == I2C_Mode_Slave)
	{
		I2CSLCR = (I2CSLCR & ~0x78) | State;
	}
	CMP_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_UART1_Init
// 描述: UART1嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_UART1_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	UART1_Interrupt(State);
	UART1_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_UART2_Init
// 描述: UART2嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_UART2_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	UART2_Interrupt(State);
	UART2_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_UART3_Init
// 描述: UART3嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_UART3_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	UART3_Interrupt(State);
	UART3_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_UART4_Init
// 描述: UART4嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_UART4_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	UART4_Interrupt(State);
	UART4_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_SPI_Init
// 描述: SPI嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, ENABLE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_SPI_Init(u8 State, u8 Priority)
{
	if(State > ENABLE) return FAIL;
	if(Priority > Priority_3) return FAIL;
	SPI_Interrupt(State);
	SPI_Priority(Priority);
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_PWM_Init
// 描述: PWM嵌套向量中断控制器初始化.
// 参数: Channel:  通道, PWMA/PWMB.
// 参数: State:    中断使能状态, PWM_BIE/PWM_TIE/PWM_COMIE/PWM_CC8IE~PWM_CC1IE/PWM_UIE/DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_PWM_Init(u8 Channel, u8 State, u8 Priority)
{
	if(Channel > PWMB) return FAIL;
	if(Priority > Priority_3) return FAIL;
	switch(Channel)
	{
		case PWMA:
			PWMA_IER = State;
			PWMA_Priority(Priority);
		break;

		case PWMB:
			PWMB_IER = State;
			PWMB_Priority(Priority);
		break;

		default:
			PWMB_IER = State;
			Priority = NULL;
		break;
	}
	return SUCCESS;
}

//========================================================================
// 函数: NVIC_RTC_Init
// 描述: SPI嵌套向量中断控制器初始化.
// 参数: State:    中断使能状态, 中断使能, 0x80:闹钟中断, 0x40:日中断, 0x20:小时中断, 0x10:分钟中断, 0x08:秒中断, 0x04:1/2秒中断, 0x02:1/8秒中断, 0x01:1/32秒中断 /DISABLE.
// 参数: Priority: 中断优先级, Priority_0,Priority_1,Priority_2,Priority_3.
// 返回: 执行结果 SUCCESS/FAIL.
// 版本: V1.0, 2020-09-29
//========================================================================
u8 NVIC_RTC_Init(u8 State, u8 Priority)
{
	if(Priority <= Priority_3) RTC_Priority(Priority); else  return FAIL;
	RTC_Interrupt(State); 
	return SUCCESS;
}

