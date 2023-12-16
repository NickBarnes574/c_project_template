#!/usr/bin/python3

import socket


def main():
    host = '127.0.0.1'  # Server address (localhost in this case)
    port = 31337        # Port number your server is listening on

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((host, port))
            print("Connected to server.")

            # Send a message
            message = "Hello, Server!"
            s.sendall(message.encode())

            # Receive a response
            response = s.recv(1024)
            print(f"Received from server: {response.decode()}")

        except Exception as e:
            print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
