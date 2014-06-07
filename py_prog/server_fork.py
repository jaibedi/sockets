# Echo server
import socket
import os

def handleClient(connection):
    # Read data
    while True:
        data = connection.recv(1024)
        if not data: break            
        print("from connected user: ", data.decode()) # convert from byte to string
        connection.send(b'Echo=>' + data)           

    connection.close()
    os._exit(0)

def Main():
    HOST = '' #specifies that the socket is reachable by any address the machine happens to have
    PORT = 5000

    # create an INET, TCP socket
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # bind the socket to a public host, and a well-known port
    serversocket.bind((HOST, PORT))
    
    # become a server socket
    serversocket.listen(2)
    print("Server Listening...")
    
    while True:
        # accept connections from outside
        (connsocket, address) = serversocket.accept()    

        print("Connection from: ", address)
        
        childPid = os.fork()
        # copy this process
        if childPid == 0:
            # if in child process: handle
            handleClient(connsocket)
        else:
        		# close connection in parent
            connsocket.close()
        
    serversocket.close()        
    
if __name__ == '__main__':
    Main()
