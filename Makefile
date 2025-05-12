CC = gcc
CCFLAGS = -Wall
SOURCES = $(wildcard src/*.c)
TARGET = bdd-package

all:
	$(CC) $(SOURCES) main.c -o $(TARGET) $(CCFLAGS)
