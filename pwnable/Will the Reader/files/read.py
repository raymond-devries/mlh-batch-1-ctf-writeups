import socket
import os
import sys

def read(book):
    command = "cat books/" + book + ".txt"
    print(command)
    return os.popen(command).read()


if __name__ == "__main__":
    host = "localhost"
    port = 12345

    if len(sys.argv) < 3:
        print(f"usage: python3 {sys.argv[0]} HOST PORT")
        exit(1)

    host = sys.argv[1]
    port = int(sys.argv[2])

    sock = socket.socket()
    sock.bind((host, port))
    sock.listen()
    while True:
        conn, _ = sock.accept()
        book = conn.recv(1024).decode().strip()
        conn.send(read(book).encode())
        conn.close()
