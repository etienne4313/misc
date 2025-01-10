# OS source tree
OS_SRC := ../os_src

# Application source tree
APP_SRC := ../app/t2

CFLAGS = -c -Wall -I $(OS_SRC) -I./
LDFLAGS = -lrt -lpthread

# Port build
objects := $(patsubst %.c,%.o,$(wildcard *.c ))
%.o: %.c %.h
	$(CC) $(CFLAGS) -o $@ $<

# Application build
objects_app := $(patsubst $(APP_SRC)/%.c,$(APP_SRC)/%.o,$(wildcard $(APP_SRC)/*.c ))
$(APP_SRC)/%.o: $(APP_SRC)/%.c $(APP_SRC)/%.h
	$(CC) $(CFLAGS) -o $@ $<

# OS build
objects_os := $(OS_SRC)/ucos-ii.o
$(objects_os): $(OS_SRC)/*.c $(OS_SRC)/*.h
	$(CC) $(CFLAGS) -o $@ $(OS_SRC)/ucos_ii.c

all: $(objects) $(objects_os) $(objects_app)
	$(CC) -o ucos-ii $(objects) $(objects_os) $(objects_app) $(LDFLAGS)

clean:
	rm -f ucos-ii $(objects) $(objects_os) $(objects_app)

