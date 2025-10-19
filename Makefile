CC      := zig cc
CFLAGS  := -std=c11 -Wall -Wextra -O2
SRC     := src/main.c
TARGET  := headerGen

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

run:
	$(MAKE) all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
