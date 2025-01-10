#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <thin_lib.h>
#include "spi.h"

#if 0
/* 93l56 == 128 16bit data */
#define DATA_SIZE  128 
#define ADDRESS_SIZE 8
#define START_ADDR 112
#define END_ADDR 128
#define ADDR_MASK 0x80
#else
/* 93l76 == 512 16bit data */
#define DATA_SIZE  512 
#define ADDRESS_SIZE 10 
#define START_ADDR	208
#define END_ADDR	224
#define ADDR_MASK 0x200
#endif


/*   
 *   1 CS		8 VCC
 *   2 CLK		7 PE   PE to GND disable programming, PE not connected has pull up and enable Programming.
 *   3 DI		6 ORG  Has PULLUP for 16 bit wide // If GND connection then 8 bit wide
 *   4 DO		5 VSS  GND
 */

/*
 * PORTD 4 CLOCK
 * PORTD 5 CS
 * PORTD 6 DO   <===> DI
 * PORTD 7 DI   <===> DO 
 */
#define CFG_OUTPUT() do { \
	DDRD |= _BV(DDD4); \
	DDRD |= _BV(DDD5); \
	DDRD |= _BV(DDD6); \
} while(0)

#define SK_ON() do {PORTD |= _BV(DDD4) ; } while(0)
#define SK_OFF() do {PORTD &= ~_BV(DDD4) ; } while(0)
#define CS_ON() do {PORTD |= _BV(DDD5) ; } while(0)
#define CS_OFF() do {PORTD &= ~_BV(DDD5) ; } while(0)
#define DO_ON() do {PORTD |= _BV(DDD6) ; } while(0)
#define DO_OFF() do {PORTD &= ~_BV(DDD6) ; } while(0)

#define CLOCK() do {SK_ON(); _delay_ms(1); SK_OFF(); _delay_ms(1);} while(0)
#define CLOCKUP() do {SK_ON(); _delay_ms(1);} while(0)
#define CLOCKDOWN() do {SK_OFF(); _delay_ms(1);} while(0)

#define CFG_INPUT() do{ \
	DDRD &= ~(_BV(DDD7)); \
} while(0)
#define DI_VAL() (PIND & _BV(DDD7))  

/*
 * Read mode from address 0
 */
static void L56_auto_increment_mode(void)
{
	int x;
	uint16_t address = 0;

	CS_ON();
	_delay_ms(1);
	CLOCK();

	/* start bit */
	DO_ON();
	_delay_ms(1);
	CLOCK();

	/* read cmd 10 */
	DO_ON();
	_delay_ms(1);
	CLOCK();
	DO_OFF();
	_delay_ms(1);
	CLOCK();

	for(x=0;x<ADDRESS_SIZE;x++){
		if(address & ADDR_MASK)
			DO_ON();
		else
			DO_OFF();
		_delay_ms(1);
		CLOCK();
		address = address <<1;
	}
	DO_OFF();
}

static void L56_auto_increment_mode_off(void)
{
	_delay_ms(1);
	CS_OFF();
}

static uint16_t L56_auto_increment_read_byte(void)
{
	int x;
	uint16_t val =0;

	CLOCKUP();
	if(DI_VAL())
		val = val | 0x1;
	CLOCKDOWN();

	for(x=0;x<15;x++){ /* Always by 16 */
		val = val <<1;
		CLOCKUP();
		if(DI_VAL())
			val = val | 0x1;
		CLOCKDOWN();
	}
	_delay_ms(1);
	return val;
}
	
static uint16_t L56_data_read_byte(uint16_t address)
{
	int x;
	uint16_t val =0;

	CS_ON();
	_delay_ms(1);
	CLOCK();

	/* start bit */
	DO_ON();
	_delay_ms(1);
	CLOCK();

	/* read cmd 10 */
	DO_ON();
	_delay_ms(1);
	CLOCK();
	DO_OFF();
	_delay_ms(1);
	CLOCK();

	for(x=0;x<ADDRESS_SIZE;x++){
		if(address & ADDR_MASK)
			DO_ON();
		else
			DO_OFF();
		_delay_ms(1);
		CLOCK();
		address = address <<1;
	}
	DO_OFF();

	CLOCKUP();
	if(DI_VAL())
		val = val | 0x1;
	CLOCKDOWN();

	for(x=0;x<15;x++){ /* Always by 16 */ 
		val = val <<1;
		CLOCKUP();
		if(DI_VAL())
			val = val | 0x1;
		CLOCKDOWN();
	}

	_delay_ms(1);
	CS_OFF();
	return val;
}

static void L56_data_write_ctrl(int enable)
{
	int x;
	uint16_t address;

	if(enable){	
		PRINTF("WRITE ENABLE\n");
		address = 0xffff;
	}
	else{
		PRINTF("WRITE DISABLE\n");
		address = 0;
	}

	CS_ON();
	_delay_ms(1);
	CLOCK();

	/* start bit */
	DO_ON();
	_delay_ms(1);
	CLOCK();

	/* write enable cmd 00 */
	DO_OFF();
	_delay_ms(1);
	CLOCK();
	DO_OFF();
	_delay_ms(1);
	CLOCK();

	/* Address */
	for(x=0;x<ADDRESS_SIZE;x++){
		if(address & ADDR_MASK)
			DO_ON();
		else
			DO_OFF();
		_delay_ms(1);
		CLOCK();
		address = address <<1;
	}
	DO_OFF();
	CS_OFF();
	_delay_ms(1);
	CLOCK();
	_delay_ms(20);

}

static void L56_data_write_byte(uint16_t address, uint16_t val)
{
	int x;
	
//	PRINTF("%d:\t %2x %2x \n",address,(val>>8)&0xff, val&0xff);

	CS_ON();
	_delay_ms(1);
	CLOCK();

	/* start bit */
	DO_ON();
	_delay_ms(1);
	CLOCK();

	/* write cmd 01 */
	DO_OFF();
	_delay_ms(1);
	CLOCK();
	DO_ON();
	_delay_ms(1);
	CLOCK();

	/* Address */
	for(x=0;x<ADDRESS_SIZE;x++){
		if(address & ADDR_MASK)
			DO_ON();
		else
			DO_OFF();
		_delay_ms(1);
		CLOCK();
		address = address <<1;
	}
	DO_OFF();

	/* Data */
	for(x=0;x<16;x++){
		if(val & 0x8000)
			DO_ON();
		else
			DO_OFF();
		_delay_ms(1);
		CLOCK();
		val = val <<1;
	}
	DO_OFF();

	_delay_ms(1);
	CS_OFF();
	_delay_ms(1);
	CS_ON();
	/* Write to eeprom */
	_delay_ms(50);
	CS_OFF();
}

static int volatile tt;
static void warm_up(void)
{
	int x;

	/* Clear everyting */
	CFG_OUTPUT();
	DO_OFF();
	CS_OFF();
	SK_OFF();
	_delay_ms(100);

	/* Warm up */
	for(x=0;x<10;x++){
		tt = L56_data_read_byte(x);
	}
}

/* read value */
void eeprom_dump (void)
{
	int x;
	uint16_t val;

	warm_up();

	L56_auto_increment_mode();
	for(x=0;x<DATA_SIZE;x++){
		val = L56_auto_increment_read_byte();
		if(!(x%8))
			PRINTF("\n");
		PRINTF("0x%04x,\t ",val);
	}
	L56_auto_increment_mode_off();
	PRINTF("\n");
}

/* Full write */
void eeprom_write(void)
{
	uint16_t x;
	
	warm_up();

	L56_data_write_ctrl(1);
	for(x=0;x<DATA_SIZE;x++){
		if(!(x%8))
			PRINTF("\n");
		PRINTF("0x%04x,\t ",pgm_read_word_near(&(full_src[x])));
		L56_data_write_byte(x, pgm_read_word_near(&(full_src[x])));
	}
	L56_data_write_ctrl(0);
	PRINTF("\n");
}

/* Dump current file */
void eeprom_list(void)
{
	uint16_t x;
	for(x=0;x<DATA_SIZE;x++){
		if(!(x%8))
			PRINTF("\n");
		PRINTF("0x%04x,\t ",pgm_read_word_near(&(full_src[x])));
	}
	PRINTF("\n");
}

void io_init(void)
{
    CFG_OUTPUT();
    CFG_INPUT();
	SK_OFF();
	CS_OFF();
	DO_OFF();
}

/*
 *  0x4355,  0xbcaa,     0x4355,     0xbcaa,     0x4355,     0xbcaa,     0x4355,     0xbcaa,
 *  0x4358,  0xbca7,     0x4358,     0xbca7,     0x4358,     0xbca7,     0x4358,     0xbca7,
 *
 *  There is 8 entry with their respective checksum each
 */
int lookup[] ={15,93,223,143,159,207,79,31,191,239,111,63,47,127,255,175};
int checksum[] ={0xf,0xe,0xd,0xc,0xb,0xa,9,8,7,6,5,4,3,2,1,0};

void eeprom_extract(void)
{
	int x;
	uint32_t output, chk, val;

	warm_up();

	for(x=START_ADDR; x<END_ADDR; x=x+2){
		val = L56_data_read_byte(x);
		chk = L56_data_read_byte(x+1);
		PRINTF("Byte %lx: READ CHK %lx\n",val, chk);
		PRINTF("CHK = %x%x%x%x\n",checksum[((val&0xf000)>>12)], checksum[((val&0x0f00)>>8)],
			checksum[((val&0x00f0)>>4)], checksum[((val&0x000f))]);
		output = (((val&0xf000)>>12) * 65536) + (((val&0x0f00)>>8) * 4096) + (((val&0x00f0)>>4) * 256);
		output = output + lookup[val&0xf];
		PRINTF("Current Value = %ld\n",output);
	}
}

#define MIN(a,b) ( (a) <= (b) ? (a) : (b) )
static void att(uint32_t value, uint32_t *k, uint32_t *c)
{
	uint32_t tmp;
	unsigned short prog, prog1, val, chk;
	int x, min=1024, best=0, delta;

	tmp = (value/65536);
	prog = tmp;
	value = value - (tmp*65536);

	tmp = (value/4096);
	prog = (prog<<4) | tmp;
	value = value - (tmp*4096);

	tmp = (value/256);
	prog = (prog<<4) | tmp;
	value = value - (tmp*256);

//	fprintf(stderr,"PROG %x ", prog);
	prog1 = prog;

	tmp = value;
	prog = (prog<<4) | tmp;
//	fprintf(stderr,"[%ld] ", tmp); //LOOKUP table closet possible

	for(x=0; x<16; x++){
		if(tmp >= lookup[x])
			delta = tmp - lookup[x];
		else
			delta = lookup[x] - tmp;
		if(MIN(min, delta) < min){
			min = delta;
			best = x;
		}
	}
//	fprintf(stderr,"X = %d\n",best);

	val = prog1<<4 | (best+1);

	
	chk = checksum[((val&0xf000)>>12)];
	chk = chk <<4;
	chk |= checksum[((val&0x0f00)>>8)];
	chk = chk <<4;
	chk |= checksum[((val&0x00f0)>>4)];
	chk = chk <<4;
	chk |= checksum[((val&0x000f))];

	PRINTF("==> %x :: %x \n ",val, chk);

	*k = val;
	*c = chk;

//	PRINTF("CHK = %x%x%x%x\n",checksum[((val&0xf000)>>12)], checksum[((val&0x0f00)>>8)],
//		checksum[((val&0x00f0)>>4)], checksum[((val&0x000f))]);
}

void eeprom_update(uint32_t km)
{
	uint16_t x;
	uint32_t k, c;

	att(km, &k, &c);
	
	warm_up();

	L56_data_write_ctrl(1);
	for(x=START_ADDR; x<END_ADDR; x=x+2){
		PRINTF("0x%04lx,\t ",k);
		L56_data_write_byte(x, k);
		PRINTF("0x%04lx,\n ",c);
		L56_data_write_byte(x+1, c);
	}
	L56_data_write_ctrl(0);
	PRINTF("\n");

}


