#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <thin_lib.h>

volatile int a = 0;
volatile int b = 0;
volatile int c = 0;
ISR(TIMER0_OVF_vect)
{
	a++;
}

ISR(TIMER1_OVF_vect)
{
	b++;
}

ISR(TIMER1_CAPT_vect) 
{
	c++;
}
#if 0
#define SETUP timer1_setup
#define PRESCALE timer1_set_prescale
#define COUNT timer1_cnt
#else
#define SETUP timer0_setup
#define PRESCALE timer0_set_prescale
#define COUNT timer0_cnt
#endif

int main (void)
{
	int x =1;
	unsigned char i;

	init();
	SETUP();
	PRESCALE(BY_1024);
	sei();

	while(1){
		printf("%x %d %d %d\n",COUNT(),a,b,c);
		if(!(x%1024)){
			i = getchar();
			switch(i){
				case '1':
					PRESCALE(BY_1);
					break;
				case '2':
					PRESCALE(BY_8);
					break;
				case '3':
					PRESCALE(BY_64);
					break;
				case '4':
					PRESCALE(BY_256);
					break;
				case '5':
					PRESCALE(BY_1024);
					break;
			}
		}
		x++;
	}
}


