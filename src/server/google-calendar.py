import socket

server_address = ('localhost', 16662)
message = "PRINT\n\nFROM PYTHON\n\n"

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    try:
        s.connect(server_address)
        
        s.sendall(message.encode('utf-8'))
        
    except Exception as e:
        print(f"Error: {e}")