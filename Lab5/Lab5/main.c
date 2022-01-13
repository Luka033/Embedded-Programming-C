/'[[/*
 * Lab5.c
 *
 * Created: 10/1/2021 2:33:40 PM
 * Author : luka0
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#define ROWS 4
#define COLS 4
#define DUTY_CYCLE 0.5

const uint8_t keyNotes[ROWS][COLS] = {
	{0, 1, 2, 3},
	{4, 5, 6, 7},
	{8, 9, 10, 11},
	{12, 13, 14, 15}
};

void init(void) {
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);				// set rows as output
	DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);			// set columns as input
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);		// enable internal pull up resistors for columns
	PORTD |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);								// set all rows output as high
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);						// enable timer 2 PWM output mode
	TCCR2B |= (1 << WGM22) | (1 << CS22) | (1 << CS21);							// enable timer 2 waveform generation with pre-scaler = 256
	DDRD |= (1 << DDD3);														// set D3 as output for speaker
}

uint8_t getStepSize(uint8_t x) {
	float period = 1 / (440 * pow(pow(2, (1 / 12.0)), x)) * pow(10, 6.0);
	period = (256 * period) / 4096;
	return (uint8_t)period - 1;
}

void playTone(uint8_t i, uint8_t j) {
	while(!(PINB & (1 << j))) {													// check which columns is pulled low
		uint8_t stepSize = getStepSize(keyNotes[i][j]);							// get the step size	
		OCR2A = stepSize;														// set frequency
		OCR2B = OCR2A * DUTY_CYCLE;												// 50% duty cycle
	}
	OCR2A = 0;																	// reset period
	OCR2B = 0;																	// reset duty cycle
}


void scan_keypad() {
	for(uint8_t i = 0; i < ROWS; i++) {
		PORTD &= ~(1 << (i + ROWS));											// set i:th row to low (offset by 4 since we start at pin 4)
		for(uint8_t j = 0; j < COLS; j++) {
			playTone(i, j);
		}
		PORTD |= (1 << (i + ROWS));												// set the i:th row back to high (offset by 4 since we start at pin 4)
	}
}


int main(void)
{
	init();
		
	while (1)
	{
		scan_keypad();
	}
}

