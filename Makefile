# Makefile for compiling cwire.c

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2
TARGET = cwire
SRC = cwire.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
