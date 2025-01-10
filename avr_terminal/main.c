/*
 * avrdude - A Downloader/Uploader for AVR device programmers
 * Copyright (C) 2000-2005  Brian S. Dean <bsd@bsdhome.com>
 * Copyright 2007-2014 Joerg Wunsch <j@uriah.heep.sax.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* $Id: main.c 1347 2014-11-23 21:49:56Z joerg_wunsch $ */

/*
 * Code to program an Atmel AVR device through one of the supported
 * programmers.
 *
 * For parallel port connected programmers, the pin definitions can be
 * changed via a config file.  See the config file for instructions on
 * how to add a programmer definition.
 *  
 */

#include "ac_cfg.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <pthread.h>
#include <termios.h>

#include "avrdude.h"
#include "libavrdude.h"

#include "term.h"


/* Get VERSION from ac_cfg.h */
char * version      = VERSION;

char * progname = "tt";
char   progbuf[PATH_MAX]; /* temporary buffer of spaces the same
                             length as progname; used for lining up
                             multiline messages */

int avrdude_message(const int msglvl, const char *format, ...)
{
    int rc = 0;
    va_list ap;
    if (verbose >= msglvl) {
        va_start(ap, format);
        rc = vfprintf(stderr, format, ap);
        va_end(ap);
    }
    return rc;
}

int    verbose = 0;     /* verbose output */

#if 0
#define serial_open (serdev->open)
#define serial_setspeed (serdev->setspeed)
#define serial_close (serdev->close)
#define serial_send (serdev->send)
#define serial_recv (serdev->recv)
#define serial_drain (serdev->drain)
#define serial_set_dtr_rts (serdev->set_dtr_rts)
#endif
union filedescriptor fdp;

static void *action_thread (void *ctx)
{
	int c; 
	static struct termios oldt, newt;
	unsigned char buf;

	/*tcgetattr gets the parameters of the current terminal
	STDIN_FILENO will tell tcgetattr that it should write the settings
	of stdin to oldt*/
	tcgetattr( STDIN_FILENO, &oldt);
	/*now the settings will be copied*/
	newt = oldt;

	/*ICANON normally takes care that one line at a time will be processed
	that means it will return if it sees a "\n" or an EOF or an EOL*/
	newt.c_lflag &= ~(ICANON);

	/*Those new settings will be set to STDIN
	TCSANOW tells tcsetattr to change attributes immediately. */
	tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	/*This is your part:
	I choose 'e' to end input. Notice that EOF is also turned off
	in the non-canonical mode*/
	while((c=getchar())!= 'e'){
//		fprintf(stderr, "_%x_ ",c);
        if (c != '\n' && c != '\r' && c != ' ') {
			buf = c;
			serial_send(&fdp, &buf, 1);
		}
		usleep(100);
	}

	/*restore the old settings*/
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

	exit(0);
	return 0;
}

/*
 * main routine
 */
int main(int argc, char * argv [])
{
	int err;
	unsigned char buf;
	union pinfo pinfo = { .baud = 115200 };
	char *port;
    pthread_t thread;

	if(argc > 1)
		port = "/dev/ttyUSB0"; // NANO
	else
		port="/dev/ttyACM0"; 	// UNO

	err = pthread_create(&thread, NULL, action_thread, NULL);
    assert(err == 0);

	serial_open(port, pinfo, &fdp);
	serial_drain(&fdp, 1);
//	serial_set_dtr_rts(&fdp, 1);
	sleep(1);

	
	while(1){
		err = serial_recv(&fdp, &buf, 1);
		if(err < 0 )
			exit(-1);
		fprintf(stderr,"%c", buf);
	}

    avrdude_message(MSG_INFO, "%s: error reading system wide configuration file \n",
                    progname);
}
