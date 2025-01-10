#ifndef __THIN__LIB__H__
#define __THIN__LIB__H__
#include <avr/io.h>

#if 0
/* Direct UART interface */
void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );
int USART_Flush( void );
void usart_init(void);
#endif

extern int debug;
#define PRINTF(fmt, args...) do {\
	if(debug) \
		fprintf(stderr, fmt, ## args); \
} while(0)

/* TIMER 0, TIMER 1 prescale */
#define BY_1 (0<<CS02   | 0<<CS01  | 1<<CS00)
#define BY_8 (0<<CS02   | 1<<CS01  | 0<<CS00)
#define BY_64 (0<<CS02   | 1<<CS01  | 1<<CS00)
#define BY_256 (1<<CS02   | 0<<CS01  | 0<<CS00)
#define BY_1024 (1<<CS02   | 0<<CS01  | 1<<CS00)

void timer0_setup();
void timer0_set_prescale(unsigned char prescale);
void timer0_disable(void);
static inline unsigned char timer0_cnt(void)
{
	return TCNT0;
}

void timer1_setup();
void timer1_set_prescale(unsigned char prescale);
void timer1_disable(void);
static inline unsigned short timer1_cnt(void)
{
	unsigned short t;
	t = TCNT1;
	return t;
}
static inline unsigned short timer1_icr(void)
{
	unsigned short t;
	t = ICR1;
	return t;
}

static inline void set_ocr1a(unsigned short val)
{   
	OCR1A = val;
}

void timer2_setup();
void timer2_set_prescale(unsigned char prescale);
void timer2_disable(void);
static inline unsigned char timer2_cnt(void)
{
	return TCNT2;
}


/* Library initialization */
void lib_init(void);

/* Time */
void time_calibration(void);

/* Analog input */
void analogEnable(void);
void analogDisable(void);
int analogRead(uint8_t pin);

/* Watchdog */
#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_4S     (_BV(WDP3) | _BV(WDE))
#define WATCHDOG_8S     (_BV(WDP3) | _BV(WDP0) | _BV(WDE))
#define wdt_reset() __asm__ __volatile__ ("wdr")
void watchdog_enable(uint8_t x);

/* UART */
int USART_data_available(void);
int USART_Flush( void );

/* MATH */
unsigned int divu10(unsigned int n);
#endif
