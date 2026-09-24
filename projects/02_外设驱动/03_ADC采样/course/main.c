#include "ADC.h"
#include "GPIO.h"
#include "UART.h"
#include "NVIC.h"
#include "Switch.h"
#include "Delay.h"

void GPIO_Config(void) {
	GPIO_InitTypeDef	GPIO_InitStructure;		//结构定义
	GPIO_InitStructure.Pin  = GPIO_Pin_4;		//指定要初始化的IO,
	GPIO_InitStructure.Mode = GPIO_HighZ;	//指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0, &GPIO_InitStructure);//初始化
}

void UART_Config(void) {
	// >>> 记得添加 NVIC.c, UART.c, UART_Isr.c <<<
    COMx_InitDefine		COMx_InitStructure;					//结构定义
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
    COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
    COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
    COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
    UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4

  	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
    UART1_SW(UART1_SW_P30_P31);		// 引脚选择, UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void ADC_Config(void) {
	// 创建结构体变量
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_SMPduty = 10;		// 采样时间
    ADC_InitStructure.ADC_Speed = ADC_SPEED_2X1T;	// 转换速度
    ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	// 结果调整
    ADC_InitStructure.ADC_CsSetup = 0;		// 通道选择时间
    ADC_InitStructure.ADC_CsHold = 1;		// 通道选择保持时间

    // 初始化ADC
    ADC_Inilize(&ADC_InitStructure);

    // 使能ADC
    ADC_PowerControl(ENABLE);    // 使能ADC
}

// 电阻阻值和温度对照数组. 电阻 * 100 再来比较
u16 code temp_table[]= {
	58354, // -55
	55464, // -54
	52698, // -53
	50048, // -52
	47515, // -51
	45097, // -50
	42789, // -49
	40589, // -48
	38492, // -47
	36496, // -46
	34597, // -45
	32791, // -44
	31075, // -43
	29444, // -42
	27896, // -41
	26427, // -40
	25034, // -39
	23713, // -38
	22460, // -37
	21273, // -36
	20148, // -35
	19083, // -34
	18075, // -33
	17120, // -32
	16216, // -31
	15361, // -30
	14551, // -29
	13785, // -28
	13061, // -27
	12376, // -26
	11728, // -25
	11114, // -24
	10535, // -23
	9986,  // -22
	9468,  // -21
	8977,  // -20
	8513,  // -19
	8075,  // -18
	7660,  // -17
	7267,  // -16
	6896,  // -15
	6545,  // -14
	6212,  // -13
	5898,  // -12
	5601,  // -11
	5319,  // -10
	5053,  // -9
	4801,  // -8
	4562,  // -7
	4336,  // -6
	4122,  // -5
	3920,  // -4
	3728,  // -3
	3546,  // -2
	3374,  // -1
	3211,  // 0
	3057,  // 1
	2910,  // 2
	2771,  // 3
	2639,  // 4
	2515,  // 5
	2396,  // 6
	2284,  // 7
	2177,  // 8
	2076,  // 9
	1978,  // 10
	1889,  // 11
	1802,  // 12
	1720,  // 13
	1642,  // 14
	1568,  // 15
	1497,  // 16
	1430,  // 17
	1366,  // 18
	1306,  // 19
	1248,  // 20
	1193,  // 21
	1141,  // 22
	1092,  // 23
	1044,  // 24
	1000,  // 25
	957,   // 26
	916,   // 27
	877,   // 28
	840,   // 29
	805,   // 30
	771,   // 31
	739,   // 32
	709,   // 33
	679,   // 34
	652,   // 35
	625,   // 36
	600,   // 37
	576,   // 38
	552,   // 39
	530,   // 40
	509,   // 41
	489,   // 42
	470,   // 43
	452,   // 44
	434,   // 45
	417,   // 46
	401,   // 47
	386,   // 48
	371,   // 49
	358,   // 50
	344,   // 51
	331,   // 52
	318,   // 53
	306,   // 54
	295,   // 55
	284,   // 56
	274,   // 57
	264,   // 58
	254,   // 59
	245,   // 60
	236,   // 61
	228,   // 62
	220,   // 63
	212,   // 64
	205,   // 65
	198,   // 66
	191,   // 67
	184,   // 68
	178,   // 69
	172,   // 70
	166,   // 71
	160,   // 72
	155,   // 73
	150,   // 74
	145,   // 75
	140,   // 76
	135,   // 77
	131,   // 78
	126,   // 79
	122,   // 80
	118,   // 81
	115,   // 82
	111,   // 83
	107,   // 84
	104,   // 85
	101,   // 86
	97,    // 87
	94,    // 88
	91,    // 89
	89,    // 90
	86,    // 91
	83,    // 92
	81,    // 93
	78,    // 94
	76,    // 95
	74,    // 96
	71,    // 97
	69,    // 98
	67,    // 99
	65,    // 100
	63,    // 101
	61,    // 102
	60,    // 103
	58,    // 104
	56,    // 105
	55,    // 106
	53,    // 107
	52,    // 108
	50,    // 109
	49,    // 110
	47,    // 111
	46,    // 112
	45,    // 113
	43,    // 114
	42,    // 115
	41,    // 116
	40,    // 117
	39,    // 118
	38,    // 119
	37,    // 120
	36,    // 121
	35,    // 122
	34,    // 123
	33,    // 124
	32,    // 125
};


void main(void) {
    u16 adcValue , min_index;
    float voltage , resistance , min_diff , diff;
    u8 i;
	EA = 1;		// 开启总中断
	GPIO_Config();	// 配置GPIO
	UART_Config();	// 配置UART
    ADC_Config();	// 配置ADC

	while (1) {

        //1. 获取ADC值
        adcValue = Get_ADCResult(ADC_CH12);	// 读取ADC值
        printf("ADC Value: %d\n", adcValue);		// 打印ADC值

        //2. 通过ADC的值获取到电压值
        voltage = adcValue *2.5 / 4096;	// 计算电压值
        printf("Voltage: %.2f V\n", voltage);		// 打印电压值

        //3. 通过电压值获取到电阻值
        resistance = voltage * 10 / (3.3 - voltage);	// 计算电阻值
        printf("Resistance: %.2f\n", resistance);		// 打印电阻值

        //4. 通过电阻值获取到温度值

        //4.1 让电阻乘以 100， 然后和数组中的每一个元素进行差值比较， 找到最接近的那个元素
        // 得到这个元素的下标之后， -55 即可得到温度值..
        resistance = resistance * 100;	
       
        //4.2. 先用第0个元素来和100倍数组先计算差值，我们就认为这个差值是最小的
        min_diff = resistance - temp_table[0] > 0 ? resistance - temp_table[0] : -(resistance - temp_table[0]);	// 计算差值
        min_index = 0;	// 记录最小差值的下标

        //4.3. 遍历数组剩下的每一个元素，得到每一个元素的差值，都和上面的最小差值进行比大小，是不是还更小呢？如果是就更新最小的差值。
        for( i= 1; i < sizeof(temp_table) / sizeof(u16); i++) {
        	
            //4.4 拿出来每一个元素都和100倍的电阻比较
            diff = resistance - temp_table[i] > 0 ? resistance - temp_table[i] : -(resistance - temp_table[i]);

            //4.5 拿出来的差值和最小差值进行比较
            if(diff < min_diff) {
                min_diff = diff;	// 更新最小差值
                min_index = i;	// 更新最小差值的下标
            }
        }

        //4.6 最小差值的下标就是温度值
        printf("Temperature: %d\n",  min_index - 55);		// 打印温度值

        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250); 
		
	}
}