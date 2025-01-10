/*
  wiring_analog.c - analog input and output
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  Modified 28 September 2010 by Mark Sproul
*/

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <thin_lib.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define DEFAULT 1
uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	analog_reference = mode;
}

void analogEnable(void)
{
	ADCSRA = 0x4; // Need to set the division to that value
	sbi(ADCSRA, ADEN); // Enable ADC
	DIDR0 = 0x3f; // Disable the Digital input
}

void analogDisable(void)
{
	cbi(ADCSRA, ADEN);
}

int analogRead(uint8_t pin)
{
	ADMUX = (analog_reference << 6) | (pin & 0x07);

	// without a delay, we seem to read from the wrong channel
	//delay(1);
	
	// start the conversion
	sbi(ADCSRA, ADSC);

	// ADSC is cleared when the conversion finishes
	while (bit_is_set(ADCSRA, ADSC));

	// ADC macro takes care of reading ADC register.
	// avr-gcc implements the proper reading order: ADCL is read first.
	return ADC;
}
