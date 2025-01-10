#include <stdio.h>
#include <avr/io.h>
#include <thin_lib.h>
#include <util/delay.h>

void watchdog_enable(uint8_t x)
{
	MCUSR = 0;
	WDTCSR = _BV(WDCE) | _BV(WDE);
	WDTCSR = x;
}
