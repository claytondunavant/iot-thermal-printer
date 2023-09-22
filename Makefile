CC = g++
CFLAGS = -Wall

SRC_FILES = server.cpp
BIN_FILES = server
SRC_DIR=src/
BIN_DIR=bin/

SRC=$(addprefix $(SRC_DIR), $(SRC_FILES))
BIN=$(addprefix $(BIN_DIR), $(BIN_FILES))

all: $(BIN)

$(BIN): $(SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)
	
clean:
	rm -rf $(BIN_DIR)
