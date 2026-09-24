#include "RTC.h"

void RTC_EXTI_Config(){
	//使能中断，配置中断优先级
	NVIC_INT3_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}


void RTC_GPIO_Config(void) {


	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_OD;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_7;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_PullUp;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3, &GPIO_InitStructure);//初始化
}


void RTC_I2C_Config(void) {
	
	//1. 创建结构体变量
	I2C_InitTypeDef init;		//结构定义

	//2. 给成员赋值
	//总线速度有100K 也有400K
	//400K=24M/2/(Speed*2+4) --> 400K =12M / (speed*2+4) ---> (speed*2+4)  = 12M / 400k  -->  (speed*2+4) = 30;
	init.I2C_Speed = 13 ;				    //总线速度=Fosc/2/(Speed*2+4),      0~63
	init.I2C_Enable = ENABLE;				//I2C功能使能,   ENABLE, DISABLE
	init.I2C_Mode = I2C_Mode_Master;		//主从模式选择,  I2C_Mode_Master,I2C_Mode_Slave
	init.I2C_MS_WDTA = DISABLE;				//主机使能自动发送,  ENABLE, DISABLE . 选择手动发送

	//3. 初始化
	I2C_Init(&init);

	//5. 切换引脚
	I2C_SW(I2C_P33_P32);

}

//初始化RTC
void RTC_Init(void){

    // ?? 注意: I2C 和 PWM 一样,它们的寄存器是位于一块比较隐蔽的区域,我们必须打开开关,才能对这些寄存器进行写入操作,这样才能成功的配置I2C
	EAXSFR();
    RTC_GPIO_Config();
    RTC_I2C_Config();
	RTC_EXTI_Config();

}

u8 dat[7]; 

//================================= 时钟 ===============================




//读取时间
//参数: 结构体指针,用来接收时间数据
void RTC_ReadTime(RTC_Time * time){

    //读取时间
	//参数一: 设备地址 [写地址]
	//参数二: 寄存器地址 [读取的是什么数据,就写这个数据的寄存器地址]
	//参数三: 指针,用来接受数据的 
	//参数四: 读取数据的个数[读取几个字节]
	I2C_ReadNbyte(DEV_ADD, MEM_ADD, &dat,7);
	//由于读出来的额数据是按照BCD码来存储的,所以我们需要进行转换
	//如果显示是12秒,它对应的二进制是: VL 0 0 1  0 0 1 0  :: VL 用来描述时间准还是不准,我们不需要看这个,所以不理会它.
	time->second = BCD2Decimal(dat[0] , 0x70) ;
	time->minute = BCD2Decimal(dat[1] , 0x70);
	time->hour   = BCD2Decimal(dat[2] , 0x70);
	//由于读出来的额数据是按照BCD码来存储的,所以我们需要进行转换
	time->day    = BCD2Decimal(dat[3] , 0x70);
	time->week   = dat[4];
	time->month  = BCD2Decimal(dat[5] , 0x70);
	time->year   = BCD2Decimal(dat[6] , 0xF0); // 00 ~ 99
	//看看世纪位 C 是什么值
	if(dat[5] & 0x80){ // 世纪位 C 是 1  22世纪  从 2100 开始
		time->year += 2100;
	}else{             // 世纪位 C 是 0  21世纪  从 2000 开始
		time->year += 2000;
	}
}

//写入时间
//参数: 结构体指针,用来传递时间数据
//	1. 我们需要先把时间数据转换为BCD码
//  2. 然后再写入
void RTC_WriteTime(RTC_Time * time){
	//写入时间
	// 参数一：写入地址 ， 参数二：写入这份数据从哪里开始写，写到哪里去。
	// 参数三：写的数据是什么 ， 参数四：写多少个字节。

	dat[0] = Decimal2BCD(time->second);	// 秒   => 10
	dat[1] = Decimal2BCD(time->minute);	// 分
	dat[2] = Decimal2BCD(time->hour);	// 时
	dat[3] = Decimal2BCD(time->day);	// 日
	dat[4] = time->week;	// 周	
	dat[5] = Decimal2BCD(time->month);	// 月
	dat[6] = Decimal2BCD(time->year % 100);	// 年

	//处理世纪位：
	if(time->year >= 2100){
		dat[5] |= 0x80; // 1000 0000  最高位置1
	}else if(time->year >= 2000){ // 2000 ~ 2099
		dat[5] &= ~0x80; // 0111 1111  最高位置0		
	}

	I2C_WriteNbyte(DEV_ADD, MEM_ADD, &dat, 7);

}




//================================= 闹钟 ===============================

// 开启闹钟[分、时、日、周]
// 外部给过来的数据至少得有一个，否则闹钟就不会触发了。
// 如果传递过来的值是 -1, 即表示不想开启这个分支
void RTC_Alarm_On(char minute , char hour , char day ,char week){
	u8 dat;

	//---------------------- 1. 配置闹钟开关 ---------------------- 
	//1.1 先把0x01寄存器的值，先读取处理啊
	I2C_ReadNbyte(0xA2, 0x01,&dat, 1);

	//1.2 要设置AIE = 1, 才能使能闹钟 , AIE 是第 1 位，其他位不能动。
	dat |= 1<<1;

	//1.3 还要设置AF = 0 ，才能清除闹钟发生过的标记位，因为不确定它默认是什么值。
	dat&= ~(1<<3);

	//1.4 再把值写回0x01寄存器，就可以了。
	I2C_WriteNbyte(DEV_ADD, 0x01,&dat, 1);


	//---------------------- 2. 配置闹钟时间 ---------------------- 

	//2.1  =====分钟===========
	if(minute != -1){ // 表示用户想开启分钟
		dat = Decimal2BCD(minute) ;
		dat &= ~(1<<7);
		I2C_WriteNbyte(DEV_ADD, 0x09,&dat, 1);
	}

	//2.1  =====小时===========
	if(hour != -1){ // 表示用户想开启小时
		dat = Decimal2BCD(hour) ;
		dat &= ~(1<<7);
		I2C_WriteNbyte(DEV_ADD, 0x0A,&dat, 1);
	}
	//2.2  =====日期===========
	if(day != -1){ // 表示用户想开启小时
		dat = Decimal2BCD(day) ;
		dat &= ~(1<<7);
		I2C_WriteNbyte(DEV_ADD, 0x0B,&dat, 1);
	}
	//2.2  =====星期===========
	if(week != -1){ // 表示用户想开启小时
		dat = Decimal2BCD(week) ;
		dat &= ~(1<<7);
		I2C_WriteNbyte(DEV_ADD, 0x0C,&dat, 1);
	}




}

//停止闹钟
void RTC_Alarm_Off(void){
	u8 dat;
	I2C_ReadNbyte(DEV_ADD, 0x01, &dat, 1); // 0x01 寄存器是用来控制闹钟的
	dat &= ~(1<<1); // AIE 是第 1 位 设置为0，其他位不能动。
	I2C_WriteNbyte(DEV_ADD, 0x01, &dat, 1);
}


//清除闹钟的标志位
void RTC_Alarm_Clear_Flag(){
	u8 dat;
	//还要记得清除闹钟的AF标志位，否则下次闹钟就不会触发了。
	//1. 先把0x01寄存器的值，先读取处理啊
	I2C_ReadNbyte(0xA2, 0x01,&dat, 1);
	dat&= ~(1<<3);
	I2C_WriteNbyte(0xA2, 0x01,&dat, 1);
}


//================================= 定时器 ===============================

// 开启定时器
// hz: 频率
// count: 倒计数数值
// 例如: 4096Hz, 2s 触发一次定时器中断, 那么 count = 4096 * 2 = 8192
// 例如: 64Hz, 2s 触发一次定时器中断, 那么 count = 64 * 2 = 128
// 例如: 1Hz, 2s 触发一次定时器中断, 那么 count = 1 * 2 = 2
void RTC_Timer_On(u8 hz , u8 count){

	u8 dat;

	//1. 配置定时器开关
	I2C_ReadNbyte(0xA2 , 0x01 , &dat, 1);

	//1.1 设置TIE =1 ，第 0 位为 1 ，表示使能定时器中断
	dat |= 1<<0;

	//1.2 设置TF = 0 , 第 2 位为 0, 表示定时器中断标志位清零
	dat &= ~(1<<2);

	//1.3 再把数据写回去
	I2C_WriteNbyte(0xA2, 0x01, &dat, 1);

	//2. 配置定的时间长短

	//2.1 配置频率
	//a. 先设置频率
	//dat = 0x00 ; // 4096Hz
	//dat = 0x01 ; // 64Hz
	//dat = 0x02 ; // 1Hz

	//b. 再配置启用频率
	dat = hz | (1<<7);
	I2C_WriteNbyte(0xA2, 0x0E, &dat, 1);

	//2.2 配置倒计数数值
	//dat =64 * 2; // 2s 触发一次定时器中断
	dat = count;
	I2C_WriteNbyte(0xA2, 0x0F, &dat, 1);
 
}

// 停止定时器
void RTC_Timer_Off(void){
	u8 dat;
	I2C_ReadNbyte(DEV_ADD, 0x01, &dat, 1); // 0x01 寄存器是用来控制闹钟的
	dat &= ~(1<<0); // TIE 是第 0 位 设置为0，其他位不能动。
	I2C_WriteNbyte(DEV_ADD, 0x01, &dat, 1);
}

// 清除定时器标志位
void RTC_Timer_Clear_Flag(void){
	u8 dat;
	//还要记得清除闹钟的AF标志位，否则下次闹钟就不会触发了。
	//1. 先把0x01寄存器的值，先读取处理啊
	I2C_ReadNbyte(0xA2, 0x01,&dat, 1);
	dat&= ~(1<<2);
	I2C_WriteNbyte(0xA2, 0x01,&dat, 1);
}


//=================================== 判断 ===============================

// 判断是否是闹钟引发的中断 1 表示是 0 表示不是
u8 RTC_IS_ALARM_INT(void){
	u8 dat;
	I2C_ReadNbyte(0xA2, 0x01, &dat, 1); // 0x01 寄存器是用来控制闹钟的
	return (dat & (1<<3)) != 0 ? 1 : 0 ; // AF 是第 3 位
}

// 判断是否是定时器引发的中断 1 表示是 0 表示不是
u8  RTC_IS_TIMER_INT(void){
	u8 dat;
	I2C_ReadNbyte(0xA2, 0x01, &dat, 1); // 0x01 寄存器是用来控制闹钟的
	return (dat & (1<<2))!= 0? 1 : 0 ; // TF 是第 2 位
}
