/*
 * Lab4.c
 *
 * Created: 9/23/2021 3:23:49 PM
 * Author : luka0
 * Description: Simulates an analog LED output when a button is pressed/released
 */ 

#include <avr/io.h>
#define F_CPu 16000000UL	


void init() {
	TCCR0A |= (1 << WGM01);							// set timer to CTC mode
	TCCR0B |= (1 << CS02) | (1 << CS00);			// set pre-scaler to 1024
	DDRB &= ~(1 << DDB7);							// set button as input (0)
	DDRB |= (1 << DDB5);							// set LED as output (1)
	PORTB |= (1 << DDB7);							// enable pull-up resistor for button
	OCR0A = 0x9B;									// top value (100%)
	OCR0B = 0x00;									// duty cycle (0%)
}

void ledOn() {
	PORTB |= (1 << 5);
}

void ledOff() {
	PORTB &= ~(1 << 5);
}

int main(void)
{
	init();
    while (1) 
    {
		if(!(PINB & (1 << 7)) && (OCR0B < OCR0A)) { // check button pressed
			OCR0B++;								// increment duty cycle
		}
		else if (PINB & (1 << 7) && (OCR0B > 0)) {	// check button released
			OCR0B--;								// decrement duty cycle
		}
		
		ledOn();									// turn LED on
		while(!(TIFR0 & (1 << OCF0B)));				// wait for the overflow event
		TIFR0 |= (1 << OCF0B);						// reset the overflow flag
		
		ledOff();									// turn LED off 
		while(!(TIFR0 & (1 << OCF0A)));				// wait for the overflow event
		TIFR0 |= (1 << OCF0A);						// reset the overflow flag
    }	
}

