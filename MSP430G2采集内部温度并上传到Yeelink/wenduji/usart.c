/*
 * usart.c
 *
 *  Created on: 2015-11-5
 *      Author: Administrator
 */
#include"usart.h"
volatile char USART_CMD[2];
volatile int ESP8266_OK;
void put(char *x)
{
  while(*x)
  {
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    UCA0TXBUF=*x;
    x++;
  }
}

void Usart_Init(void)
{
		P1SEL  = BIT1 + BIT2;                     // P1.1 = RXD, P1.2=TXD
		P1SEL2 = BIT1 + BIT2;
		UCA0CTL1 |= UCSSEL_2;                     // SMCLK
		UCA0BR0 = 131;                              // 16MHz 9600
		UCA0BR1 = 6;                              // 16MHz 9600
		UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
		UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
		IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
		delay_us(100);
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
		while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
		//UCA0TXBUF = UCA0RXBUF;                    // TX -> RXed character
		USART_CMD[1]=USART_CMD[0];
		USART_CMD[0]=UCA0RXBUF;


		if(USART_CMD[1]=='O'&&USART_CMD[0]=='K')
		{
			ESP8266_OK=3;
		}
}


