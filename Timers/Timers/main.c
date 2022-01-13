/*
 * Timers.c
 *
 * Created: 10/18/2021 11:12:06 AM
 * Author : luka0
 */ 

#include <avr/io.h>

#define LEDON PORTB |= (1 << 5);
#define LEDOFF PORTB &= ~(1 << 5);

void init() {
	TCCR0A |= (1 << WGM01);							// set timer to CTC mode
	TCCR0B |= (1 << CS01) | (1 << CS00);			// set pre-scaler to 64
	DDRB |= (1 << DDB5);							// set LED as output (1)
	OCR2A = 249;
	OCR2B = 124;
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		LEDON;										// turn LED on
		while(!(TIFR0 & (1 << OCF0B)));				// wait for the overflow event
		TIFR0 |= (1 << OCF0B);						// reset the overflow flag
		
		LEDOFF;									// turn LED off
		while(!(TIFR0 & (1 << OCF0A)));				// wait for the overflow event
		TIFR0 |= (1 << OCF0A);						// reset the overflow flag
    }
}

