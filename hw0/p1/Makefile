CC = gcc
CFLAGS = -g -std=c99 -O2 -DDEBUG -Wall -fsanitize=address
TARGET = main
OBJFILES = $(TARGET).o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) -lm

clean:
