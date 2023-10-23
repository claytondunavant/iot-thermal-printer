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
PRINT\r\n
\r\n
I am printing this\r\n
\r\n
```

## TODO

- [ ] add support for bold
- [ ] add support for align left
- [ ] add support for align center
- [ ] add support for align right
- [ ] add support line feed n lines
- [ ] add support for tabs
- [ ] add support for printing bitmaps
