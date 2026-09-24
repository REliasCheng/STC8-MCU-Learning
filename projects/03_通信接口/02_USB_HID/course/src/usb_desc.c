/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

#include "stc.h"
#include "usb_desc.h"

//设备描述 :: 告诉主机，设备的信息有哪些
char code DEVICEDESC[18] =
{
    0x12,                   //bLength(18);
    0x01,                   //bDescriptorType(Device);
    0x00,0x02,              //bcdUSB(2.00);  
    0x00,                   //bDeviceClass(0);
    0x00,                   //bDeviceSubClass0);
    0x00,                   //bDeviceProtocol(0);
    0x40,                   //bMaxPacketSize0(64);
    0xbf,0x34,              //idVendor(34bf);
    0x01,0xff,              //idProduct(ff01);
    0x00,0x01,              //bcdDevice(1.00);
    0x01,                   //iManufacturer(1);
    0x02,                   //iProduct(2);
    0x00,                   //iSerialNumber(0);
    0x01,                   //bNumConfigurations(1);
};

// 配置描述符 :: 供电、发数据、接受数据，数据长度是多少..
char code CONFIGDESC[41] =
{
	//供电信息
    0x09,                   //bLength(9);
    0x02,                   //bDescriptorType(Configuration);
    0x29,0x00,              //wTotalLength(41);
    0x01,                   //bNumInterfaces(1);
    0x01,                   //bConfigurationValue(1);
    0x00,                   //iConfiguration(0);
    0x80,                   //bmAttributes(BUSPower);
    0x32,                   //MaxPower(100mA);

	// 接口信息
    0x09,                   //bLength(9);
    0x04,                   //bDescriptorType(Interface);
    0x00,                   //bInterfaceNumber(0);
    0x00,                   //bAlternateSetting(0);
    0x02,                   //bNumEndpoints(2);  // 2个端点  输入 和 输出
    0x03,                   //bInterfaceClass(HID);
    0x01,                   //bInterfaceSubClass(Boot);
    0x01,                   //bInterfaceProtocol(0);   // Keyboard
    0x00,                   //iInterface(0);

	// HID 描述信息 ，用的是什么版本的HID协议  1.01
    0x09,                   //bLength(9);
    0x21,                   //bDescriptorType(HID);
    0x01,0x01,              //bcdHID(1.01);
    0x00,                   //bCountryCode(0);
    0x01,                   //bNumDescriptors(1);
    0x22,                   //bDescriptorType(HID Report);
    0x41,0x00,              //wDescriptorLength(65);

		// 输入的信息描述
    0x07,                   //bLength(7);
    0x05,                   //bDescriptorType(Endpoint);
    0x81,                   //bEndpointAddress(EndPoint1 as IN);
    0x03,                   //bmAttributes(Interrupt);
    0x08,0x00,              //wMaxPacketSize(64);
    0x0a,                   //bInterval(10ms);

		//输出的信息描述
    0x07,                   //bLength(7);
    0x05,                   //bDescriptorType(Endpoint);
    0x01,                   //bEndpointAddress(EndPoint1 as OUT);
    0x03,                   //bmAttributes(Interrupt);
    0x01,0x00,              //wMaxPacketSize(1);
    0x0a,                   //bInterval(10ms);
};


/*
这个USB设备给电脑发送消息，每次都发送什么？

0 0 0 0   0 0 0 0
第0个字节用来说明功能组合键位：
	ctrL : 0000 0001
	ctrl+shift :  0000 0011
	ctrl+shift+alt :  0000 0111
0   Modifierkeys (D0:LCtrl D1:LShift D2:LAlt D3:LGui D4:RCtrl D5:RShift D6:RAlt D7:RGui)

//第1个字节，保留不用！
1	Reserved

//第2个字节到第7个字节，就用来装普通的按键
2	Keycode 1
3	Keycode 2
4	Keycode 3
5	Keycode 4
6	Keycode 5
7	Keycode 6



这个USB设备从电脑收到消息，每次都收到什么？
只会从电脑那里得到1个字节。
	0000 0000
	数字锁定键：PC 会返回：    0000 0001
	大小写切换键： PC 会返回： 0000 0010
	滚动锁定键： PC 会返回：   0000 0100
	0   LEDs (D0:NumLock D1:CapLock D2:ScrollLock)
*/

char code HIDREPORTDESC[65] =
{
   0x05,0x01,              //USAGE_PAGE(Generic Desktop);
    0x09,0x06,              //USAGE(Keyboard);
    0xa1,0x01,              //COLLECTION(Application);
    0x05,0x07,              //  USAGE_PAGE(Keyboard);
    0x19,0xe0,              //  USAGE_MINIMUM(224);
    0x29,0xe7,              //  USAGE_MAXIMUM(255);
    0x15,0x00,              //  LOGICAL_MINIMUM(0);
    0x25,0x01,              //  LOGICAL_MAXIMUM(1);
    0x75,0x01,              //  REPORT_SIZE(1);
    0x95,0x08,              //  REPORT_COUNT(8);
    0x81,0x02,              //  INPUT(Data,Variable,Absolute);
    
    0x75,0x08,              //  REPORT_SIZE(8);
    0x95,0x01,              //  REPORT_COUNT(1);
    0x81,0x01,              //  INPUT(Constant);
    
    0x19,0x00,              //  USAGE_MINIMUM(0);
    0x29,0x65,              //  USAGE_MAXIMUM(101);
    0x15,0x00,              //  LOGICAL_MINIMUM(0);
    0x25,0x65,              //  LOGICAL_MAXIMUM(101);
    0x75,0x08,              //  REPORT_SIZE(8);
    0x95,0x06,              //  REPORT_COUNT(6);
    0x81,0x00,              //  INPUT(Data,Array);
    
    0x05,0x08,              //  USAGE_PAGE(LEDs);
    0x19,0x01,              //  USAGE_MINIMUM(1);
    0x29,0x03,              //  USAGE_MAXIMUM(3);
    0x15,0x00,              //  LOGICAL_MINIMUM(0);
    0x25,0x01,              //  LOGICAL_MAXIMUM(1);
    0x75,0x01,              //  REPORT_SIZE(1);
    0x95,0x03,              //  REPORT_COUNT(3);
    0x91,0x02,              //  OUTPUT(Data,Variable,Absolute);
    
    0x75,0x05,              //  REPORT_SIZE(5);
    0x95,0x01,              //  REPORT_COUNT(1);
    0x91,0x01,              //  OUTPUT(Constant);
    
    0xc0,                   //END_COLLECTION;
};

char code LANGIDDESC[4] =
{
    0x04,0x03,
    0x09,0x04,
};

char code MANUFACTDESC[8] =
{
    0x08,0x03,
    'S',0,
    'T',0,
    'C',0,
};


//产品描述
char code PRODUCTDESC[14] =
{
	// 0x1a， 就代表26， 表示发送的数据总长有26. 
	// 0x03, 表示字符类型， 相当于过来有26个字符
	// 黑马@@键盘 ：  \u9ed1\u9a6c\u952e\u76d8  :: 每一个字符，都由两个字节组成，不管是中文还是英文。英文都是1个字节，所以在高字节补 0
    0x0E,0x03,
    0xd1,0x9e,
	  0x6c,0x9a,
		'@',0,
		'@',0,
	  0x2e,0x95,
	  0xd8,0x76
};

char code PACKET0[2] = 
{
    0, 0,
};

char code PACKET1[2] = 
{
    1, 0,
};
