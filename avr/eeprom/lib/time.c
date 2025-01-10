#include <stdio.h>
#include <avr/io.h>
#include <thin_lib.h>
#include <util/delay.h>

#define CFG_OUTPUT() do { \
	DDRC |= _BV(DDC1);\
} while(0)

#define DO_ON() do {PORTC |= _BV(PC1) ; } while(0)
#define DO_OFF() do {PORTC &= ~_BV(PC1) ; } while(0) 

#if 0
/* 
 * Expecting a 50usec square wave
 * Here you need to adjust the -DF_CPU=16000000UL to the matching CPU freq
 */
void time_calibration(void)
{
	CFG_OUTPUT();
	while(1){
		DO_ON();
		_delay_us(25);
		DO_OFF();
		_delay_us(25);
	}
}
#else
/* 
 * Here this is a trick to find out the Freq of the CPU
 * We run a square wave at 800 CPU cycle duty cycle
 * and measure on the scope the time it takes.
 *
 * EX: The scope is giving me out a ~50usec square wave.
 *  => 50 usec / 800 cycle = 62.5nsec per cycle ==> 16Mhz
 *  Then we go to above and adjust the F_CPU to 16Mhz
 *  	and we run the _delay_us(16) to see if this is matching
 */
void time_calibration(void)
{
	unsigned int us;;

	CFG_OUTPUT();
	
	while(1){
		DO_ON(); // 1 cycle 
		us=100;
		__asm__ __volatile__ ( /* 4 CPU cycle per iteration => 400 cycles */
			"1: sbiw %0,1" "\n\t"
			"brne 1b" : "=w" (us) : "0" (us)
		);
		DO_OFF(); // 1 cycle
		us=100;
		__asm__ __volatile__ ( /* 4 CPU cycle per iteration => 400 cycles */
			"1: sbiw %0,1" "\n\t" // 2 cycles
			"brne 1b" : "=w" (us) : "0" (us) // 2 cycles
		);
	} // 1 cycle
}
#endif
