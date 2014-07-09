# ECE254 lab4 Makefile

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g -lrt


OBJS1=producer.o common.o
OBJS2=consumer.o common.o

all: produce consume

produce: $(OBJS1) 
	$(LD) $(LDFLAGS) -o $@ $(OBJS1) 

producer.o: producer.c common.h 
	$(CC) $(CFLAGS) -c producer.c 

consume: $(OBJS2) 
	$(LD) $(LDFLAGS) -o $@ $(OBJS2) 

consumer.o: consumer.c common.h 
	$(CC) $(CFLAGS) -c consumer.c 

common.o: common.c common.h 
	$(CC) $(CFLAGS) -c common.c 


.PHONY: clean
clean:
	rm -f *.o *.out 
