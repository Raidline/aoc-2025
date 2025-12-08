# Compiler
CC = cc

# Flags
CFLAGS = -Wall -Wextra -O0 -g

# Search all directories recursively for .c files
SRCS := $(shell find . -name '*.c')

# Convert each .c path into a .o path
OBJS := $(SRCS:.c=.o)

# Output executable name
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Rules for building object files (works for subdirs)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
