/*
 * Lab7.c
 *
 * Created: 10/26/2021 9:22:09 AM
 * Author : luka0
 * RedID: 825209977
 * X = 9 = 10ms for timer 0
 * Y = 7 % 6 = 1
 * Z = 7 = 1.25ms duty cycle
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>

#define LEDON PORTB |= (1 << PORTB5)
#define LEDOFF PORTB &= ~(1 << PORTB5)


void adc_init(void) {
	//ADMUX &= 0b00010000;											// clear bits except reserved bit
	//ADMUX |= 0b01000001;											// set VCC reference and ADC1
	
	ADMUX |= (1 << REFS0) | (1 << MUX0);							// input channel
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE) |			// enable adc
			  (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);								
	ADCSRB |= (1 << ADTS0) | (1 << ADTS1);							// compare match	
	DDRC &= ~(1 << PINC1);											// set pin 1 in PORTC as input
	DDRB |= (1 << DDB5);											// set LED as output
}


void timer0_init() {
	TCCR0A |= (1 << WGM01);											// set timer 0 to CTC mode
	TCCR0B |= (1 << CS02);											// set pre-scaler to 1024
	TIMSK0 |= (1 << OCIE0A);										// overflow interrupt
	OCR0A = 0x9F;													// set top value for timer 0

}

void timer2_init() {
	TCCR2A |= (1 << WGM21);											// set timer 2 to CTC mode
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);						// enable interrupts for A and B
	TCCR2B |= (1 << CS21) | (1 << CS22);							// set pre-scaler to 256
	OCR2A =  0x4D;													// set top value for timer 2
}

ISR(TIMER0_COMPA_vect) {
	ADCSRA |= (1 << ADSC);											// timer 0 overflow occurs, start conversion
}

ISR(ADC_vect) {
	OCR2B = OCR2A * (ADC / 1023.0);									// calculate OCR2B
}

ISR(TIMER2_COMPA_vect) {
	LEDON;
}

ISR(TIMER2_COMPB_vect) {
	LEDOFF;
}

int main(void)
{
	timer0_init();
	timer2_init();												
	adc_init();
	sei();
		
	while (1);
}

