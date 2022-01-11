#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#define LED PB0

int main(void)
{
	DDRB=0x01;
	while(1)
	{
		PORTB = 0x01;
		_delay_us(10);
		PORTB = 0x00;
		_delay_us(10);
		
	}
}
