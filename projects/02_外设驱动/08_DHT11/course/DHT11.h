#ifndef DHT11_H
#define DHT11_H

#include "GPIO.h"
#include "Delay.h"

#define DHT11 P46

#define calc_high_and_low_time(level , min , max , desc , val)     count = 0;		                                          \                                           
                                                                   do{                                                          \      
                                                                       count++;	                                              \      
                                                                       NOP16();	                                              \          
                                                                   }while( DHT11 == level);                                   \                  
                                                                   if(count <min  || count > max){                             \                          
                                                                       printf("%s ::  %d\n", desc ,(int)count);	                \                                              
                                                                       return val;                                                \                     
                                                                   }                                                                                      



typedef struct{
    float temp;
    float hum;
}TH;


//初始化
void DHT11_Init(void); 

//获取温湿度
//参数：
//      指针，用来接受温度和湿度的数据
//返回值：
//      0：成功
//      -1：主机释放总线时间超时
//      -2：从机响应低电平时间超时
//      -3：从机响应高电平时间超时      
//      -4：信号0或1低电平时间超时
//      -5：信号0或1高电平时间超时
//      -6：数据校验失败
int DHT11_GetTempAndHumidity(TH * th);

#endif