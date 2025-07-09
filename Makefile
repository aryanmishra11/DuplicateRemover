CC = g++
CFLAGS = -Iinclude -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto
SRC = src/main.cpp src/file_scanner.cpp src/hash_calculator.cpp src/duplicate_handler.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = duplicate_file_finder

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: $(TARGET)
	./$(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean test install