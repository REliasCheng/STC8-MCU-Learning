#include "RTC.h"



void RTC_GPIO_Config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_2|GPIO_Pin_3;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_OUT_OD;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
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

}

u8 dat[7]; 

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
