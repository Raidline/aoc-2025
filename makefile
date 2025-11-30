# Compiler
CC = cc

# Flags
CFLAGS = -Wall -Wextra -O2

# Automatically gather all .c files
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

# Output executable name
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
