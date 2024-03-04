# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRCDIR = src
INCDIR = include
BUILDDIR = build

# Files
SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRCS))

# Target executable
TARGET = fuzzer

# Build rules
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Phony targets
.PHONY: clean

clean:
	rm -f $(BUILDDIR)/*.o $(TARGET) ./*.tar ./output/*.tar

run: $(TARGET)
	@echo "Running $(TARGET)"
	./$(TARGET) ./extractor_x86_64