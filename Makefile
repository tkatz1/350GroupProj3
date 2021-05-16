CC = g++

CFLAGS  = -g -Wall -pthread

TARGET = main

all: clean $(TARGET)

$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cc

clean:
	$(RM) $(TARGET)
