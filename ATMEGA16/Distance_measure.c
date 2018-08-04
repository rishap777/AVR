/*
 * GccApplication1.c
 *
 * Created: 18-02-2017 11:33:07
 *  Author: Rishap Singh
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
void lcd_data(unsigned char data)
{
	PORTC=0b00000001;
	PORTB=data+48;
	PORTC=0b00000011;
	_delay_us(800);
	PORTC&=0b00000001;
	_delay_us(800);
}
void lcd_data1(unsigned char data)
{
	PORTC=0b00000001;
	PORTB=data;
	PORTC=0b00000011;
	_delay_us(800);
	PORTC&=0b00000001;
	_delay_us(800);
}

void lcd_cmd(unsigned char cmd)
{
	PORTC=0b00000000;
	PORTB=cmd;
	PORTC=0b00000010;
	_delay_us(800);
	PORTC=0b00000000;
	_delay_us(800);
}

void division3(unsigned int x)
{
	int/* a,*/b,c,d,e,f;
	//
	// 	a=x/1000;
	// 	a=a%10;
	// 	lcd_data(a);
	b=x/100;
	c=b%10;
	lcd_data(c);
	d=x/10;
	e=d%10;
	lcd_data(e);
	
	f=x%10;
	lcd_data(f);
}

void lcd_initialize()
{
	DDRB=0xFF;
	DDRC=0b00000011;
	lcd_cmd(0x0E);
}


int main(void)
{
	unsigned int value,dis;
	DDRD=0x00;
	DDRA=0x11;
	lcd_initialize();
	
while(1)
	{
	lcd_cmd(0x01);	
   TCCR1A=0x00;
   
   PORTA=0xff;
   _delay_us(10);
   PORTA=0x00;
   while(PIND==0x00);
    
   TCCR1B=0x02;
  TCNT1=0x00;
   while(PIND!=0x00);
   value=TCNT1;
    TCCR1B=0x00;
    TCNT1=0x00;
   dis=(value)/58;
   lcd_data1('d');
   lcd_data1('i');
   lcd_data1('s');
   lcd_data1('=');
   division3(dis);
   _delay_ms(500);
  
   
	value=0;
	}
}    