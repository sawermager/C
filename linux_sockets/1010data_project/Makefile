
OBJECTS = server.o client.o
INCLUDES = -I./include
CFLAGS = -g3
CC = gcc
EXECS = server client test
OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

server: server.o include/assertf.h include/rot13.h
	$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $^

client: client.o include/assertf.h include/rot13.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

test: test.o include/assertf.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^


all: $(EXECS) 

clean: 
	rm -rf *.o server client core test
