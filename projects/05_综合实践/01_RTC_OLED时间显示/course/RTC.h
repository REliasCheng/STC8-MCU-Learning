#ifndef __RTC_H
#define __RTC_H

#include "GPIO.h"
#include "Switch.h"		
#include "I2C.h"

#define DEV_ADD 0xA2
#define MEM_ADD 0x02

#define BCD2Decimal(byte , val) ((( byte & val )>>4) * 10) +  (byte & 0x0F)
#define Decimal2BCD(byte)   (( byte /10 )<< 4) + (byte % 10)


typedef struct {
     u8 second , minute, hour ,day, month, week;
     u16  year;
} RTC_Time;


//初始化RTC
void RTC_Init(void);

//读取时间
//参数: 结构体指针,用来接收时间数据
void RTC_ReadTime(RTC_Time * time);

//写入时间
//参数: 结构体指针,用来传递时间数据
void RTC_WriteTime(RTC_Time * time);

#endif