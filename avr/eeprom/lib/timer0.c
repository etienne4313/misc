#include <avr/io.h>
#include <avr/interrupt.h>
#include <thin_lib.h>

/*
 * TIMER0:
 * EXTERNAL CLOCK source ?? to feed the timer??                                                                         
 * 		An external clock source applied to the T1/T0 pin can be used as Timer/Counter clock (clk T1 /clk T0 ). The T1/T0    
 * 		pin is sampled once every system clock cycle by the pin synchronization logic. The synchronized (sampled)            
 *		signal is then passed through the edge detector. 
 */
#if 0
#define TCCR0A _SFR_IO8(0x24)
#define WGM00 0
#define WGM01 1
#define COM0B0 4
#define COM0B1 5
#define COM0A0 6
#define COM0A1 7

#define TCCR0B _SFR_IO8(0x25)
#define CS00 0
#define CS01 1
#define CS02 2
#define WGM02 3
#define FOC0B 6
#define FOC0A 7

#define TCNT0 _SFR_IO8(0x26)
#define TCNT0_0 0
#define TCNT0_1 1
#define TCNT0_2 2
#define TCNT0_3 3
#define TCNT0_4 4
#define TCNT0_5 5
#define TCNT0_6 6
#define TCNT0_7 7

#define OCR0A _SFR_IO8(0x27)
#define OCR0A_0 0
#define OCR0A_1 1
#define OCR0A_2 2
#define OCR0A_3 3
#define OCR0A_4 4
#define OCR0A_5 5
#define OCR0A_6 6
#define OCR0A_7 7

#define OCR0B _SFR_IO8(0x28)
#define OCR0B_0 0
#define OCR0B_1 1
#define OCR0B_2 2
#define OCR0B_3 3
#define OCR0B_4 4
#define OCR0B_5 5
#define OCR0B_6 6
#define OCR0B_7 7

/* IRQ */
#define TIMSK0 _SFR_MEM8(0x6E)
#define TOIE0 0
#define OCIE0A 1
#define OCIE0B 2
#endif

void timer0_setup()
{
	/* 
	 * Normal mode:
	 * The simplest mode of operation is the Normal mode (WGM02:0 = 0). In this mode the counting direction is
	 * always up (incrementing), and no counter clear is performed. The counter simply overruns when it passes its
	 * maximum 8-bit value (TOP = 0xFF) and then restarts from the bottom (0x00). In normal operation the
	 * Timer/Counter Overflow Flag (TOV0) will be set in the same timer clock cycle as the TCNT0 becomes zero

	 * 
	 * Clear Timer on Compare Match (CTC) Mode:
	 * In Clear Timer on Compare or CTC mode (WGM02:0 = 2), the OCR0A Register is used to manipulate the
	 * counter resolution. In CTC mode the counter is cleared to zero when the counter value (TCNT0) matches the
	 * OCR0A.
	 *
	 * For generating a waveform output in CTC mode, the OC0A output can be set to toggle its logical level on each
	 * compare match by setting the Compare Output mode bits to toggle mode (COM0A1:0 = 1).
	 */
	TCCR0A = 0<<COM0A1 | 0<<COM0A0 | 0<<COM0B1 | 0<<COM0B0 | 0<<WGM01 | 0<<WGM00;
	/* No prescaling, timer0 is stopped */
	TCCR0B = 0<<FOC0A  | 0<<FOC0B  | 0<<WGM02  | 0<<CS02   | 0<<CS01  | 0<<CS00;
	OCR0A = 0;
	OCR0B = 0;

	/*
	 * Overflow interrupt enable
	 * ISR(TIMER0_OVF_vect)
	 */
	TIMSK0 =  0<<OCIE0B | 0<<OCIE0A | 1<<TOIE0;
}

/*
 * Setting prescale will kick start timer 0
 */
void timer0_set_prescale(unsigned char prescale)
{
	unsigned char t;
	t = TCCR0B;
	t &= ~(1<<CS02   | 1<<CS01  | 1<<CS00);
	t = t | prescale;
	TCCR0B = t;
}

void timer0_disable(void)
{
	TCCR0B &= ~(1<<CS02   | 1<<CS01  | 1<<CS00);
}
