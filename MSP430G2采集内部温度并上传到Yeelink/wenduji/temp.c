#include"temp.h"
#include"msp430g2553.h"
/*
 * temp.c
 *
 *  Created on: 2014-12-4
 *      Author: Administrator
 */

void InitTemp(void)
{
	ADC10CTL0 = SREF_1 + ADC10SHT_2 + ADC10IE + REFON + ADC10ON;
	ADC10CTL1 = INCH_10 + ADC10SSEL_3 + ADC10DIV_3;
}
int GetTemp(void)
{
   ADC10CTL0 |= ENC + ADC10SC;
   __delay_cycles(20);
   __bis_SR_register(CPUOFF + GIE);
   return ADC10MEM;
}

#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
   __bic_SR_register_on_exit(CPUOFF);
}

