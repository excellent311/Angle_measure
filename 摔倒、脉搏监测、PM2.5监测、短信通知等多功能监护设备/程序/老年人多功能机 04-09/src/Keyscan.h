#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__
#include "DataDef.h"

sbit Key_Pulse  = P2^1;
sbit Key_Stop   = P2^6;
sbit Key_Time   = P2^3;
sbit Key_Left   = P2^0;
sbit Key_Right  = P2^5;
sbit Key_Up     = P2^4;
sbit Key_Down   = P2^2;
sbit Key_Fall   = P0^0;
sbit Up   = P3^5;
sbit Down = P3^4;
sbit Left = P3^7;
sbit Right= P3^3;
#define TimeHoldTime 3
extern uchar Key_Value;
extern uint KeyTime;
extern uchar Flan1;
extern uchar KeyScan(void);
void Set_Time();
void MP3(void);
#endif