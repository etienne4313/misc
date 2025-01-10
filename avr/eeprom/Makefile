PWD := $(shell pwd)
PROG := $(shell basename `pwd`)

#
# ARCH / CPU type
#
ARCH = avr
#CPU = mega2560
CPU = mega328

#
# Project files
#
common_objects := $(patsubst %.c,%.o,$(wildcard $(PWD)/*.c))
INCLUDE = $(PWD)

#
# Library files
#
AVRLIB := $(PWD)/lib/
lib_objects := $(patsubst %.c,%.o,$(wildcard $(AVRLIB)/*.c))

#
# Cross Compiler
# 	/usr/bin/avr-gcc
# 	/usr/lib/avr
#
CROSS_COMPILE = avr-
CC = $(CROSS_COMPILE)gcc
OBJDUMP = $(CROSS_COMPILE)objdump
OBJCOPY = $(CROSS_COMPILE)objcopy

#
# AVRDUDE
#
AVR_DUDE=/usr/bin/avrdude
AVR_DUDECONF=/usr/share/arduino/hardware/tools/avrdude.conf

ifeq ($(CPU),mega2560)
CFLAGS = -Wall -Os -DF_CPU=16000000UL -mmcu=atmega2560 -DARDUINO=10608 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR -std=c99 -c -I $(INCLUDE) -I $(AVRLIB) -Dmega2560
LDFLAGS = -mmcu=atmega2560
endif

ifeq ($(CPU),mega328)
CFLAGS = -Wall -Os -DF_CPU=16000000UL -mmcu=atmega328p -std=c99 -c -I $(INCLUDE) -I $(AVRLIB)
LDFLAGS = -mmcu=atmega328p
endif

all_objects := $(lib_objects) $(common_objects)

all: $(all_objects)
	$(CC) -o $(PROG) $(all_objects) $(LDFLAGS)
#	$(OBJDUMP) -D $(PROG) >diss
#	$(OBJDUMP) -x $(PROG) |grep -A6 "Idx Name"
#	$(OBJDUMP) -x $(PROG) |grep " .bss" |grep -v "*" |sort -nk 5  |tail -20
#	$(OBJDUMP) -x $(PROG) |grep " .data" |grep -v "*" |sort -nk 5  |tail -20

clean:
	rm -f $(all_objects) $(PROG) $(PROG).hex diss

flash:
	$(OBJCOPY) -O ihex -R .eeprom $(PROG) $(PROG).hex

ifeq ($(CPU),mega2560)
	$(AVR_DUDE) -C $(AVR_DUDECONF) -patmega2560 -cwiring  -P /dev/ttyACM0 -b 115200 -D -U flash:w:$(PROG).hex
endif

ifeq ($(CPU),mega328)
	$(AVR_DUDE) -C $(AVR_DUDECONF) -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:$(PROG).hex
endif

