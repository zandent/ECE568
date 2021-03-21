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

DNS_SERVER_IP = "127.0.0.1"  # proxy server ip and BIND ip

def client(data):
	# Create a UDP socket
	sock_to_dns = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	server_address = (DNS_SERVER_IP, dns_port)
	try:
	    # Send data
	    print >>sys.stderr, 'sending "%s"' % data
	    sent = sock_to_dns.sendto(data, server_address)
	    # Receive response
	    print >>sys.stderr, 'waiting to receive'
	    datarecv, server = sock_to_dns.recvfrom(4096)
	    # print >>sys.stderr, 'received "%s"' % datarecv
	    print >>sys.stderr, 'received %s bytes' % len(datarecv)
	finally:
	    print >>sys.stderr, 'closing socket'
	    sock_to_dns.close()
	    return datarecv

def server():
	# Create a TCP/IP socket
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	# Bind the socket to the port
	server_address = (DNS_SERVER_IP, port)
	print >>sys.stderr, 'starting up on %s port %s' % server_address
	sock.bind(server_address)
	while True:
	    print >>sys.stderr, 'waiting to receive message'
	    data, address = sock.recvfrom(4096)
	    print >>sys.stderr, 'received %s bytes from %s' % (len(data), address)
	    data_back = client(data)
	    sent = sock.sendto(data_back, address)
	    print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)

server()

