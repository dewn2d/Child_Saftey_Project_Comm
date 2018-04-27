import socket
import sys

sock = socket .socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('localhost', 8080 )
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

try:
    while(True):
        message = raw_input("Command? ")
        print >>sys.stderr, 'sending %s' % message
        sock.sendall(message)

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
    
