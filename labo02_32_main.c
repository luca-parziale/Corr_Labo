/**
 * @brief Labo02 program 3.2
 * @file labo02_32_main.c
 * @author gregory.medwed@heig-vd..ch
 * @date March 2017
 */


#include <exp430f5529/bsp.h>
#include <exp430f5529/led.h>
#include <exp430f5529/portio.h>
#include <exp430f5529/switch.h>


#define DELAY_PAD5		(500000)
#define DELAY_DEBOUNCE	(50000)

/*
 * GLOBAL variables
 */
volatile bool buttonS1Pressed = false;
volatile bool buttonS2Pressed = false;


/**
 * Blink PAD5 LED every 0.5 second and
 * switchs S1/S2 turns ON/OFF PAD3
 */
int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Init eval board I/O
	ports_io_init();

	port_irq_init(PORT1, BIT7, LOW_TO_HIGH);
	port_irq_init(PORT2, BIT2, LOW_TO_HIGH);

	port_irq(PORT1, BIT7, ENABLE);
	port_irq(PORT2, BIT2, ENABLE);
	
	__enable_interrupt();

	// Main loop
	while (1)
	{

		led_pad5(LED_TOGGLE);
		__delay_cycles(DELAY_PAD5);

		if (buttonS1Pressed)
		{
			__delay_cycles(DELAY_DEBOUNCE);
			buttonS1Pressed = false;
		}
		else if (buttonS2Pressed)
		{
			__delay_cycles(DELAY_DEBOUNCE);
			buttonS2Pressed = false;
		}

	}

	return 0;
}


/**
 * Port1 interrupt service routine
 */
#pragma vector=PORT1_VECTOR
__interrupt void port1_isr(void)
{
	if (buttonS1Pressed)
	{
		P1IFG = 0;	// Clear all interrupt flags
		return;
	}

	// Check which IO fired an interrupt
	if ((P1IFG & BIT7) == BIT7)
	{
		buttonS1Pressed = true;
		led_pad3(LED_TOGGLE);
		P1IFG &= ~BIT7;	// Clear interrupt flag
	}
}


/**
 * Port2 interrupt service routine
 */
#pragma vector=PORT2_VECTOR
__interrupt void port2_isr(void)
{
	if (buttonS2Pressed)
	{
		P2IFG = 0;	// Clear all interrupt flags
		return;
	}

	// Check which IO fired an interrupt
	if ((P2IFG & BIT2) == BIT2)
	{
		buttonS2Pressed = true;
		led_pad3(LED_TOGGLE);
		P2IFG &= ~BIT2;	// Clear interrupt flag
	}
}
