CC=gcc
CFLAGS=-Wall -g -I/usr/include/GL -I. -o3
LDFLAGS=-lSDL2 -lGL -lGLEW  -lm 
SOURCES=main.c utils.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm $(EXECUTABLE) $(OBJECTS)

