#include "STC12C5A60S2.h"
#include "Delays.h"
#include "DataDef.h"
#include "stdio.h"
#include "string.h"
#include "Uart.h"

#define Buf_Max 100   //���泤��
uchar  xdata Rec_Buf[Buf_Max];
uchar count=0;
//**************************���ڳ�ʼ������
void Serial_Init()
{
	//**9600bps@22.1184MHz@1T**//
	PCON &= 0x7F;		//??????
	SCON = 0x50;		//8???,?????
	AUXR |= 0x40;		//???1???Fosc,?1T
	AUXR &= 0xFE;		//??1?????1???????
	TMOD &= 0x0F;		//?????1???
	TMOD |= 0x20;		//?????1?8???????
	TL1 = 0xB8;		//??????
	TH1 = 0xB8;		//????????
	ET1 = 0;		//?????1??
	TR1 = 1;		//?????1
	EA  = 1;
	ES  = 1;
	//**2400bps@22.1184MHz@12T**//����TxD2/P1.3;����RxD2/P1.2
	AUXR &= 0xF7;		//??????
	S2CON = 0x50;		//8???,?????
	AUXR &= 0xFB;		//???????????Fosc/12,?12T
	BRT = 0xE8;		//?????????????
	AUXR |= 0x10;		//??????????
	IE2=0x01;  
}

//***************************�������������
void CLR_Buf()
{
	 uchar k;
	 for(k=0;k<Buf_Max;k++)   //��ջ�����
	
	  {
	   Rec_Buf[k]=0;
	  }
	 count=0;      //�����ַ�������ʼ�洢λ��
}

//**************************���ڷ����ַ�����
void Send_Char(uchar a)
{
	 ES=0;
	 SBUF=a;
	 while(TI==0);
	 TI=0;
	 ES=1; 
}

//**************************���ڷ����ַ�������
void Send_String(uchar *tab)
{        
	 ES=0;       
	 while(*tab!='\0')    
	  {
	   Send_Char(*tab++);  //Send_Char(*tab); tab++;  
	  }
	 ES=1;
}

// //**************************���ڷ����ַ�����
// void Send_Char2(uchar a)
// {
// 	 IE2=0x00;
// 	 S2BUF=a;
// 	 while(!(S2CON&0x02));
// 	 S2CON&=0xFD;
// 	 IE2=0x01; 
// }

// //**************************���ڷ����ַ�������
// void Send_String2(uchar *tab)
// {        
// 	 IE2=0x00;      
// 	 while(*tab!='\0')    
// 	  {
// 	   Send_Char2(*tab++);  //Send_Char(*tab); tab++;  
// 	  }
// 	 IE2=0x01;
// }
//***************************�������ֺ���
bit Hand(uchar *ptr)
{
	if(strstr(Rec_Buf,ptr)!=NULL)
		return 1;
	else
		return 0;
}

void ATD()
{
	Send_String("ATD13790371594;\r\n"); 
	delay_ms(100);
}
void ATH()
{
	Send_String("ATH\r\n"); 
	delay_ms(100);
}
void SendMessage()
{
	//CLR_Buf();
	Send_String("AT\r\n"); //����ָ��(AT+�س�+����)
	delay_ms(100);
	//while(!Hand("OK"))
	//CLR_Buf();
	Send_String("AT+CSCS=\"GSM\"\r\n"); //��GSM���ַ���
	delay_ms(100);
	//while(!Hand("OK"));
	//CLR_Buf();
	Send_String("AT+CMGF=1\r\n"); //����Ӣ�Ķ���
	delay_ms(100);
	//while(!Hand("OK"));
	//CLR_Buf();
	Send_String("AT+CMGS=\"13790371594\"\r\n"); //����Ӣ�Ķ��
	delay_ms(100);
	//while(!Hand(">"));
	//CLR_Buf();
	Send_String("Warning"); //����Ӣ�Ķ���
	delay_ms(100);
	Send_Char(0X1A);
  //CLR_Buf();	
}
//void GSMInit()
//{
////  CLR_Buf(); 
//	Send_String("AT\r\n"); //����ָ��(AT+�س�+����)
//	delay_ms(100);
////	while(!Hand("OK"))
////	{
////	  Send_String("AT\r\n");
////	   delay_ms(100);  
////	}
////	CLR_Buf();
//	Send_String("AT+CMGF=1\r\n"); //����Ӣ�Ķ���
//	delay_ms(100);
////	while(!Hand("OK"));
//// 	CLR_Buf();
// 	Send_String("AT+CNMI=2,1\r\n"); //�����¶���ʱ��ʾ����ʾ����λ��
// 	delay_ms(100);
//// 	while(!Hand("OK"));
//// 	CLR_Buf();
// 	Send_String("AT+CMGD=1\r\n");  //ɾ��1��λ�ö���
// 	delay_ms(100);
//// 	while(!Hand("OK"));
//// 	CLR_Buf();
//// 	Send_String("AT+CLIP=1\r\n");  //
//// 	delay_ms(100);
//// 	while(!Hand("OK"));
//}

void Serial_1Int() interrupt 4 
{
	ES = 0;
	if(TI){TI=0;}
	if(RI)
	{
		RI=0;		
		Rec_Buf[count]=SBUF;
		count++;
		if(count>Buf_Max)
    {
     	count=0;
    }
	}	
	ES = 1;
}


	