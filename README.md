# iot-thermal-printer

## Setting up

### On the client

Update the SERVER constant to server IP address.

Update the SERIAL constant to path to serial device.

### On the server

Update the SERVER constant to Server IP address.

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

Heartbeat packets are sent between the client and server on a time interval.
The first line of the body is always the number hearbeat it is.
Client sends the 0th heartbeat to the server without a uid because the server has yet to give to give it one.
Server creates saves the connection of this 0th heartbeat and provides the uid in the 1st heartbeat.

### PRINT

```
PRINT\n
\n
I am printing this\n
\n
```

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
- [ ] implment heartbeats between client and server every 10 seconds
- [x] implment registering a uid for each client by the server after 0th heartbeat
- [x] implment DS for keeping track of connections on server
- [ ] broadcast PRINT message from another process to all connections
