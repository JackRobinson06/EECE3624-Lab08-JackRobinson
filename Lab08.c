/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified:
 *   ModDate:
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D		// Enter
#define LF          0x0A		// New line
#define ESC         0x1B		// escape

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 9600 // standard values from 1200 to 115200
#define DATABITS    8 // 5-8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

const char msg[] = "\r\n\r\nRS232 Communications Server\r\nStatus: Active\r\nJack Robinson\r\nEECE3624\r\nFall 2026\r\n\r\n";
const char bye[] = "\r\n\r\nBye";

void print(const char *s)
{
	while (*s) uart_tx(*s++);
}

int main(void)
{
		char c;
		unsigned char count = 0;   // number of letters of "end" matched

		DDRC |= 1;                 // LED0 on PC0 (assumed; active-low)
		PORTC |= 1;                // LED off

    /* See the UARTLibrary.h for available calls.
	 *     	  // initialize the UART
	 *     	  uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	 *
	 *        // transmit a value (0x41) to the comm port
	 *        c = 0x41;
	 *        uart_tx(c);
	 *
	 *        // receive a byte from the comm port
	 *        c = uart_rx();
	 */

	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	print(msg);
	while(1)
	{
				c = uart_rx();
				PORTC &= ~1;           // LED on
				_delay_ms(75);
				PORTC |= 1;            // LED off

				if (c == ESC) {
					print(msg);
					count = 0;
					continue;
				}
				uart_tx(c);
				if (c == CR){
					 uart_tx(LF);
				}

				if (c == 'e'){
					 count = 1;
				}
				else if (c == 'n' && count == 1){
					count = 2;
				}
				else if (c == 'd' && count == 2) {
					print(bye);
					while (1) {}
				}
				else count = 0;
		
	}
	return 0;
}
