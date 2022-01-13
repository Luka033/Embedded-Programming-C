/*
 * Lab2.c
 *
 * Created: 9/10/2021 8:52:02 AM
 * Author : luka0
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>


// define some macros

#define BAUD 9600										// define baud
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)          // set baud rate value for UBRR

// function to initialize UART
void uart_init(void) {
	UBRR0H = (BAUDRATE >> 8);							// shift the baud rate register right by 8 bits
	UBRR0L = BAUDRATE;									// set baud rate
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);				// enable receiver and transmitter
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);			// 8bit data format
}

// function to receive data
unsigned char uart_recieve(void) {
	while(!(UCSR0A) & (1<<RXC0));						// wait while data is being received
	return UDR0;										// return 8-bit data
}

// function to send data 
void uart_transmit(unsigned char data) { 
	while (!( UCSR0A & (1<<UDRE0)));					// wait while register is free 
	UDR0 = data;										// load data in the register 
}



int main(void)
{
    uart_init();
	char redID[] = "825209977\n\r"
    while (1) 
    {
		
		for(uint8_t i = 0; i < (uint8_t)strlen(redID); i++) {
			uart_transmit(redID[i]);		
		}
		_delay_ms(500);	
    }
}

