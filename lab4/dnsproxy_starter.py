#!/usr/bin/env python
import argparse
import socket
from scapy.all import *
import sys
# This is going to Proxy in front of the Bind Server

parser = argparse.ArgumentParser()
parser.add_argument("--port", help="port to run your proxy on - careful to not run it on the same port as the BIND server", type=int)
parser.add_argument("--dns_port", help="port the BIND uses to listen to dns queries", type=int)
parser.add_argument("--spoof_response", action="store_true", help="flag to indicate whether you want to spoof the BIND Server's response (Part 3) or return it as is (Part 2). Set to True for Part 3 and False for Part 2", default=False)
args = parser.parse_args()

# Port to run the proxy on
port = args.port
# BIND's port
dns_port = args.dns_port
# Flag to indicate if the proxy should spoof responses
SPOOF = args.spoof_response

IFACE = "lo0"   # Or your default interface
DNS_SERVER_IP = "127.0.0.1"  # Your local IP

# BPF_FILTER = "udp port " + str(port) +" and ip dst "+DNS_SERVER_IP

# print ("filter",BPF_FILTER)

def client(data):
	# Create a UDP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

	server_address = (DNS_SERVER_IP, dns_port)
	message = 'This is the message.  It will be repeated.'

	try:

	    # Send data
	    print >>sys.stderr, 'sending "%s"' % data
	    sent = sock.sendto(data, server_address)

	    # Receive response
	    print >>sys.stderr, 'waiting to receive'
	    datarecv, server = sock.recvfrom(4096)
	    print >>sys.stderr, 'received "%s"' % datarecv

	finally:
	    print >>sys.stderr, 'closing socket'
	    sock.close()
	    return datarecv

def forward_dns(orig_pkt, address):
    print("Forwarding: ",orig_pkt)
    data_from_bind = client(orig_pkt)
    # response = sr1(
    #     IP(dst=DNS_SERVER_IP)/
    #         UDP(sport=address[1])/
    #         DNS(rd=1, qd=DNSQR(qname=data)),
    #     verbose=0,
    # )
    # resp_pkt = IP(dst=address[0], src=DNS_SERVER_IP)/UDP(dport=address[1])/DNS()
    # resp_pkt[DNS] = response[DNS]
    # send(resp_pkt, verbose=0)

    return data_from_bind

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the port
server_address = (DNS_SERVER_IP, port)
print >>sys.stderr, 'starting up on %s port %s' % server_address
sock.bind(server_address)
while True:
    print >>sys.stderr, '\nwaiting to receive message'
    data, address = sock.recvfrom(4096)
    print >>sys.stderr, 'received %s bytes from %s' % (len(data), address)
    new_data = forward_dns(data, address)
    sent = sock.sendto(new_data, address)
    print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)

