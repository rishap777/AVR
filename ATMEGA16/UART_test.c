/*
 * uart_test1.c
 *
 * Created: 6/1/2017 7:54:59 PM
 *  Author: Rishap singh
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
void lcd_data(unsigned char data)
{
	PORTC=0b00000001;
	PORTA=data+48;
	PORTC=0b00000011;
	_delay_ms(2);
	PORTC&=0b00000001;
	_delay_ms(2);
}
void lcd_data1(unsigned char data)
{
	PORTC=0b00000001;
	PORTA=data;
	PORTC=0b00000011;
	_delay_ms(2);
	PORTC&=0b00000001;
	_delay_ms(2);
}

void lcd_cmd(unsigned char cmd)
{
	PORTC=0b00000000;
	PORTA=cmd;
	PORTC=0b00000010;
	_delay_ms(1);
	PORTC=0b00000000;
	_delay_us(1);
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
	DDRA=0xFF;
	DDRC=0b00000011;
	lcd_cmd(0x0E);
}


void uart_initialise()
{
 DDRD=0b00000010;
UCSRA=0x00;
UCSRB=0x18;                    //tx and rx enable
UCSRC=0x80;                   // writting to ucsrc reg
UCSRC=0X86;
UCSRC=0x06;
UBRRH=0x00;
UBRRL=0x33;                       // baud rate setting   
}

void display(unsigned char data)
{
	lcd_data1(data);
	// PORTD &=~(1<<PIND7);
}
void uart_rx()
{
	unsigned char data;
	while(!(UCSRA & (1<<RXC)));
// 	if(UCSRA & (  (1<<DOR)))   //|(1<<FE)
// 	{
// 	   PORTD |=(1<<PIND7);
// 	   _delay_ms(200);
// 	   PORTD &=~(1<<PIND7);
// 	}
 PORTD |=(1<<PIND7);
	data=UDR;
	if(data!=0)
	lcd_data1(data);
	//display(data);
	UDR=0x00;
	
}

void uart_tx(unsigned char data)
{
	while(PIND & (1<<PIND6));
	UDR=data;
	while(!(UCSRA & (1<<UDRE)));
	UCSRA|=0x20;
	
	_delay_ms(200);
}





int main(void)
{
	unsigned char str1[]="hello bhagwan";
	
	uart_initialise();
	DDRB=0x00;
	//DDRD|=(1<<PIND7);
	//DDRD&=~(1<<PIND6);
	//lcd_data1('a');
	lcd_initialize();
    while(1)
    {
		lcd_data1('1');
	         // while(PINB==0x01)//(PINB<<1))
	   	   _delay_us(10);
	   		 for(int i=0;i<14;i++)
	   		   uart_tx(str1[i]);
	   		   
	   		  
	   		   
	   		  
	   	   
// 	    _delay_us(100);
// 	   while(PINB==0x80)//(PINB<<8))
// 	   {
// 		  
// 		   uart_rx();
// 	   }
    }
}