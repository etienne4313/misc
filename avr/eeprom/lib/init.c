#include <stdio.h>
#include <avr/io.h>
#include <thin_lib.h>

extern void uart_init(void);

void lib_init(void)
{
	uart_init();
	PRINTF("AVR init\n");
}
