CC = g++
CFLAGS = -Wall -pthread

PWD=$(shell pwd)

SOCKET_HELPER=src/sockethelper/sockethelper.cpp

CLIENT_SRC_FILES = client.cpp thermalprinter.cpp
SERVER_SRC_FILES = server.cpp
CLIENT_SRC_DIR=src/client/
SERVER_SRC_DIR=src/server/
BIN_DIR=bin/

CLIENT_SRC=$(addprefix $(CLIENT_SRC_DIR), $(CLIENT_SRC_FILES))
SERVER_SRC=$(addprefix $(SERVER_SRC_DIR), $(SERVER_SRC_FILES))

RPI_COMPILE_CONTAINER_NAME=raspberry-pi-compiler
RPI_COMPILE_CONTAINER_PATH=/docker/compile-arm

.PHONY: compile-arm clean

all: server client
	
server: $(SERVER_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(SERVER_SRC) $(SOCKET_HELPER)

client: $(CLIENT_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)$@ $(CLIENT_SRC) $(SOCKET_HELPER)
	
docker-build:
	docker build -t $(RPI_COMPILE_CONTAINER_NAME) $(PWD)$(RPI_COMPILE_CONTAINER_PATH)

all-arm:
	docker run --rm --mount type=bind,source='$(PWD)',target=/app $(RPI_COMPILE_CONTAINER_NAME) 
	
clean:
	rm -rf $(BIN_DIR)
