/**
 * @brief Labo02 program 3.1
 * @file labo02_31_main.c
 * @author gregory.medwed@heig-vd..ch
 * @date March 2017
 */


#include <exp430f5529/bsp.h>
#include <exp430f5529/led.h>
#include <exp430f5529/portio.h>
#include <exp430f5529/switch.h>


#define PAD5_DELAY	(500000)

/**
 * Blink PAD5 LED every 0.5 second
 */
int main(void)
{

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Init eval board I/O
	ports_io_init();


	// Main loop
	while (1)
	{

		led_pad5(LED_TOGGLE);
		__delay_cycles(PAD5_DELAY);
	}

	return 0;
}
