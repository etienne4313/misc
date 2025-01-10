#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <thin_lib.h>
#include "spi.h"

int debug=1;

int main(void)
{
	uint32_t km;
	unsigned char i;
	lib_init();
	io_init();

again:
	USART_Flush();
	fprintf(stderr, "Going into debug mode\n");
	i = getchar();
	switch(i){
	case 'd': /* Full ASCII dump */
		eeprom_dump();
		break;
	case 'w': /* Extract K + chksum */
		eeprom_extract();
		break;
	case 'l': /* DUMP internal file */
		eeprom_list();
		break;
	case 'k': /* Full eeprom write from internal files */
		eeprom_write();
		break;
	case 'b': /* Provide K */
		PRINTF("ENTER KM\n");
		fscanf(stdin, "%ld", &km); // PRESS 'b' after HACK / bug
		eeprom_update(km);//275903);
		break;
	default:
		fprintf(stderr, "Error\n");
		break;
	}
	goto again;

	return 0;
}
