# @author Kang chan yeong(rrrfffrrr@hanyang.ac.kr)
# @Create 06/02/2020
# @Last_update 06/02/2020
CC		:= gcc
CFLAG	:= -g -Wall
SPORT	:= 32322
SOURCE	:= $(wildcard *.c)
OBJECT	:= $(SOURCE:.c=.o)

.PHONY:	all clean

all: server.out
	./server.out $(SPORT)

server.out: $(OBJECT)
	$(CC) $(CFLAG) -o $@ $(OBJECT)

clean:
	$(RM) *.out *.o

%.o: %.c
	$(CC) $(CFLAG) -c -o $@ $^