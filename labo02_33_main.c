/**
 * @brief Labo02 program 3.3
 * @file labo02_33_main.c
 * @author gregory.medwed@heig-vd..ch
 * @date March 2017
 */


#include <exp430f5529/bsp.h>
#include <exp430f5529/led.h>
#include <exp430f5529/portio.h>
#include <exp430f5529/switch.h>


#define DELAY_PAD5		(500000)
#define DELAY_DEBOUNCE	(40000)

/**
 * Button state
 */
typedef enum _ButtonState_t
{
	BUTTON_DOWN = 0,
	BUTTON_UP
} ButtonState_t;


/*
 * GLOBAL variables
 */
volatile ButtonState_t buttonS1State = BUTTON_UP;
volatile bool buttonS1Event = false;


/**
 * Blink PAD5 LED every 0.5 second and
 * Turns ON PAD1, PAD2 & PAD3 when S1 is pressed OFF when released
 */
int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Init eval board I/O
	ports_io_init();

	port_irq_init(PORT1, BIT7, HIGH_TO_LOW);
	port_irq(PORT1, BIT7, ENABLE);

	__enable_interrupt();

	// Main loop
	while (1)
	{

		led_pad5(LED_TOGGLE);
		__delay_cycles(DELAY_PAD5);

		if (buttonS1Event)
		{
			__delay_cycles(DELAY_DEBOUNCE);
			buttonS1Event = false;
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
	// Check which IO triggered an interrupt
	if ((P1IFG & BIT7) == BIT7)
	{
		if (buttonS1State == BUTTON_UP)
		{
			P1IES &= ~BIT7;		// LOW_TO_HIGH edge
			buttonS1State = BUTTON_DOWN;
			buttonS1Event = true;
			led_pad1(ON);
			led_pad2(ON);
			led_pad3(ON);
		}
		else
		{
			P1IES |= BIT7;	// HIG_TO_LOW edge
			buttonS1State = BUTTON_UP;
			buttonS1Event = true;
			led_pad1(OFF);
			led_pad2(OFF);
			led_pad3(OFF);

		}
		P1IFG &= ~BIT7;	// Clear interrupt flag
	}
}
