/*
 * led_testing.c
 *
 * Created: 23-07-2017 18:35:50
 *  Author: Rishap Singh
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	 DDRD=0xff;
	 DDRA=0xff;
	 DDRB=0xff;
	 DDRC=0xff;
	 
	TCCR0=0b01101000;
	TCNT0=0;
	OCR0=255;            /////////  green
	
	TCCR1A=0b10100010;
	TCCR1B=0b00011000;
	TCNT1=0;
	
	OCR1A=1;  /////   red
	
	OCR1B=215;
	ICR1=256;     ////////////       red
	
	TCCR2=0b01101000;
	TCNT2=0x00;
	OCR2=255;               //////      blue
   
		  TCCR0|=0b01101001;
		TCCR1B |=0b00011100;
		   TCCR2=0b01101001;
	while(1)
    {
		_delay_ms(100);
		
		OCR1A=1; 
		_delay_ms(100);
		OCR1A=255;
		
		OCR0=1;
		_delay_ms(100);
		OCR0=255;
		
		OCR2=1;
		_delay_ms(100);
		OCR2=255;
		
	}
}