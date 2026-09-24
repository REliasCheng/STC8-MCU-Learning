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

#include "STC8H_PWM.h"

//========================================================================
// 函数: PWM_Configuration
// 描述: PWM初始化程序.
// 参数: PWMx: 结构参数,请参考PWM.h里的定义.
// 返回: 成功返回 SUCCESS, 错误返回 FAIL.
// 版本: V1.0, 2012-10-22
//========================================================================
u8 PWM_Configuration(u8 PWM, PWMx_InitDefine *PWMx)
{
	if(PWM == PWM1)
	{
		PWMA_CC1E_Disable();		//关闭输入捕获/比较输出
		PWMA_CC1NE_Disable();		//关闭比较输出
		PWMA_CC1S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMA_OC1ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO1P)
		{
			PWMA_CC1E_Enable();			//开启输入捕获/比较输出
			PWMA_ENO |= ENO1P;
		}
		else
		{
			PWMA_CC1E_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO1P;
		}
		if(PWMx->PWM_EnoSelect & ENO1N)
		{
			PWMA_CC1NE_Enable();		//开启输入捕获/比较输出
			PWMA_ENO |= ENO1N;
		}
		else
		{
			PWMA_CC1NE_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO1N;
		}
		
		PWMA_Duty1(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM2)
	{
		PWMA_CC2E_Disable();		//关闭输入捕获/比较输出
		PWMA_CC2NE_Disable();		//关闭比较输出
		PWMA_CC2S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMA_OC2ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO2P)
		{
			PWMA_CC2E_Enable();			//开启输入捕获/比较输出
			PWMA_ENO |= ENO2P;
		}
		else
		{
			PWMA_CC2E_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO2P;
		}
		if(PWMx->PWM_EnoSelect & ENO2N)
		{
			PWMA_CC2NE_Enable();		//开启输入捕获/比较输出
			PWMA_ENO |= ENO2N;
		}
		else
		{
			PWMA_CC2NE_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO2N;
		}
		
		PWMA_Duty2(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM3)
	{
		PWMA_CC3E_Disable();		//关闭输入捕获/比较输出
		PWMA_CC3NE_Disable();		//关闭比较输出
		PWMA_CC3S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMA_OC3ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO3P)
		{
			PWMA_CC3E_Enable();			//开启输入捕获/比较输出
			PWMA_ENO |= ENO3P;
		}
		else
		{
			PWMA_CC3E_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO3P;
		}
		if(PWMx->PWM_EnoSelect & ENO3N)
		{
			PWMA_CC3NE_Enable();		//开启输入捕获/比较输出
			PWMA_ENO |= ENO3N;
		}
		else
		{
			PWMA_CC3NE_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO3N;
		}
		
		PWMA_Duty3(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM4)
	{
		PWMA_CC4E_Disable();		//关闭输入捕获/比较输出
		PWMA_CC4NE_Disable();		//关闭比较输出
		PWMA_CC4S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMA_OC4ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO4P)
		{
			PWMA_CC4E_Enable();			//开启输入捕获/比较输出
			PWMA_ENO |= ENO4P;
		}
		else
		{
			PWMA_CC4E_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO4P;
		}
		if(PWMx->PWM_EnoSelect & ENO4N)
		{
			PWMA_CC4NE_Enable();		//开启输入捕获/比较输出
			PWMA_ENO |= ENO4N;
		}
		else
		{
			PWMA_CC4NE_Disable();		//关闭输入捕获/比较输出
			PWMA_ENO &= ~ENO4N;
		}
		
		PWMA_Duty4(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWMA)
	{
//		PWMA_OC1_ReloadEnable(PWMx->PWM_Reload);	//输出比较的预装载使能
//		PWMA_OC1_FastEnable(PWMx->PWM_Fast);		//输出比较快速功能使能
//		PWMA_CCPCAPreloaded(PWMx->PWM_PreLoad);	//捕获/比较预装载控制位(该位只对具有互补输出的通道起作用)
//		PWMA_BrakeEnable(PWMx->PWM_BrakeEnable);	//开启/禁止刹车输入

		PWMA_DeadTime(PWMx->PWM_DeadTime);	//死区发生器设置
		PWMA_AutoReload(PWMx->PWM_Period);	//周期设置
		PWMA_BrakeOutputEnable(PWMx->PWM_MainOutEnable);	//主输出使能
		PWMA_CEN_Enable(PWMx->PWM_CEN_Enable);	//使能计数器
		
		return	0;
	}

	if(PWM == PWM5)
	{
		PWMB_CC5E_Disable();		//关闭输入捕获/比较输出
		PWMB_CC5S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMB_OC5ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO5P)
		{
			PWMB_CC5E_Enable();			//开启输入捕获/比较输出
			PWMB_ENO |= ENO5P;
		}
		else
		{
			PWMB_CC5E_Disable();		//关闭输入捕获/比较输出
			PWMB_ENO &= ~ENO5P;
		}
		
		PWMB_Duty5(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM6)
	{
		PWMB_CC6E_Disable();		//关闭输入捕获/比较输出
		PWMB_CC6S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMB_OC6ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO6P)
		{
			PWMB_CC6E_Enable();			//开启输入捕获/比较输出
			PWMB_ENO |= ENO6P;
		}
		else
		{
			PWMB_CC6E_Disable();		//关闭输入捕获/比较输出
			PWMB_ENO &= ~ENO6P;
		}
		
		PWMB_Duty6(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM7)
	{
		PWMB_CC7E_Disable();		//关闭输入捕获/比较输出
		PWMB_CC7S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMB_OC7ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO7P)
		{
			PWMB_CC7E_Enable();			//开启输入捕获/比较输出
			PWMB_ENO |= ENO7P;
		}
		else
		{
			PWMB_CC7E_Disable();		//关闭输入捕获/比较输出
			PWMB_ENO &= ~ENO7P;
		}
		
		PWMB_Duty7(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWM8)
	{
		PWMB_CC8E_Disable();		//关闭输入捕获/比较输出
		PWMB_CC8S_Direction(CCAS_OUTPUT);		//CCnS仅在通道关闭时才是可写的
		PWMB_OC8ModeSet(PWMx->PWM_Mode);		//设置输出比较模式

		if(PWMx->PWM_EnoSelect & ENO8P)
		{
			PWMB_CC8E_Enable();			//开启输入捕获/比较输出
			PWMB_ENO |= ENO8P;
		}
		else
		{
			PWMB_CC8E_Disable();		//关闭输入捕获/比较输出
			PWMB_ENO &= ~ENO8P;
		}
		
		PWMB_Duty8(PWMx->PWM_Duty);
		
		return	SUCCESS;
	}
	
	if(PWM == PWMB)
	{
//		PWMB_OC5_ReloadEnable(PWMx->PWM_Reload);	//输出比较的预装载使能
//		PWMB_OC5_FastEnable(PWMx->PWM_Fast);		//输出比较快速功能使能
//		PWMB_CCPCBPreloaded(PWMx->PWM_PreLoad);	//捕获/比较预装载控制位(该位只对具有互补输出的通道起作用)
//		PWMB_BrakeEnable(PWMx->PWM_BrakeEnable);	//开启/禁止刹车输入

		PWMB_DeadTime(PWMx->PWM_DeadTime);	//死区发生器设置
		PWMB_AutoReload(PWMx->PWM_Period);	//周期设置
		PWMB_BrakeOutputEnable(PWMx->PWM_MainOutEnable);	//主输出使能
		PWMB_CEN_Enable(PWMx->PWM_CEN_Enable);	//使能计数器
		
		return	0;
	}

	return	FAIL;	//错误
}

//========================================================================
// 函数: UpdatePwm
// 描述: PWM占空比更新程序.
// 参数: PWM: PWM通道/组号, PWMx结构参数,请参考PWM.h里的定义.
// 返回: none.
// 版本: V1.0, 2012-10-22
//========================================================================
void UpdatePwm(u8 PWM, PWMx_Duty *PWMx)
{
	switch(PWM)
	{
		case PWM1:
			PWMA_Duty1(PWMx->PWM1_Duty);
		break;

		case PWM2:
			PWMA_Duty2(PWMx->PWM2_Duty);
		break;

		case PWM3:
			PWMA_Duty3(PWMx->PWM3_Duty);
		break;

		case PWM4:
			PWMA_Duty4(PWMx->PWM4_Duty);
		break;

		case PWM5:
			PWMB_Duty5(PWMx->PWM5_Duty);
		break;

		case PWM6:
			PWMB_Duty6(PWMx->PWM6_Duty);
		break;

		case PWM7:
			PWMB_Duty7(PWMx->PWM7_Duty);
		break;

		case PWM8:
			PWMB_Duty8(PWMx->PWM8_Duty);
		break;

		case PWMA:
			PWMA_Duty1(PWMx->PWM1_Duty);
			PWMA_Duty2(PWMx->PWM2_Duty);
			PWMA_Duty3(PWMx->PWM3_Duty);
			PWMA_Duty4(PWMx->PWM4_Duty);
		break;

		case PWMB:
			PWMB_Duty5(PWMx->PWM5_Duty);
			PWMB_Duty6(PWMx->PWM6_Duty);
			PWMB_Duty7(PWMx->PWM7_Duty);
			PWMB_Duty8(PWMx->PWM8_Duty);
		break;
	}
}

/*********************************************************/
