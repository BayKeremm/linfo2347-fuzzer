# Makefile for the tar creation project

# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra

# Source files
SRCS = main.c fuzzer.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = project

# Phony targets
.PHONY: all clean

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean rule
clean:
	$(RM) $(TARGET) $(OBJS) archive.tar