#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <thin_lib.h>

volatile int a =0;
ISR(TIMER0_OVF_vect)
{
	a++;
}

int main (void)
{
	int x =1;
	unsigned char i;

	init();
	timer0_setup();
	timer0_set_prescale(BY_1024);
	sei();

	while(1){
		printf("H: %x,%d,%d\n",x,a, TCCR0B);
		if(!(x%1024)){
			i = getchar();
			switch(i){
				case '1':
					timer0_set_prescale(BY_1);
					break;
				case '2':
					timer0_set_prescale(BY_8);
					break;
				case '3':
					timer0_set_prescale(BY_64);
					break;
				case '4':
					timer0_set_prescale(BY_256);
					break;
				case '5':
					timer0_set_prescale(BY_1024);
					break;
			}
		}
		x++;
	}
}


