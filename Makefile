CC = gcc
CFLAGS = -Wall -g -lraylib -lm
TARGET = dsdf

all: dsdf

dsdf: $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

.PHONY: all dsdf
