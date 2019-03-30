#include "Delays.h"

//void Delay(unsigned int t)
//{
//	while(t--);
//}

void delay_1us(void)//—” ±1us
{
	unsigned char i=3;
	while(i--);
}

void delay_us(unsigned char t)
{
	unsigned char i;
	i=t;
	while(i--)
	{
		delay_1us();
	};
}

#define MAIN_Fosc 22118400L//11059200L
void delay_ms(unsigned char ms)
{
	unsigned int i;
	do{
		i = MAIN_Fosc / 13000;
		while(--i);//14T per loop
	}while(--ms);
}