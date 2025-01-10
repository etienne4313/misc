#include <avr/io.h>
#include <avr/interrupt.h>
#include <thin_lib.h>

/*
 * TIMER1:
 * The TCNT1, OCR1A/B, and ICR1 are 16-bit registers that can be accessed by the AVR CPU via the 8-bit data
 * bus. The 16-bit register must be byte accessed using two read or write operations. Each 16-bit timer has a
 * single 8-bit register for temporary storing of the high byte of the 16-bit access.
 *
 * To do a 16-bit write, the high byte must be written before the low byte. For a 16-bit read, the low byte must be
 * read before the high byte.
 *
 * The Timer/Counter incorporates an Input Capture unit that can capture external events and give them a time-
 * stamp indicating time of occurrence. The external signal indicating an event, or multiple events, can be applied
 * via the ICP1 pin or alternatively, via the analog-comparator unit. The time-stamps can then be used to calculate
 * frequency, duty-cycle, and other features of the signal applied. Alternatively the time-stamps can be used for
 * creating a log of the events.
 *
 * Measurement of an external signal’s duty cycle requires that the trigger edge is changed after each capture.
 * Changing the edge sensing must be done as early as possible after the ICR1 Register has been read. After a
 * change of the edge, the Input Capture Flag (ICF1) must be cleared by software (writing a logical one to the I/O
 * bit location). For measuring frequency only, the clearing of the ICF1 Flag is not required (if an interrupt handler
 * is used).
 * The simplest mode of operation is the Normal mode (WGM13:0 = 0). In this mode the counting direction is
 * always up (incrementing), and no counter clear is performed. The counter simply overruns when it passes its
 * maximum 16-bit value (MAX = 0xFFFF) and then restarts from the BOTTOM (0x0000). In normal operation the
 * Timer/Counter Overflow Flag (TOV1) will be set in the same timer clock cycle as the TCNT1 becomes zero. The
 * TOV1 Flag in this case behaves like a 17th bit, except that it is only set, not cleared. However, combined 
 * withthe timer overflow interrupt that automatically clears the TOV1 Flag, the timer resolution can be increased by
 * software. There are no special cases to consider in the Normal mode, a new counter value can be written anytime.
 * The Input Capture unit is easy to use in Normal mode. However, observe that the maximum interval between
 * the external events must not exceed the resolution of the counter. If the interval between events are too long,
 * the timer overflow interrupt or the prescaler must be used to extend the resolution for the capture unit.
 */
#if 0
#define TCCR1A _SFR_MEM8(0x80)
#define WGM10 0
#define WGM11 1
#define COM1B0 4
#define COM1B1 5
#define COM1A0 6
#define COM1A1 7

#define TCCR1B _SFR_MEM8(0x81)
#define CS10 0
#define CS11 1
#define CS12 2
#define WGM12 3
#define WGM13 4
#define ICES1 6
#define ICNC1 7

#define TCCR1C _SFR_MEM8(0x82)
#define FOC1B 6
#define FOC1A 7

#define TCNT1 _SFR_MEM16(0x84)

#define TCNT1L _SFR_MEM8(0x84)
#define TCNT1L0 0
#define TCNT1L1 1
#define TCNT1L2 2
#define TCNT1L3 3
#define TCNT1L4 4
#define TCNT1L5 5
#define TCNT1L6 6
#define TCNT1L7 7

#define TCNT1H _SFR_MEM8(0x85)
#define TCNT1H0 0
#define TCNT1H1 1
#define TCNT1H2 2
#define TCNT1H3 3
#define TCNT1H4 4
#define TCNT1H5 5
#define TCNT1H6 6
#define TCNT1H7 7

#define ICR1 _SFR_MEM16(0x86)

#define ICR1L _SFR_MEM8(0x86)
#define ICR1L0 0
#define ICR1L1 1
#define ICR1L2 2
#define ICR1L3 3
#define ICR1L4 4
#define ICR1L5 5
#define ICR1L6 6
#define ICR1L7 7

#define ICR1H _SFR_MEM8(0x87)
#define ICR1H0 0
#define ICR1H1 1
#define ICR1H2 2
#define ICR1H3 3
#define ICR1H4 4
#define ICR1H5 5
#define ICR1H6 6
#define ICR1H7 7

#define OCR1A _SFR_MEM16(0x88)

#define OCR1AL _SFR_MEM8(0x88)
#define OCR1AL0 0
#define OCR1AL1 1
#define OCR1AL2 2
#define OCR1AL3 3
#define OCR1AL4 4
#define OCR1AL5 5
#define OCR1AL6 6
#define OCR1AL7 7

#define OCR1AH _SFR_MEM8(0x89)
#define OCR1AH0 0
#define OCR1AH1 1
#define OCR1AH2 2
#define OCR1AH3 3
#define OCR1AH4 4
#define OCR1AH5 5
#define OCR1AH6 6
#define OCR1AH7 7

#define OCR1B _SFR_MEM16(0x8A)

#define OCR1BL _SFR_MEM8(0x8A)
#define OCR1BL0 0
#define OCR1BL1 1
#define OCR1BL2 2
#define OCR1BL3 3
#define OCR1BL4 4
#define OCR1BL5 5
#define OCR1BL6 6
#define OCR1BL7 7

#define OCR1BH _SFR_MEM8(0x8B)
#define OCR1BH0 0
#define OCR1BH1 1
#define OCR1BH2 2
#define OCR1BH3 3
#define OCR1BH4 4
#define OCR1BH5 5
#define OCR1BH6 6
#define OCR1BH7 7

/* IRQ */
#define TIMSK1 _SFR_MEM8(0x6F)
#define TOIE1 0
#define OCIE1A 1
#define OCIE1B 2
#define ICIE1 5
#endif

/*
 * EX at 20mhz 50nsec/cycle the 16bit timer1 will overflow in 3.277msec 
 * 	with the BY_1 PRESCALE
 */
void timer1_setup()
{
	/* Normal mode + Input capture */
	TCCR1A = 0<<COM1A0 | 0<<COM1A1 | 0<<COM1B0 | 0<<COM1B1 | 0<<WGM10 | 0<<WGM11;
	/* No prescaling, timer1 is stopped, No input capture */
	TCCR1B = 0<<WGM12 | 0<<WGM13 | 0<<CS10 | 0<<CS11 | 0<<CS12 | 0<<ICNC1 || 0<<ICES1;
	/* No force output compare */
	TCCR1C = 0<<FOC1B | 0<<FOC1A;

	OCR1AH = 0;
	OCR1AL = 0;

	OCR1BH = 0;
	OCR1BL = 0;

	/* 
	 * ISR(TIMER1_CAPT_vect) 
	 * ISR(TIMER1_OVF_vect)
	 * ISR(TIMER1_COMPA_vect)
	 * No input capture, no output compare, Overflow interrupt
	 */
	TIMSK1 =  0<<ICIE1 | 0<<OCIE1A | 0<<OCIE1B | 1<<TOIE1;
}

/*
 * Setting prescale will kick start timer 1
 */
void timer1_set_prescale(unsigned char prescale)
{
	unsigned char t;
	t = TCCR1B;
	t &= ~(1<<CS02   | 1<<CS01  | 1<<CS00);
	t = t | prescale;
	TCCR1B = t;
}

void timer1_disable(void)
{
	TCCR1B &= ~(1<<CS02   | 1<<CS01  | 1<<CS00);
}

