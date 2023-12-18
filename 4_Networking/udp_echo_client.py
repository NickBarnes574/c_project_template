#!/usr/bin/python3

import socket


def main():
    host = '127.0.0.1'  # Server address (localhost in this case)
    port = 31337        # Port number your server is listening on

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        try:
            # Send a message
            message = "Hello, Server!"
            s.sendto(message.encode(), (host, port))
            print("Message sent to server.")

            # Receive a response
            response, server_address = s.recvfrom(1024)
            print(f"Received from server: {response.decode()}")

        except Exception as e:
            print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
