CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = labyrinth
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

debug: CFLAGS += -g -O0
debug: clean $(TARGET)

.PHONY: all clean run debug
