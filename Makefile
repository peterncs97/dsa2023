CC = gcc
CFLAGS = -g -std=c99 -O2 -DDEBUG -Wall
TARGET = main
OBJFILES = $(TARGET).o listnode.o

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) -lm

clean:
