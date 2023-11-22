CC=g++
CFLAGS=-g -ggdb -Wall -Werror
SRC=$(wildcard ./*.cpp)
OBJ=$(SRC:%.c=%.o)
TARGET=test

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $< -lpthread


.PHONY:clean

clean:
	rm -rf *.o
	rm -rf *.bin
	rm -rf $(TARGET)
	

