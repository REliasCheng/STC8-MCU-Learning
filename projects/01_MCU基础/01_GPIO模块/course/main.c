#include "GPIO.h"
#include "Delay.h"


void main() {

    GPIO_InitTypeDef init;
    init.Mode = GPIO_OUT_PP;		//IO模式,  		GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
    init.Pin  = GPIO_Pin_3;		//要设置的端口
    GPIO_Inilize(GPIO_P5, &init);


    while(1) {


        P53 = 1 ;
        delay_ms(250);
        delay_ms(250);
        //delay_ms(250);
        //delay_ms(250);

        P53 = 0 ;
        delay_ms(250);
        delay_ms(250);
        //delay_ms(250);
        //delay_ms(250);
    }
}
