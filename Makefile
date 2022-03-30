CC=gcc
CFLAGS=-g -ggdb -Wall -Werror
SRC=$(wildcard ./*.c)
OBJ=$(SRC:%.c=%.o)
TARGET=out

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -o $@ $^ -lpthread

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<


.PHONY:clean

clean:
	rm -rf *.o
	rm -rf *.txt
	rm -rf *.bin
	rm -rf $(TARGET)
	

