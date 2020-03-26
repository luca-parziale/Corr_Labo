/**
 * @brief Labo 3 program 3.1
 * @file labo3_31.c
 * @author gregory.medwed@heig-vd.ch
 * @date March 2017
 */
 
#include <msp430.h>


/**
 * Blink PAD1 LED ever 1 second using timer
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	P1DIR |= 0x01;
	
	TA0CCTL0 = CCIE;	// Enable compare interrupt
	TA0CCR0 = 50000;	// Count until 50000 is reached, it will take 50000/32.768 kHz = 1.5 second
	TA0CTL = TASSEL__ACLK + MC__CONTINUOUS + TACLR;	// ACLK=32768 Hz, count up to CCR0 then clear counter
	
	__enable_interrupt();

	while (1);

	return 0;
}


/**
 * Timer0 A0 interrupt service routine
 * Triggered every time TACCR0 value is match
 */
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_CCR0_ISR(void)
{
	P1OUT ^= 0x01;
}
