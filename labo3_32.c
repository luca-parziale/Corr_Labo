/**
 * @brief Labo 3 program 3.2
 * @file labo3_32.c
 * @author gregory.medwed@heig-vd.ch
 * @date March 2017
 */
 
#include <msp430.h>

#include <exp430f5529/led.h>
#include <exp430f5529/portio.h>


/**
 * Blink LEDs as follow:
 * PAD1 every 4 seconds
 * PAD2 every 2 seconds
 * PAD3 every 1 second
 * PAD4 every 0.5 second
 * PAD5 every 0.25 second
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;
	
	// Init eval board I/O
	ports_io_init();
	
	// Init Timer A
	TA0CCR4 = 0x8000;	// Set Comparator values
	TA0CCR3 = 0x4000;
	TA0CCR2 = 0x2000;
	TA0CCR1 = 0x1000;

	TA0CCTL4 = CCIE;	// Enable compare interrupt
	TA0CCTL3 = CCIE;
	TA0CCTL2 = CCIE;
	TA0CCTL1 = CCIE;

	TA0CTL = TASSEL__ACLK + MC__CONTINUOUS + TACLR + TAIE; // ACLK=32768 Hz, continuous mode
	
	__enable_interrupt();

	while (1);

	return 0;
}


/**
 * Timer0 A1 interrupt service routine
 * Handle TACCR1, TACCR2, TACCR3, TACCR4 and TAIFG interrupt sources
 */
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
{
	switch (__even_in_range(TA0IV, TA0IV_TAIFG))
	{
	case TA0IV_TACCR1:
		led_pad5(LED_TOGGLE);
		TA0CCR1 += 0x1000;		// Update comparator value
		TA0CCTL1 &= ~CCIFG;		// Clear compare/capture interrupt flag
		break;
	case TA0IV_TACCR2:
		led_pad4(LED_TOGGLE);
		TA0CCR2 += 0x2000;
		TA0CCTL2 &= ~CCIFG;
		break;
	case TA0IV_TACCR3:
		led_pad3(LED_TOGGLE);
		TA0CCR3 += 0x4000;
		TA0CCTL3 &= ~CCIFG;
		break;
	case TA0IV_TACCR4:
		led_pad2(LED_TOGGLE);
		TA0CCR4 += 0x8000;
		TA0CCTL4 &= ~CCIFG;
		break;
	case TA0IV_TAIFG:		// Triggered when counter overflow (reach 0xFFFF)
		led_pad1(LED_TOGGLE);
		TA0CTL &= ~TAIFG;
		break;
	}
}
