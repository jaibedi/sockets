# Echo client
import socket

def Main():
    HOST = '127.0.0.1'
    PORT = 5000

    sock = socket.socket()
    sock.connect((HOST, PORT))

    while True:
        message = raw_input("-> ")
        # Send message to server
        sock.send(message.encode())
        
        # Receive echo back
        data = sock.recv(1024)
        print("Received from server: ", data.decode())

    sock.close()

if __name__ == '__main__':
    Main()
