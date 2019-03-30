#include "STC12C5A60S2.h"
#include "DataDef.h"
#include "Delays.h"
#include "keyscan.h"
#include "DS1302.h"



// bit U_D = 0;
// uchar Flan = 1;
// uchar Flan1 = 1;
// uchar j = 0;
uchar Flan1=1;
uchar Key_Value;
uchar KeyHold;
uint KeyTime;
// unsigned char xdata ss1[16]="1234567890123456";
// unsigned char xdata aa1[16]="1234567890123456";
uchar KeyScan(void)
{
	Key_Pulse  = 1;
	Key_Left   = 1;
	Key_Right  = 1;
	Key_Up     = 1;
	Key_Down   = 1;
	Key_Stop   = 1;
	Key_Time   = 1;
	Key_Fall   = 1;
	
	if(Key_Pulse==0)
	{
		KeyTime=0;
		if(KeyHold!='P')
		{
			KeyHold='P';
			return 'P';
		}
	}
	else if(Key_Fall==0)
	{
		KeyTime=0;
		if(KeyHold!='F')
		{
			KeyHold='F';
			return 'F';
		}
	}
	else if(Key_Left==0)
	{
		KeyTime=0;
		if(KeyHold!='L')
		{
			KeyHold='L';
			return 'L';
		}
	}
	else if(Key_Right==0)
	{
		KeyTime=0;
		if(KeyHold!='R')
		{
			KeyHold='R';
			return 'R';
		}
	}
	else if(Key_Up==0)
	{
		KeyTime=0;
		if(KeyHold!='U')
		{
			KeyHold='U';
			return 'U';
		}
	}
	else if(Key_Down==0)
	{
		KeyTime=0;
		if(KeyHold!='D')
		{
			KeyHold='D';
			return 'D';
		}
	}
	else if(Key_Stop==0)
	{
		KeyTime=0;
		if(KeyHold!='S')
		{
			KeyHold='S';
			return 'S';
		}
	}
	else if(Key_Time==0)
	{
		KeyTime++;
		if(KeyHold!='T')
		{
			KeyHold='T';
			return 'T';
		}
	}
	else//无按键动作时
	{
		KeyTime=0;
		KeyHold=0;
		return 0;
	}
	
	return 0xFF;//长按时返回0xFF
}

void Set_Time()
{
	
		if(Key_Value=='R')
		{
			Flan1++;
			if(Flan1 > 6)	Flan1 = 6;
		}
		else if(Key_Value=='L')
		{
			Flan1--;
			if(Flan1 < 1)	Flan1 = 1;
		}
		if(Flan1 == 1)
		{
			if(Key_Value=='U')
			{
				Year++;
				if(Year > 99)	Year = 0;
			}
			else if(Key_Value=='D')
			{
				Year--;
				if(Year < 0)	Year = 99;
			}
		}
		else if(Flan1 == 2)
		{
			if(Key_Value=='U')
			{
				Month++;
				if(Month > 12)	Month = 1;
			}
			else if(Key_Value=='D')
			{
				Month--;
				if(Month < 1)	Month = 12;
			}
		}
		else if(Flan1 == 3)
		{
			if(Key_Value=='U')
			{
				Day++;
				if(Day > 31)	Day = 1;
			}
			else if(Key_Value=='D')
			{
				Day--;
				if(Day < 1)	Day = 31;
			}
		}
		else if(Flan1 == 4)
		{
			if(Key_Value=='U')
			{
				Hour++;
				if(Hour > 23)	Hour = 0;
			}
			else if(Key_Value=='D')
			{
				Hour--;
				if(Hour < 0)	Hour = 23;
			}
		}
		else if(Flan1 == 5)
		{
			if(Key_Value=='U')
			{
				Minute++;
				if(Minute > 59)	Minute = 0;
			}
			else if(Key_Value=='D')
			{
				Minute--;
				if(Minute < 0)	Minute = 59;
			}
		}
		else if(Flan1 == 6)
		{
			if(Key_Value=='U')
			{
				Second++;
				if(Second > 59)	Second = 0;
			}
			else if(Key_Value=='D')
			{
				Second--;
				if(Second < 0)	Second = 59;
			}
		}
}
void MP3(void)
{
//	if(Key_Value=='U')
//	{
//		Up = 0;
//		delay_ms(100);
//		Up = 1;
//	}
//	else if(Key_Value=='D')
//	{
//		Down = 0;
//		delay_ms(100);
//		Down = 1;
//	}
	if(Key_Value=='L')
	{
		Left = 0;
		delay_ms(100);
		Left = 1;
	}
	else if(Key_Value=='R')
	{
		Right = 0;
		delay_ms(100);
		Right = 1;
	}
}
	