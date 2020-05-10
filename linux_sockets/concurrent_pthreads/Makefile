
OBJECTS = server.o client.o
INCLUDES = -I./include
CFLAGS = -g3
CC = gcc
EXECS = server client
OBJS = $(SRCS:.c=.o)
LIBS = -lpthread

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

server: server.o include/assertf.h include/primes.h
	$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $^

client: client.o include/assertf.h include/primes.h
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS)

all: $(EXECS) 

clean: 
	rm -rf *.o server client core 
