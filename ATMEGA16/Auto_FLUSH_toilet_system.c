/*
 * autoflush.c
 *
 * Created: 16-03-2017 12:37:52
 *  Author: Rishap Singh
 */ 


#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

unsigned int c;

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
	DDRC=0XFF;
	lcd_cmd(0x0E);
}

void ultrasonic()
{
		unsigned int value=0,dis=0;
			lcd_cmd(0x01);
			TCCR1A=0x00;
			
			PORTC =0XFF;
			_delay_us(10);
			PORTC=0x00;
			while(PINA==0x00);
			
			TCCR1B=0x02;
			TCNT1=0x00;
			while(PINA!=0x00);
			value=TCNT1;
			TCCR1B=0x00;
			TCNT1=0x00;
			dis=(value)/58;
			//lcd_data(0X01);
			lcd_data1('d');
			lcd_data1('i');
			lcd_data1('s');
			lcd_data1('=');
			division3(dis);
			_delay_ms(500);
			
			
			c=dis;
			
}

int main(void)
{
	int flag=0;
	DDRD=0xff;
	DDRA=0X00;
	lcd_initialize();
	while(1)
	{
		flag=0;
	ultrasonic();
		while(c<=30)  // check if person is within the range
		{
			ultrasonic();
			flag=1;
		}
		if(flag==1)
		{
			PORTD=0x3f;  // flushing the water by pulling siphone up by motor upwards
				_delay_ms(1000);
			PORTD=0xfc;              //motor downward     (stoping       the flow of water)
			    _delay_ms(1000);	
		}
		else
		{
			PORTD=0x00;
		}
		c=0;
		
	}
	
	

}    