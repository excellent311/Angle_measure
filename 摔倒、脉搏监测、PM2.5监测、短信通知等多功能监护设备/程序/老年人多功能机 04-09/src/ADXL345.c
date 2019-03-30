#include "STC12C5A60S2.h"
#include "Delays.h"
#include "math.h"     
#include "stdio.h" 
#include "ADXl345.h"
#include "DataDef.h"
#include "LCD.h"

sbit  SCL1=P0^1;      //IIC??????
sbit  SDA1=P0^2;      //IIC??????

uchar BUF[8]; 
// uchar ss[7]="1234567";
//float temp;
int Xdata,Ydata,Zdata;
#define SlaveAddress   0xA6   //?????IIC???????,??ALT  ADDRESS????????

void ADXL345_Start()
{
    SDA1 = 1;                    //?????
    SCL1 = 1;                    //?????
    delay_us(5);                 //??
    SDA1 = 0;                    //?????
    delay_us(5);                 //??
    SCL1 = 0;                    //?????
}
/**************************************
????
**************************************/
void ADXL345_Stop()
{
    SDA1 = 0;                    //?????
    SCL1 = 1;                    //?????
    delay_us(5);                 //??
    SDA1 = 1;                    //?????
    delay_us(5);                 //??
}
/**************************************
??????
????:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    SDA1 = ack;                  //?????
    SCL1 = 1;                    //?????
    delay_us(5);                 //??
    SCL1 = 0;                    //?????
    delay_us(5);                 //??
}
/**************************************
??????
**************************************/
bit ADXL345_RecvACK()
{
    SCL1 = 1;                    //?????
    delay_us(5);                 //??
    CY = SDA1;                   //?????
    SCL1 = 0;                    //?????
    delay_us(5);                 //??
    return CY;
}
/**************************************
?IIC??????????
**************************************/
void ADXL345_SendByte(uchar dat)
{
    uchar i;
    for (i=0; i<8; i++)         //8????
    {
        dat <<= 1;              //????????
        SDA1 = CY;               //????
        SCL1 = 1;                //?????
        delay_us(5);             //??
        SCL1 = 0;                //?????
        delay_us(5);             //??
    }
    ADXL345_RecvACK();
}
/**************************************
?IIC??????????
**************************************/
uchar ADXL345_RecvByte()
{
    uchar i;
    uchar dat = 0;
    SDA1 = 1;                    //??????,??????,
    for (i=0; i<8; i++)         //8????
    {
        dat <<= 1;
        SCL1 = 1;                //?????
        delay_us(5);             //??
        dat |= SDA1;             //???               
        SCL1 = 0;                //?????
        delay_us(5);             //??
    }
    return dat;
}
//******?????*******************************************
 
void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //????
    ADXL345_SendByte(SlaveAddress);   //??????+???
    ADXL345_SendByte(REG_Address);    //???????,?????pdf22? 
    ADXL345_SendByte(REG_data);       //???????,?????pdf22? 
    ADXL345_Stop();                   //??????
}
//********?????*****************************************
// uchar Single_Read_ADXL345(uchar REG_Address)
// {  
//     uchar REG_data;
//     ADXL345_Start();                          //????
//     ADXL345_SendByte(SlaveAddress);           //??????+???
//     ADXL345_SendByte(REG_Address);            //????????,?0?? 
//     ADXL345_Start();                          //????
//     ADXL345_SendByte(SlaveAddress+1);         //??????+???
//     REG_data=ADXL345_RecvByte();              //???????
//     ADXL345_SendACK(1);   
//     ADXL345_Stop();                           //????
//     return REG_data; 
// }
//*********************************************************
//????ADXL345???????,????0x32~0x37
//*********************************************************
void Multiple_read_ADXL345(void)
{   
    uchar i;
    ADXL345_Start();                          //????
    ADXL345_SendByte(SlaveAddress);           //??????+???
    ADXL345_SendByte(0x32);                   //????????,?0x32?? 
    ADXL345_Start();                          //????
    ADXL345_SendByte(SlaveAddress+1);         //??????+???
    for (i=0; i<6; i++)                      //????6?????,???BUF
    {
        BUF[i] = ADXL345_RecvByte();          //BUF[0]??0x32??????
        if (i == 5)
        {
           ADXL345_SendACK(1);                //?????????NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //??ACK
        }
    }
    ADXL345_Stop();                          //????
    delay_ms(5);
		Xdata=(int)((BUF[1]<<8)+BUF[0])*3.9;   
  	Ydata=(int)((BUF[3]<<8)+BUF[2])*3.9;
 	  Zdata=(int)((BUF[5]<<8)+BUF[4])*3.9;
}

// void conversion(uint temp_data)  
// {  
//   ss[0]='X';
// 	ss[1]=':';  
// 	ss[2]=temp_data/1000+0x30 ;  //??
// 	temp_data=temp_data%1000;    //????
// 	ss[3]='.';
// 	ss[4]=temp_data/100+0x30;  //??
// 	temp_data=temp_data%100;     //????
// 	ss[5]=temp_data/10+0x30;  //??
// 	temp_data=temp_data%10;      //????
// 	ss[6]=temp_data+0x30;   //??
// }
// uchar *Show_xyz(void)
// {   
// 	Multiple_read_ADXL345(); 
// 	dis_Xdata=(float)((BUF[1]<<8)+BUF[0])*3.9;   
// 	dis_Ydata=(float)((BUF[3]<<8)+BUF[2])*3.9;
// 	dis_Zdata=(float)((BUF[5]<<8)+BUF[4])*3.9;
// 	if(dis_Xdata<0)   dis_Xdata=-dis_Xdata;
//  	conversion(dis_Xdata);          //??????????
// 	return ss;
// }

//*****************************************************************
//???ADXL345,???????pdf????************************
void Init_ADXL345()
{
    Single_Write_ADXL345(0x31,0x0B);   //????,??16g,13???
    Single_Write_ADXL345(0x2C,0x08);   //?????12.5 ??pdf13?
    Single_Write_ADXL345(0x2D,0x08);   //??????   ??pdf24?
    Single_Write_ADXL345(0x2E,0x80);   //?? DATA_READY ??
    Single_Write_ADXL345(0x1E,0x00);   //X ??? ????????????pdf29?
    Single_Write_ADXL345(0x1F,0x00);   //Y ??? ????????????pdf29?
    Single_Write_ADXL345(0x20,0x05);   //Z ??? ????????????pdf29?
}