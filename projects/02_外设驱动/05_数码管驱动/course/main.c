#include "GPIO.h"
#include "Delay.h"
#include "NIXIE.h"

void main(){
	u8 i;

	NIXIE_Init();

	

	while(1){

		//所有的数码管都显示数字 7
		for (i = 0; i < 33; i++){
			NIXIE_Show_Number(i , 0xFF);
			delay_ms(250);
			delay_ms(250);
		}
		

	}
}