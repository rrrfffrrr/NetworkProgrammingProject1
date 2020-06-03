# @author Kang chan yeong(rrrfffrrr@hanyang.ac.kr)
# @Create 06/02/2020
# @Last_update 06/02/2020
CC		:= gcc
CFLAG	:= -g -Wall
SPORT	:= 32322
SOURCE	:= $(wildcard *.c)
OBJECT	:= $(SOURCE:.c=.o)

.PHONY:	all clean clog c

all: server.out
	./server.out $(SPORT)

server.out: $(OBJECT)
	$(CC) $(CFLAG) -o $@ $(OBJECT)

c: clean clog

clean:
	$(RM) *.out *.o

clog:
	$(RM) *.log

%.o: %.c
	$(CC) $(CFLAG) -c -o $@ $^