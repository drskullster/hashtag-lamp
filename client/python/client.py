__author__ = 'Greg Berger'

import socket

IP = "188.165.193.200"
PORT = 33333

BUFFER_SIZE = 20

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((IP,PORT))
print "Connected"
while True:
    data = s.recv(BUFFER_SIZE)
    print "Recieved data ", data

s.close()

