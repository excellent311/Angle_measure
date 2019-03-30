#ifndef	__DS1302_H__
#define	__DS1302_H__
#include "DataDef.h"

extern char xdata Year;
extern char xdata Month;
extern char xdata Day;
extern char xdata Hour;
extern char xdata Minute;
extern char xdata Second;
void DS1302init();
void Read_RTC(void);
void Set_RTC(void);
void Dis_Time(void);
#endif