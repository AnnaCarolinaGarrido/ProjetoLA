
CC = gcc
CFLAGS = -g -O1 -Wall -Wextra -std=c99

TARGET = validador_lba

SRC = validador_lba.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) $(FILE) "$(MSG)"

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) $(FILE) "$(MSG)"

.PHONY: all clean run valgrind