# iot-thermal-printer

## Protocol

Header and content are separated by empty lines.

### HEART

```
HEART\n
\n
n\n
uid\n
\n
```

Heartbeat packets are sent between the client and server every 10 seconds.
The first line of the body is always the number hearbeat it is.
Client sends the 0th heartbeat to the server without a uid because the server has yet to give to give it one.
Server creates saves the connection of this 0th heartbeat and provides the uid in the 1st heartbeat.
Clients send a heartbeat every 10 seconds.

### PRINT

```
PRINT\n
\n
I am printing this\n
\n
```

## Client vs. Server Architecture

### Server Architecture

The server has one thread running for each connection.
This thread blocks till something is read from a client.
Depending on what is read, something is written back to a/some client(s).

### Client Architecture

The client has one reading thread and one writing thread.
The reading thread blocks till something is read from the server.
What is read affects the local state (last received heartbeat number or print something).
The writing thread writes a heartbeat to the server every 10 seconds.

## TODO

- [x] configure server address with cli args on client/server
- [x] configure output file with cli args on client
- [x] add support for bold
- [x] add support for align left
- [x] add support for align center
- [x] add support for align right
- [ ] add support line feed n lines
- [ ] add support for tabs
- [ ] add support for printing bitmaps
- [ ] make reading/writing on a connection async rather than polling
- [x] implment trivial heartbeats
- [x] implment heartbeats between client and server every 10 seconds
- [x] implment registering a uid for each client by the server after 0th heartbeat
- [x] implment DS for keeping track of connections on server
- [x] broadcast PRINT message from another process to all connections
- [x] print my agenda from google calendar for the day
- [ ] send PRINT messages to only one client
