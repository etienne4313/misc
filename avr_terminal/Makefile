CFLAGS = -Wall -c
LDFLAGS = -lrt -lpthread
INSTALL = /usr/local/bin/

CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)
INSTALL += $(EXTRA_INSTALL)

PROG := $(shell basename `pwd`)

objects := $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $< 

all: $(objects)
	gcc -o $(PROG) $(objects) $(LDFLAGS)

install: $(PROG)
	sudo cp $(PROG) $(INSTALL)

clean:
	rm -f $(objects) $(PROG)

mrproper:
	sudo rm -f /usr/local/bin/$(PROG)

