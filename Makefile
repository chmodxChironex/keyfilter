CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
TARGET = keyfilter
SRC = keyfilter.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
