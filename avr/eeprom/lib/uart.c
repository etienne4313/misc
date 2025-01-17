#include <stdio.h>
#include <avr/io.h>

#define BAUD 115200
#define UBRR_SETTING ( ( F_CPU / 4 / BAUD - 1) / 2) /* u2x mode */
//#define UBRR_SETTING ( ( F_CPU / 8 / BAUD - 1) / 2) /* NO u2x mode */

void usart_init(void)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(UBRR_SETTING>>8);
	UBRR0L = (unsigned char)UBRR_SETTING;
	/* Double transmission speed */
	UCSR0A = 1 << U2X0;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

int USART_data_available(void)
{
	if( !(UCSR0A & (1<<RXC0)) )
		return 0;
	return 1;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

int USART_Flush( void )
{
	unsigned char dummy = 0;
	while ( UCSR0A & (1<<RXC0) ) dummy = UDR0;
	return dummy;
}

/* 
 * STREAM INTERFACE
 */
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, 
	NULL, _FDEV_SETUP_WRITE);

static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
		uart_putchar('\r', stream);
	USART_Transmit(c);
	return 0;
}

static int uart_getchar(FILE *stream);
static FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, 
	_FDEV_SETUP_READ);
static int uart_getchar(FILE *stream)
{
	return USART_Receive() & 0x7f;
}

void uart_init(void)
{
	usart_init();
	USART_Flush();
	stdout = &mystdout;
	stderr = &mystdout;
	stdin = &mystdin;
}
