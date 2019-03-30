#include "STC12C5A60S2.h"
#include "GPIO.h"
#include "LCD.h"
#include "Delays.h"
#include "ADXl345.h"
#include "Uart.h"
#include "DS1302.h"
#include "keyscan.h"
#include "math.h"
sbit led = P3^6;
sbit Pulseled = P2^7;
sbit Beep = P1^6;
sbit Fallled = P0^3;
#define Buf_Max2 26   //缓存长度
uchar  xdata Rec_Buf2[Buf_Max2];
uchar count2=0;
uchar Fall;
uchar ss[3];
uchar time = 1,num1 = 1,num2 = 2;


bit PM25_Read = 0;
bit bt500ms;
bit bt1s;
bit bt30s;
bit bt1000ms;
float xdata Vout;
uint xdata Pulse,PM25;
uint xdata cnt0,cnt1,cnt2,cnt3;
int xdata X1,X2,Y1,Y2,Z1,Z2;


void Timer0Init(void)		//1ms@22.1184MHz
{
	AUXR &= 0x7F;		//?????12T??
	TMOD &= 0xF0;		//???????
	TL0 = 0x68;		//??????
	TH0 = 0xC6;		//??????
	TF0 = 0;		//??TF0??
	TR0 = 1;		//???0????
	EA  = 1;
	ET0 = 1;
	
	EX0 = 0;
	IT0 = 1;
}

// uchar *Dis_qian(uint temp_data)  
// {  
// 	ss[0]=temp_data/1000+0x30 ;  //??
// 	temp_data=temp_data%1000;    //????
// 	ss[1]=temp_data/100+0x30   ;  //??
// 	temp_data=temp_data%100;     //????
// 	ss[2]=temp_data/10+0x30    ;  //??
// 	return ss;
// }
void Beep_single()
{
	Beep=0;
	delay_ms(500);
	Beep=1;
}
void Beep_long()
{
	Beep=0;
	delay_ms(100);
	Beep=1;
	delay_ms(100);
	Beep=0;
	delay_ms(100);
	Beep=1;
	delay_ms(100);
	Beep=0;
	delay_ms(100);
	Beep=1;
}	
uchar *Dis_bai(uint temp_data)  
{  
	ss[0]=temp_data/100+0x30;  //??
	temp_data=temp_data%100;     //????
	ss[1]=temp_data/10+0x30;  //??
	temp_data=temp_data%10;      //????
	ss[2]=temp_data+0x30;   //??
	return ss;
}

void main(void)
{
	GPIO_config();
	
	Serial_Init();
	Timer0Init();
	//DS1302init();
//	GSMInit();
	Lcd_Init();
	Init_ADXL345();
	Beep=1;
	bl=1;//通过IO控制背光亮
	led=0;
	Pulseled=0;
	Fallled=1;
	Up=1;
	Down=1;
	Left=1;
	Right=1;
	
	
Normal_Mode://正常工作模式
	do{	
		//**********DS1302采集程序**********//
		if(bt1000ms)
		{
			bt1000ms=0;
			cnt3=0;
			Read_RTC();
			Dis_Time();
		}
		//**********PM2.5采集程序**********//
		if(bt500ms)
		{
			bt500ms=0;
			cnt0=0;
			Vout = (float)((Rec_Buf2[21]*256+Rec_Buf2[22])*5)/1024;
 			PM25 = 145.206*Vout + 19.821;
			Gui_DrawFont_GBK16(16,110,BLUE,GRAY0,"PM2.5:");
			Gui_DrawFont_GBK16(64,110,RED,GRAY0,Dis_bai(PM25));
			//if(PM25>75)  Beep_long();
		}
		
		//**********心率采集程序**********//
		if(Pulseled==1)
		{		
			Gui_DrawFont_GBK16(16,130,BLUE,GRAY0,"Pulse:");
			Gui_DrawFont_GBK16(64,130,RED,GRAY0,Dis_bai(Pulse));
			if(bt30s)
			{
				bt30s=0;
				cnt1=0;
				if(Fall>60 || Fall <20)  Fall = 0;
				else 
				{
					Fall = Fall;
					Beep_single();
				}
				Pulse = Fall*2;
				Fall=0;
				
			}			
		}
		else if(Pulseled==0)
		{
			Gui_DrawFont_GBK16(16,130,BLUE,GRAY0,"         ");
		}	
		//Multiple_read_ADXL345();
		//**********ADXL345采集程序**********//
		if(num2 == 1)
		{		
			if(bt1s)
			{
				bt1s=0;
				cnt2=0;
				Multiple_read_ADXL345();				
				if(time == 1)
				{
					X1=Xdata;
					Y1=Ydata;
					Z1=Zdata;
				}
				else if(time == 2)
				{
					X2=Xdata;
					Y2=Ydata;
					Z2=Zdata;
				}
				if((abs(X2-X1)>800)||(abs(Y2-Y1)>800)||(abs(Z2-Z1)>800))
				{
					led=~led;					
					SendMessage();
					Beep_single();
				}
				if(time==2) time = 0;
				time++;			
			}
	  }
		
		
		
		//**********按键采集程序**********//
		Key_Value=KeyScan();
		MP3();
		if(Key_Value == 'T')
		{
			Flan1=1;
			goto Set_Mode;//进入参数设置模式
		}
		if(Key_Value == 'F')
		{
			num2++; 
			if(num2>2) num2 = 1;
			if(num2 == 1) Fallled=0;
			else if(num2 == 2) Fallled=1;
		}
		if(Key_Value == 'U')
		{
			ATD();
		}
		if(Key_Value == 'D')
		{
			ATH();
		}
		if(Key_Value == 'P')
		{
			num1++; 
			if(num1>2) num1 = 1;
			if(num1 == 2)
			{
				Pulseled=1;
				cnt1=0;
				Fall=0;
				Pulse=0;
				EX0=1;
			}
			else if(num1 == 1)
			{
				Pulseled=0;
				EX0=0;
			}
		}
		
// 		if(KeyTime>TimeHoldTime)//Pulse按键长按检测
// 		{
// 			KeyTime=0;
//			
// 		}
	}while(1);
Set_Mode://参数设置模式
	Lcd_Clear(GRAY0);	
	do{
		Set_Time();
		Dis_Time();
		Key_Value=KeyScan();
		if(Key_Value == 'T')
		{
			Set_RTC();
			goto Normal_Mode;//进入参数设置模式
		}
	}while(1);
}

void Exint0() interrupt 0
{
	Fall++;
}
void Timer0() interrupt 1
{
	TL0 = 0x68;		//??????
	TH0 = 0xC6;		//??????
	if(cnt0<500) cnt0++;
	else
	{
		cnt0=0;
		bt500ms=1;
	}
	if(cnt2<1000) cnt2++;
	else
	{
		cnt2=0;
		bt1s=1;
	}
	if(cnt3<1000) cnt3++;
	else
	{
		cnt3=0;
		bt1000ms=1;
	}
	if(cnt1<30000) cnt1++;
	else
	{
		cnt1=0;
		bt30s=1;
	}
}

void Serial_2Int() interrupt 8
{
	IE2=0x00;         //?????,???????
	if(S2CON&0x02)      //?????2????(S2TI)?????
	{
		S2CON&=0xFD;  //S2TI?0
	}
	if(S2CON&0x01)    
	{
		S2CON&=0xFE;  //S2RI?0
		Rec_Buf2[count2]=S2BUF;
		if(Rec_Buf2[count2] == 0xaa)	
		{
			//PM25_Read=1;
			count2 = 20;
		}
		count2++;
		if(count2>Buf_Max2)
		{
			count2=0;
			//PM25_Read=0;
		} 
	}
	IE2=0x01;         //??????
}