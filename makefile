# @author Kang chan yeong(rrrfffrrr@hanyang.ac.kr)
# @Create 06/02/2020
# @Last_update 06/02/2020
CC		:= gcc
CFLAG	:= -g -Wall
PORT	:= 8080
SOURCE	:= $(wildcard *.c)
OBJECT	:= $(SOURCE:.c=.o)

.PHONY:	all run clean clog c

all: server.out

run: server.out
	./server.out $(PORT)

server.out: $(OBJECT)
	$(CC) $(CFLAG) -o $@ $(OBJECT)

c: clean clog

clean:
	$(RM) *.out *.o

clog:
	$(RM) *.log

%.o: %.c
	$(CC) $(CFLAG) -c -o $@ $^