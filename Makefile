CC = g++
CFLAGS = -Wall

SRC = server.cpp
OUT = server

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
