TARGET = scanner
CC = gcc
CFLAGS = -Iincludes -Wall -g
LDFLAGS = -lncurses

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
