#include "DataDef.h"
//#include "absacc.h"
//#include "intrins.h"
//#include "string.h"
/*****************************************************************************
//测试硬件平台：STC12C5A60S2/STC12LE5A60S2
//主频：12MHZ
//本测试程序接线定义如下：
//请认真检查接线
//本程序最少只需要接4个IO即可驱动这款液晶屏（将液晶片选和背光接至高电平3.3V）
//本程序采用STC12LE5A60S2/STC12C5A60S2 硬件SPI驱动，其中SCLK、MOSI两个引脚不可随意更改IO
//其余控制引脚均可根据需要自行修改IO配置
*******************************************************************************/
//---------------------------液晶屏接线说明-------------------------------------//

sbit bl        =P0^4;//接模块LED引脚，背光可以采用IO控制或者PWM控制，也可以直接接到高电平常亮
sbit scl       =P1^7;//接模块CLK引脚（固定）
sbit sda       =P1^5;//接模块SDI引脚（固定）
sbit rs        =P0^5;//接模块RS引脚，//
sbit reset     =P0^6;//接模块RST引脚
sbit cs        =P0^7;//接模块CS引脚	 //

//---------------------------End of液晶屏接线---------------------------------//

//测试硬件平台：STC12C5A60S2/STC12LE5A60S2
//主频：12MHZ
//单片机IO推挽输出设置定义
//#define MCU_STC12//如果您使用的单片机不是STC12系列(如STC89C52)请屏蔽此宏定义
//#define		MAIN_Fosc	22000000L	//定义时钟
// #ifdef MCU_STC12
// sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
// sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
// sfr AUXR1 = 0xA2;
// #endif
//-----------------------------SPI 总线配置--------------------------------------//
#define USE_HARDWARE_SPI     1  //1:Enable Hardware SPI;0:USE Soft SPI

//-------------------------屏幕物理像素设置--------------------------------------//
#define LCD_X_SIZE	        176
#define LCD_Y_SIZE	        220

/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#define USE_HORIZONTAL  		1	//定义是否使用横屏 		0,不使用.1,使用.

#ifdef USE_HORIZONTAL//如果定义了横屏 
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE
#endif
//////////////////////////////////////////////////////////////////////////////////

//STC单片机硬件SPI寄存器定义
// sfr SPSTAT = 0xCD;	//STC12C5A60S2系列
// sfr SPCTL  = 0xCE;	//STC12C5A60S2系列
// sfr SPDAT  = 0xCF;	//STC12C5A60S2系列
//sfr SPCTL   = 0x85;	SPI控制寄存器
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: 忽略SS脚，由MSTR位决定主机还是从机		0: SS脚用于决定主从机。
#define	SPEN		1	//1: 允许SPI，								0：禁止SPI，所有SPI管脚均为普通IO
#define	DORD		0	//1：LSB先发，								0：MSB先发
#define	MSTR		1	//1：设为主机								0：设为从机
#define	CPOL		1	//1: 空闲时SCLK为高电平，					0：空闲时SCLK为低电平
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//sfr SPSTAT  = 0xCD;	//SPI状态寄存器
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI传输完成标志。写入1清0。
#define	WCOL	0x40		//SPI写冲突标志。写入1清0。

//常用颜色值定义
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define GRAY1   0x8410      	//灰色1      00000 000000 00000
#define GRAY2   0x4208      	//灰色2  1111111111011111

void Lcd_Init(void);
void Lcd_Clear(uint Color);
void Gui_DrawFont_Num32(uint x, uint y, uint fc, uint bc, uint num);
void Gui_DrawFont_GBK16(uint x, uint y, uint fc, uint bc, uchar *s);