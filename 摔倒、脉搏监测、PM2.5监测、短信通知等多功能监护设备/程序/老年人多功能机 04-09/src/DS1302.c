#include "STC12C5A60S2.h"
#include "Delays.h"
#include "DS1302.h"
#include "DataDef.h"
#include "LCD.h"
sbit SCLK=P1^0;					 //ds1302����
sbit IO=P1^1;
sbit RES=P1^4;
sbit ACC0 = ACC^0;
sbit ACC7 = ACC^7;
char xdata Year=15;
char xdata Month=2;
char xdata Day=26;
char xdata Hour=14;
char xdata Minute=23;
char xdata Second=30;
uchar aa[2]="01";
void writebyte(uchar date)			//��1302д1�ֽڵ�����
{
	uchar i;
	RES=1;
	ACC=date;
	for(i=8;i>0;i--)
	{
	IO=ACC0;						 //��д���ֽڣ�����ֽ�
	SCLK=1;
	SCLK=0;					  //˵�������أ���ô���½����ˣ�
	ACC=ACC>>1;
	}		
}
uchar readbyte(void)					   //��1302��1�ֽڵ�����
{
	uchar i;
	RES=1;
	for(i=8;i>0;i--)
	{
		ACC=ACC>>1;
		ACC7=IO;
		SCLK=1;
		SCLK=0;
	}
	return(ACC);
}
void Write_DS1302(uint Adrr,uint Data)			 //��1302д����
{
	RES=0;
	SCLK=0;
	RES=1;
	writebyte(Adrr);				 //д��ַ
	_nop_();
	_nop_(); 
	writebyte(Data);				//д����
	SCLK=1;
	RES=0;
		
}
uchar Read_DS1302(uchar Adrr)				 //��1302����ַ������
{
	uchar Data1;
	RES=0;
	SCLK=0;
	RES=1;
	writebyte(Adrr);
	_nop_();
	_nop_();
	Data1=readbyte();					  //��һ����ַ�ͻ����һ�����ݣ�������	  ��д�ڶ�����ʲô����
	SCLK=1;
	RES=0;
	return Data1;
}
//uchar bcdtodec(uchar bcd)
//{
//	uchar data1;
//	data1 = bcd & 0x0f;     //ȡBCD��4λ
//	bcd = bcd & 0x70;       //�޳�BCD�����λ�͵�4λ��
//	data1 += bcd >> 1;
//	data1 += bcd >> 3;      //��λ�ƴ���˷�����
//	return data1;
//}

void Set_Col(uchar adress,uint dat)
{
	uchar tmp1,tmp2;

	tmp1 = dat/10;
	tmp2 = dat%10;
	tmp2 = tmp2+tmp1*16;
	Write_DS1302(0x8E,0x00);
	Write_DS1302(adress,tmp2);
	Write_DS1302(0x8E,0x80);
}
void DS1302init()				  //�ϵ�ʱ��
{
	Write_DS1302(0x8e,0);
	Set_Col(0x80,Second);
	Write_DS1302(0x8e,0);
	Set_Col(0x82,Minute);
	Write_DS1302(0x8e,0);
	Set_Col(0x84,Hour);
	Write_DS1302(0x8e,0);
	Set_Col(0x86,Day);
	Write_DS1302(0x8e,0);
	Set_Col(0x88,Month);
//	Write_DS1302(0x8e,0);
//	Write_DS1302(0x8a,0x07);		//����
	Write_DS1302(0x8e,0);
	Set_Col(0x8c,Year);

}
void Read_RTC(void)//��ȡ
{
	uchar tmp;	
	tmp=Read_DS1302(0x81)&0x7F;
	Second=tmp&0x0F;
	Second=Second+(tmp>>4)*10;
	
	tmp=Read_DS1302(0x83)&0x7F;
	Minute=tmp&0x0F;
	Minute=Minute+(tmp>>4)*10;
	
	tmp=Read_DS1302(0x85)&0x3F;
	Hour=tmp&0x0F;
	Hour=Hour+(tmp>>4)*10;
	
	tmp=Read_DS1302(0x87)&0x3F;
	Day=tmp&0x0F;
	Day=Day+(tmp>>4)*10;
	
	tmp=Read_DS1302(0x89)&0x1F;
	Month=tmp&0x0F;
	Month=Month+(tmp>>4)*10;
	
	tmp=Read_DS1302(0x8D)&0xFF;
	Year=tmp&0x0F;
	Year=Year+(tmp>>4)*10;
	//Year=Year+2000;
}
void Set_RTC(void)//�趨ʱ������
{
	Write_DS1302(0x8E,0x00);
	Set_Col(0x80,Second);
	Set_Col(0x82,Minute);
	Set_Col(0x84,Hour);
	Set_Col(0x86,Day);
	Set_Col(0x88,Month);
	Set_Col(0x8C,Year);
	Write_DS1302(0x8E,0x80);
}
uchar *Dis_shi(char temp_data)  
{  
	aa[0]=temp_data/10+0x30;  //??
	temp_data=temp_data%10;      //????
	aa[1]=temp_data+0x30;   //??
	return aa;
}
void Dis_Time(void)//Һ������ʾʱ������
{
	Gui_DrawFont_GBK16(16,30,BLUE,GRAY0,"20");
	Gui_DrawFont_GBK16(32,30,BLUE,GRAY0,Dis_shi(Year));
	Gui_DrawFont_GBK16(48,30,BLUE,GRAY0,"-");
	Gui_DrawFont_GBK16(56,30,BLUE,GRAY0,Dis_shi(Month));
	Gui_DrawFont_GBK16(72,30,BLUE,GRAY0,"-");
	Gui_DrawFont_GBK16(80,30,BLUE,GRAY0,Dis_shi(Day));
	
	Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,Dis_shi(Hour));
	Gui_DrawFont_GBK16(32,50,BLUE,GRAY0,":");
	Gui_DrawFont_GBK16(40,50,BLUE,GRAY0,Dis_shi(Minute));
	Gui_DrawFont_GBK16(56,50,BLUE,GRAY0,":");
	Gui_DrawFont_GBK16(64,50,BLUE,GRAY0,Dis_shi(Second));
}