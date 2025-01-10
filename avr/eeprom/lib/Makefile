objects := $(patsubst %.c,%.o,$(wildcard *.c ))
cppobjects := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<
#	$(AR) rcs $(ARCHIVE) $@

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -o $@ $< 
#$(AR) rcs $(ARCHIVE) $@

all: $(objects) $(cppobjects)

clean:
	rm -f *.o $(objects) $(cppobjects) 
