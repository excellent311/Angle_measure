#include <msp430g2553.h>
#include "delay.h"
#include "usart.h"
#include "lewei.h"
#include "stdio.h"
#include "temp.h"
#include "yeelink.h"

//char SSID[]="DSKY";
//char PASSWORD[]="01234560";

char SSID[]="HFUT_Electronic_Design";
char PASSWORD[]="123456789";

#define DEVICEID       "342502"
#define SENSORID0      "379895"

void CLK_Init(void)
{
	BCSCTL1 = CALBC1_16MHZ;                    // Set DCO
	DCOCTL = CALDCO_16MHZ;

}
/*
void WiFi_Init(void)
{
	char str[40]="0";

	put("AT\r\n");
	delay_ms(2000);
	sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASSWORD);
	put(str);
	delay_ms(5000);
}
*/
/*
 * main.c
 */
void main(void) {

	volatile float temp;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	//char str[40]="0";

    CLK_Init();
    Usart_Init();
    InitTemp();

    P2DIR = BIT0;
    P2OUT = BIT0;
	delay_ms(5000);
	//sprintf(str,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,PASSWORD);
	put("AT+CWJAP=\"HFUT_Electronic_Design\",\"123456789\"\r\n");
	delay_ms(10000);
   // WiFi_Init();
    while(1)
    {


        temp=(float)GetTemp();
        temp=(temp/1023*1.5-0.986)/0.00355;


        yeelink_send(DEVICEID,SENSORID0,temp);
    	delay_ms(15000);

/*
        P2OUT = 0;
        delay_ms(2000);
        P2OUT=BIT0;
        delay_ms(15000);
*/
    	//lewei_send(temp);

       // delay_ms(15000);





        P2OUT = 0;
        delay_ms(10000);
        P2OUT=BIT0;
        delay_ms(15000);

    }

}
