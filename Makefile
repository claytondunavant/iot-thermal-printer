CC = g++
CFLAGS = -Wall

CLIENT_SRC_FILES = client.cpp
SERVER_SRC_FILES = server.cpp
CLIENT_SRC_DIR=src/client/
SERVER_SRC_DIR=src/server/
BIN_DIR=bin/

CLIENT_SRC=$(addprefix $(CLIENT_SRC_DIR), $(CLIENT_SRC_FILES))
SERVER_SRC=$(addprefix $(SERVER_SRC_DIR), $(SERVER_SRC_FILES))

all: server client

server: $(SERVER_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(SERVER_SRC)

client: $(CLIENT_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(CLIENT_SRC)
	
clean:
	rm -rf $(BIN_DIR)
