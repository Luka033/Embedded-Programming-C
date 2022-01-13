/*
 * Lab8.c
 *
 * Created: 11/3/2021 7:46:48 AM
 * Author : luka0
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define LEDON PORTB |= (1 << PORTB5)
#define LEDOFF PORTB &= ~(1 << PORTB5)

uint8_t EEMEM addressLocation;										// location to write to (figured out by compiler)


ISR(TIMER0_COMPA_vect) {
	LEDON;
}

ISR(TIMER0_COMPB_vect) {
	LEDOFF;
}

void timer0_init() {
	TCCR0A |= (1 << WGM01);											// set timer 0 to CTC mode
	TCCR0B |= (1 << CS02) | (1 << CS00);							// set pre-scaler to 1024
	TIMSK0 |= (1 << OCIE0A) | (1 << OCIE0B);						// enable interrupts for A and B
	OCR0A = 0x4D;													// set top value 
	OCR0B = 0x00;													// duty cycle
}

void updateDutyCycle() {
	uint8_t dutyCycle = eeprom_read_byte(&addressLocation);			// read the most recently stored value in eeprom
	dutyCycle = (dutyCycle >= 100) ? 0 : dutyCycle + 10;			// set the duty cycle based on conditional
	OCR0B = OCR0A * (dutyCycle / 100.0);							// set new duty cycle value
	eeprom_update_byte(&addressLocation, dutyCycle);				// update new duty cycle value to eeprom
}

int main(void)
{
	DDRB |= (1 << DDB5);											// set LED as output
	timer0_init();
	updateDutyCycle();
	sei();
		
	while (1);
}

