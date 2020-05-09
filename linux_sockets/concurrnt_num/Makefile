
OBJECTS = server.o 
INCLUDES = -I./include
CFLAGS = -g3
CC = gcc
EXECS = server
OBJS = $(SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

server: server.o include/assertf.h 
	$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $^

all: $(EXECS) 

clean: 
	rm -rf *.o server
