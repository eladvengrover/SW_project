# Makefile for symmnf

# Compiler
CC = gcc

# Source files
SOURCES = symmnf.c
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = symmnf

.PHONY: all clean

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJECTS) $(TARGET)