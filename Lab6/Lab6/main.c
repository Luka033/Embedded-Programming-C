/*
 * Lab6.c
 *
 * Created: 10/3/2021 6:36:51 PM
 * Author : luka0
 * RedID: 825209977
 * X = 9 = 10ms, 1024 pre-scale = 16.384ms, 256 * 10 / 16.384 = 160 - 1 = 159
 * Z = 7 = 1.25ms,  1.25ms = 256 * 1.25 / 4.096 = 78 - 1 = 77
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>

#define LEDON PORTB |= (1 << PORTB5)	
#define LEDOFF PORTB &= ~(1 << PORTB5)
#define ROWS 4
#define COLS 4
#define PERCENTAGE 0.1

const int8_t buttons[ROWS][COLS] = {
	{1, 2, 3, -1},
	{4, 5, 6, -1},
	{7, 8, 9, -1},
	{-1, 0, -1, -1}};

void init(void) {
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);				// set rows as output
	DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);			// set columns as input
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);		// enable internal pull up resistors for columns
	PORTD |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);								// set all rows output as high
	DDRB |= (1 << DDB5);														// set LED as output
}


void scan_keypad() {	
	for(uint8_t i = 0; i < ROWS; i++) {
		PORTD &= ~(1 << (i + ROWS));											// set i:th row to low (offset by 4 since we start at pin 4)
		for(uint8_t j = 0; j < COLS; j++) {
			if(!(PINB & (1 << j))) {											// check which columns is pulled low
				int8_t keyPressed = buttons[i][j];
				if(keyPressed >= 0) {
					OCR2B = OCR2A * PERCENTAGE * keyPressed;					// set duty cycle
				}
			}
		}
		PORTD |= (1 << (i + ROWS));												// set the i:th row back to high (offset by 4 since we start at pin 4)
	}
}

void timer0_init() {
	TCCR0A |= (1 << WGM01);														// set timer 0 to CTC mode
	TCCR0B |= (1 << CS02) | (1 << CS00);										// set pre-scaler to 1024
	TIMSK0 |= (1 << OCIE0A);													// overflow interrupt
	OCR0A = 0x9F;																// set top value for timer 0
}

void timer2_init() {
	TCCR2A |= (1 << WGM21);														// set timer 2 to CTC mode
	TIMSK2 |= (1 << OCIE2A) | (1 << OCIE2B);									// enable interrupts for A and B
	TCCR2B |= (1 << CS21) | (1 << CS22);										// set pre-scaler to 256
	OCR2A = 0x4D;																// set top value for timer 2
}

ISR(TIMER2_COMPA_vect) {
	LEDON;
}

ISR(TIMER2_COMPB_vect) {
	LEDOFF;
}

ISR(TIMER0_COMPA_vect) {
	scan_keypad();
}


int main(void)
{
	init();
	timer0_init();
	timer2_init();
	sei();
	
	while (1);
}

