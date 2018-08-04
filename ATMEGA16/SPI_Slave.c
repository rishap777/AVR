/*
 * SPI_slave_tifr.c
 *
 * Created: 18-01-2018 00:44:49
 *  Author: Rishap Singh
 */ 

#define F_CPU 8000000UL			/* Define frequency here its 8MHz */
#include <avr/io.h>
#include <util/delay.h>
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define MOSI 5
#define SCK 7
#define SS 4
#define MISO 6


void SPI_Init()					/* SPI Initialize function */
{
	DDRB &= ~((1<<MOSI)|(1<<SCK)|(1<<SS));  /* Make MOSI, SCK, SS as
 						input pins */
	DDRB |= (1<<MISO);			/* Make MISO pin as 
						output pin */
	SPCR = (1<<SPE);			/* Enable SPI in slave mode */
}

void SPI_Write(unsigned char data)		/* SPI write data function */
{
	char flush_buffer;
	SPDR = data;			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF)));	/* Wait till transmission complete */
	flush_buffer = SPDR;		/* Flush received data */
	/* Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read */
}

unsigned char SPI_Receive()			/* SPI Receive data function */
{	
	SPDR=0x00;
	unsigned char k;
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	k=SPDR;			/* Return received data */
	return(k);
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

void UART_TxChar(unsigned char ch)
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

int main(void)
{
	unsigned char i;
	UART_init(9600);
	SPI_Init();
	i=SPI_Receive();
    while(1)
    {
		
		
	if(i=='a')
		{
			while (i=='a')
			{
				SPI_Write('r');
				i=SPI_Receive();
				UART_TxChar(i);
			}
        
		}
		else
		{
			while(i!='a')
			{
		 SPI_Write('j');
		 i=SPI_Receive();
		 UART_TxChar(i);
			}
		}
	
	
    }
}