CC = g++
CFLAGS = -Wall

SOCKET_HELPER=src/sockethelper/sockethelper.cpp

CLIENT_SRC_FILES = client.cpp thermalprinter.cpp
SERVER_SRC_FILES = server.cpp
CLIENT_SRC_DIR=src/client/
SERVER_SRC_DIR=src/server/
BIN_DIR=bin/

CLIENT_SRC=$(addprefix $(CLIENT_SRC_DIR), $(CLIENT_SRC_FILES))
SERVER_SRC=$(addprefix $(SERVER_SRC_DIR), $(SERVER_SRC_FILES))

all: server client

server: $(SERVER_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(SERVER_SRC) $(SOCKET_HELPER)

client: $(CLIENT_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(CLIENT_SRC) $(SOCKET_HELPER)
	
clean:
	rm -rf $(BIN_DIR)
