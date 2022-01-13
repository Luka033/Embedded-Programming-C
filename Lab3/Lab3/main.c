/*
 * Lab3.c
 *
 * Created: 9/10/2021 8:52:02 AM
 * Author : luka0
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define BAUD 9600										// define baud
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)          // set baud rate value for UBRR
#define ROWS 4
#define COLS 4
#define DELAY 250

const char buttons[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}};


// function to initialize UART
void uart_init(void) {
	UBRR0H = (BAUDRATE >> 8);							// shift the baud rate register right by 8 bits
	UBRR0L = BAUDRATE;									// set baud rate
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);				// enable receiver and transmitter
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);			// 8bit data format
}

// function to receive data
unsigned char uart_recieve(void) {
	while (!( UCSR0A & (1 << RXC0)));					// wait while data is being received
	return UDR0;										// return 8-bit data
}

// function to send data 
void uart_transmit(unsigned char data) { 
	while (!( UCSR0A & (1<<UDRE0)));					// wait while register is free 
	UDR0 = data;										// load data in the register 
}

void scan_keypad() {
	for(uint8_t i = 0; i < ROWS; i++) {
		PORTD &= ~(1 << (i + ROWS));					// set i:th row to low (offset by 4 since we start at pin 4)
		for(uint8_t j = 0; j < COLS; j++) {
			if(!(PINB & (1 << j))) {					// check which columns is pulled low
				_delay_ms(DELAY);						// de-bounce
				uart_transmit(buttons[i][j]);
			}
		}
		PORTD |= (1 << (i + ROWS));						// set the i:th row back to high (offset by 4 since we start at pin 4)
	}
}


int main(void)
{
    uart_init();
	DDRD |= (1 << DDD4) | (1 << DDD5) | (1 << DDD6) | (1 << DDD7);				// set rows as output
	DDRB &= ~(1 << DDB0) & ~(1 << DDB1) & ~(1 << DDB2) & ~(1 << DDB3);			// set columns as input
	PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);		// enable internal pull up resistors for columns
	PORTD |= (1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);								// set all rows output as high
		
    while (1) 
    {
		scan_keypad();
    }
}

