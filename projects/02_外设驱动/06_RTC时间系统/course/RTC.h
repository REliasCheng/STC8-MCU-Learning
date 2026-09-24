#ifndef __RTC_H
#define __RTC_H

#include "GPIO.h"
#include "Switch.h"		
#include "I2C.h"
#include "NVIC.h"


#define DEV_ADD 0xA2
#define MEM_ADD 0x02

#define BCD2Decimal(byte , val) ((( byte & val )>>4) * 10) +  (byte & 0x0F)
#define Decimal2BCD(byte)   (( byte /10 )<< 4) + (byte % 10)

typedef enum {
     HZ_4096, HZ_64 , HZ_1, HZ_1_60 
} Timer_HZ;
typedef struct {
     u8 second , minute, hour ,day, month, week;
     u16  year;
} RTC_Time;


// =============== 如果闹钟不想采用 -1 的写法，那么可以试着使用这种掩码的写法 =================

/*
typedef enum{
     MASK_NONE   = 0x00 ,
     MASK_MINUTE = 0x01 , 
     MASK_HOUR   = 0x02 , 
     MASK_DAY    = 0x04 , 
     MASK_WEEK   = 0x08 ,
     MASK_ALL    = 0x0F 
}RTC_Alarm_Mask;

typedef struct{
     u8  minute;
     u8  hour;
     u8  day;
     u8  week;
     RTC_Alarm_Mask mask;
}RTC_Alarm;

void RTC_Alarm_Start(RTC_Alarm * alarm);

//使用方
void main(){

     // C99 允许我们只给其中的几个成员赋值
     RTC_Alarm alarm = {.minute = 30, .mask = MASK_MINUTE};
     RTC_Alarm_Start(&alarm);

     RTC_Alarm alarm1 = { .minute = 30,  .hour=12 ,   .mask = MASK_MINUTE | MASK_HOUR};
     RTC_Alarm_Start(&alarm1);

     //C89 的写法
     RTC_Alarm alarm;
     alarm.minute = 30; // 0011 0000
     alarm.mask = MASK_MINUTE ;

}
*/

//================================= 时钟 ===============================

//初始化RTC
void RTC_Init(void);

//读取时间
//参数: 结构体指针,用来接收时间数据
void RTC_ReadTime(RTC_Time * time);

//写入时间
//参数: 结构体指针,用来传递时间数据
void RTC_WriteTime(RTC_Time * time);

//================================= 闹钟 ===============================

// 开启闹钟[分、时、日、周]
// 外部给过来的数据至少得有一个，否则闹钟就不会触发了。
// 如果传递过来的值是 -1, 即表示不想开启这个分支
void RTC_Alarm_On(char minute , char hour , char day ,char week);

//停止闹钟
void RTC_Alarm_Off(void); 

//清除闹钟标志位
void RTC_Alarm_Clear_Flag();



//================================= 定时器 ===============================

// 开启定时器
// hz: 频率
// count: 倒计数数值
// 例如: 4096Hz, 2s 触发一次定时器中断, 那么 count = 4096 * 2 = 8192
// 例如: 64Hz, 2s 触发一次定时器中断, 那么 count = 64 * 2 = 128
// 例如: 1Hz, 2s 触发一次定时器中断, 那么 count = 1 * 2 = 2
void RTC_Timer_On(Timer_HZ hz , u8 count);

// 停止定时器
void RTC_Timer_Off(void);

// 清除定时器标志位
void RTC_Timer_Clear_Flag(void);

// 判断是否是闹钟引发的中断 1 表示是 0 表示不是
u8 RTC_IS_ALARM_INT(void);

// 判断是否是定时器引发的中断 1 表示是 0 表示不是
u8  RTC_IS_TIMER_INT(void);

#endif