#include "cplf.h"

#include <errno.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>

void print_packet(struct packet *pkt);

void print_file(char *file_name) {
    FILE *file = fopen(file_name, "r");

    if (!file) {
        printf("cplf: cannot open '%s': %s\n", file_name, strerror(errno));
        return;
    }

    while (1) {
        struct packet pkt;

        if (!read_packet(file, &pkt))
            break;

        if (pkt.time < time_bound_lower)
            continue;

        if (pkt.time >= time_bound_upper)
            break;

        print_packet(&pkt);
    }

    fclose(file);
}

void print_packet(struct packet *pkt) {
    time_t ltime = pkt->time;
    struct in_addr src_ip;
    src_ip.s_addr = pkt->src_ip;

    char ip_str[16];
    inet_ntop(AF_INET, &src_ip, ip_str, sizeof(ip_str));

    char time_str[26];
    ctime_r(&ltime, time_str);
    time_str[24] = '\0'; // Remove newline

    uint8_t protocol = pkt->type & CPLF_PROTO_MASK;
    switch (protocol) {
        case CPLF_ICMP:
            printf("[%s] [%s] %s %s\n",
                   time_str,
                   ip_str,
                   protocol_str[protocol],
                   protocol_icmp_type[(pkt->type & CPLF_ICMP_MASK) >> 2]);
            break;
        case CPLF_TCP:
            printf("[%s] [%s] %s %d%s%s%s%s%s\n",
                   time_str,
                   ip_str,
                   protocol_str[protocol],
                   pkt->port,
                   pkt->type & CPLF_TCP_SYN ? protocol_tcp_flag[KEY_TCP_SYN] : "",
                   pkt->type & CPLF_TCP_PSH ? protocol_tcp_flag[KEY_TCP_PSH] : "",
                   pkt->type & CPLF_TCP_RST ? protocol_tcp_flag[KEY_TCP_RST] : "",
                   pkt->type & CPLF_TCP_FIN ? protocol_tcp_flag[KEY_TCP_FIN] : "",
                   pkt->type & CPLF_TCP_ACK ? protocol_tcp_flag[KEY_TCP_ACK] : ""
            );
            break;
        case CPLF_UDP:
            printf("[%s] [%s] %s %d\n",
                   time_str,
                   ip_str,
                   protocol_str[protocol],
                   pkt->port);
            break;
        default:
            break; // Ignore
    }
}
