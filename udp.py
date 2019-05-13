import sys
import socket

def send_packet():
    #while(1):
    UDP_IP = sys.argv[1]
    UDP_PORT = 5005
    MESSAGE = sys.argv[2]
    sock = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
    sock.sendto(bytes(MESSAGE, "utf-8"), (UDP_IP, UDP_PORT))
    #print ("UDP target IP:", UDP_IP)
    #print ("UDP target port:", UDP_PORT)
    #print "message:", MESSAGE

send_packet()
