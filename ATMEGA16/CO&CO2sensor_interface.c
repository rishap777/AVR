/*
 * air_polluttion.c
 *
 * Created: 29-05-2018 00:11:58
 *  Author: Rishap Singh
 */ 
#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>
#include <math.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
/*_______________adc_read() FUNCTION DETAILS_________________________

USER NEED TO PASS THE CHANNEL NO OF THE ADC,
 WHERE YOUR SENSOR OR ANY ANALOG DEVICE IS CONNECTED
  TO READ THE DIGITAL VALUE OF THE EQUIVALENT ANALOG VALUE ON THE ADC CHANNEL PIN SELECTED.,.......
  
  =>>ALSO THE FUNCTION RETURN THE EQUVALENT DIGITAL VALUE OF 10 BIT IN unsigned int DATA TYPE
	 THIS DATA VALUE CAN BE STORED IN USER DEFINED VARIABLES OF unsigned int DATA TYPE 
  */

unsigned int adc_read(unsigned char channel_sel)
{
	unsigned int acd_10bit_value;
	
	ADMUX|=(1<<REFS0 | 0<<ADLAR);   //    SELECTING AVCC AS A REFERENSE FOR ADC CONVERSION i.e 5 volts  AND  LEFT ADJUST
	ADCSRA |=(1<<ADEN) | (1<<ADPS2) | (1<<ADPS1)| (0<<ADPS0);	 // 1:ADEN IS FOR ADC ENABLE  2:ADPS2 ADPS1 ADPS0 IS USED TO SELECT THE PRESCALER FOR ADC CONVERSION i.e. HOW MUCH FAST OR SLOW YOUR CONVERSION SHOULD TAKE PLACE
	
		ADMUX&=0XF0;     // clearing any previous channel selection
		
		ADCSRA&=(~(1<<ADIF));   // clearing interrupt flag of adc i.e. ADIF
		
		ADMUX|=channel_sel;     // CHANNEL NO PASSED IS ASSINGNED TO ADMUX REGISTER
		
		ADCSRA |= (1<<ADSC);              //start of conversion i.e TO TRIGGER THE ADC FOR STARTING THE CONVERSION
		while(!(ADCSRA & (1<<ADIF)));  // WAITING FOR THE ADC CONVERSION TO BE DONE ,BY POLLING THE ADIF FLAG BIT ,WHICH IS SET WHEN ADC CONVERSION IS DONE
		
	
		acd_10bit_value=ADC;
		
		
		return(acd_10bit_value);
}

/* _________________lcd_data_char__________________
this function is used to send characters numbers (i.e  0 to 9 ) without ascii to lcd by passing the numbers into the function

*/

void lcd_data_num(unsigned char data)
{
	PORTC=0b00000001;                    // RS=1 FOR LCD IN DATA MODE
	PORTB=data+48;                      //   SENDING NUMBERS 0 TO 9 AND +48 IS TO MAKE THE NUMBER AS ITS ASCII CODE eg. ascii of 5 is 53 hence when data=5 lcd will receive 53 as PORTD=data+48. AS lcd only understand ascii codes.
	PORTC=0b00000011; //				// en =1
	_delay_us(800);
	PORTC&=0b00000001;						//en=0
	_delay_us(800);
}


void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);/* Use 8-bit character sizes */
	UBRRL = BAUD_PRESCALE;		/* Load lower 8-bits of the baud rate value */
	UBRRH = (BAUD_PRESCALE >> 8);	/* Load upper 8-bits*/
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);/* Wait till data is received */
	return(UDR);			/* Return the byte*/
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	/* Wait for empty transmit buffer*/
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)		/* Send string till null */
	{
		UART_TxChar(str[j]);
		j++;
	}
}

/* _________________lcd_data_char__________________
this function is used to send characters (i.e 'a' to 'z' or 'A' to 'Z' or '0' to '9' etc.) to lcd by passing the characters into the function

*/
void lcd_data_char(unsigned char data)
{
	PORTC=0b00000001;  // rs =1
	PORTB=data;
	PORTC=0b00000011;   // en=1
	_delay_us(800);
	PORTC&=0b00000001;		//en=0
	_delay_us(800);
}
/* _________________lcd_cmd__________________
this function is used to send commands to lcd by passing the command into the function

*/
void lcd_cmd(unsigned char cmd)
{
	PORTC=0b00000000; // rs=0 for using lcd in command mode
	PORTB=cmd;			
	PORTC=0b00000010;
	_delay_us(800);
	PORTC=0b00000000;
	_delay_us(800);
}
/*________________num_4digit_conv_________________
	THIS FUNCTION IS TO CONVERT 4 DIGIT NUMBER INTO SINGLE DIGIT FOR DISPLAYING IT ON LCD 
	AS LCD ACCEPT ONLY ASCII CODES.
	
*/

void num_4digit_conv(double x)
{
	int a,b,c,d,e,f;
	//x=x*100;
	 	a=x/1000;
	 	a=a%10;
	 	lcd_data_num(a);
		 UART_TxChar(a+48);
		 
		
	b=x/100;
	c=b%10;
	lcd_data_num(c);
	UART_TxChar(c+48);
	d=x/10;
	e=d%10;
	lcd_data_num(e);
	UART_TxChar(e+48);
	
	 lcd_data_char('.');
	UART_TxChar(e+48);
	
	f=fmod(x,10);
	lcd_data_num(f);
	UART_TxChar(f+48);
}

void lcd_initialize()
{
	DDRB=0xFF;            // setting PORTD AS OUTPUT FOR DATA AND COMMAND
	
	DDRC|=((1<<PD0)|(1<<PD1));     // SETTING AS OUTPUT PIN ::: 0th  PIN OF PORTB AS RS OF LCD  AND  1st  PIN OF PORTB AS EN OF LCD
}

void lcd_string(unsigned char *c)
{
	while(*c)
	{
		lcd_data_char(*c);
		c++;
	}
}
int main(void)
{
	unsigned int co,co2;
	double sen_vol1,sen_vol2,ppm1,ppm2;
	DDRA=0x00;
	DDRD|=(1<<7)|(1<<6);
	//PORTD=0xff;
	lcd_initialize();
	UART_init(9600);
	lcd_cmd(0x0E);
	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_string("WELCOME");
	_delay_ms(5000);
	lcd_cmd(0x01);
	lcd_string("AIR POLLUTION");
	lcd_cmd(0XC0);
	lcd_string("	MONITORING");
	_delay_ms(5000);
	lcd_cmd(0x01);
	lcd_string("CALIBRATING...");
	_delay_ms(5000);
	lcd_cmd(0x01);
    while(1)
    {
       co=adc_read(0);  // CHANNEL SELECTED IS  ZERO HERE U CAN SELECT ANY CHANNEL OF ADC i.e 0 TO 7::::::: VARIABLE a STORES ADC RESULT IN unsigned int FORMAT
	   co2=adc_read(1);
	   sen_vol1=(co*5)/1023;
	   ppm1=(3.027*(exp(1.0698*sen_vol1)));
	   //UART_SendString("CO2=");
	   lcd_string("CO=");
	   num_4digit_conv(co+390);  
	   lcd_string("ppm");
	   
	   if((co+390)>600)
	   {
	   PORTD&=~(1<<7);
	   PORTD&=~(1<<6);
	   
	   }
	   else
	   {
		   PORTD|=(1<<7);
		   PORTD|=(1<<6);
	   }
	   
	   UART_TxChar('\t');
	   lcd_cmd(0xC0);
	   sen_vol2=(co2*5)/1023;
	   ppm2=(3.027*(exp(1.0698*sen_vol2)));
	  // UART_SendString("NO2=");
	   lcd_string("CO2=");
	   num_4digit_conv(co2+2050);  
	   lcd_string("ppm");
	   
	   UART_TxChar('\n');
	   _delay_ms(1000);
	   lcd_cmd(0x01);  // clearing the lcd
    }
}