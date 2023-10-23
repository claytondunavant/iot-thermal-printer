# iot-thermal-printer

## Setting up

### On the client

Update the SERVER constant to server IP address.

Update the SERIAL constant to path to serial device.

### On the server

Update the SERVER constant to Server IP address.

## Protocol

Header and content are separated by empty lines.

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
