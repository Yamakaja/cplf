#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#define CPLF_OTHER      0b0000000
#define CPLF_ICMP       0b0000001
#define CPLF_TCP        0b0000010
#define CPLF_UDP        0b0000011
#define CPLF_PROTO_MASK 0b0000011

#define CPLF_ICMP_PING  0b0000100
#define CPLF_ICMP_OTHER 0b0001000
#define CPLF_ICMP_MASK  0b0001100

#define CPLF_TCP_SYN    0b0000100
#define CPLF_TCP_ACK    0b0001000
#define CPLF_TCP_PSH    0b0010000
#define CPLF_TCP_RST    0b0100000
#define CPLF_TCP_FIN    0b1000000
#define CPLF_TCP_MASK   0b1111100

#define KEY_TCP_SYN     0
#define KEY_TCP_ACK     1
#define KEY_TCP_PSH     2
#define KEY_TCP_RST     3
#define KEY_TCP_FIN     4

const char* protocol_str[] = {
    "OTHER",
    "ICMP",
    "TCP",
    "UDP"
};

const char* protocol_icmp_type[] = {
    "",
    "PING",
    "OTHER"
};

const char* protocol_tcp_flag[] = {
    " SYN",
    " ACK",
    " PSH",
    " RST",
    " FIN"
};

void process_file(char *file_name) {
    FILE *file;
    
    if (!strcmp(file_name, "-"))
        file = stdin;
    else
        file = fopen(file_name, "r");

    if (!file) {
        printf("cplf: cannot open '%s': %s\n", file_name, strerror(errno));
        return;
    }

    while (1) {
        uint8_t type;
        uint32_t time;
        struct in_addr src_ip;
        uint16_t port;

        fread(&type, sizeof(type), 1, file);
        if (feof(file))
            break;

        fread(&time, sizeof(time), 1, file);
        fread(&(src_ip.s_addr), sizeof(uint32_t), 1, file);

        if (!(type & CPLF_ICMP))
            fread(&port, sizeof(port), 1, file);

        time_t ltime = time;

        char ip_str[16];
        inet_ntop(AF_INET, &src_ip, ip_str, sizeof(ip_str));

        char time_str[26];
        ctime_r(&ltime, time_str);
        time_str[24] = '\0'; // Remove newline

        uint8_t protocol = type & CPLF_PROTO_MASK;
        switch (protocol) {
            case CPLF_ICMP:
                printf("[%s] [%s] %s %s\n",
                        time_str,
                        ip_str,
                        protocol_str[protocol],
                        protocol_icmp_type[(type & CPLF_ICMP_MASK) >> 2]);
                break;
            case CPLF_TCP:
                printf("[%s] [%s] %s %d%s%s%s%s%s\n",
                        time_str,
                        ip_str,
                        protocol_str[protocol],
                        port,
                        type & CPLF_TCP_SYN ? protocol_tcp_flag[KEY_TCP_SYN] : "",
                        type & CPLF_TCP_PSH ? protocol_tcp_flag[KEY_TCP_PSH] : "",
                        type & CPLF_TCP_RST ? protocol_tcp_flag[KEY_TCP_RST] : "",
                        type & CPLF_TCP_FIN ? protocol_tcp_flag[KEY_TCP_FIN] : "",
                        type & CPLF_TCP_ACK ? protocol_tcp_flag[KEY_TCP_ACK] : ""
                      );
                break;
            case CPLF_UDP:
                printf("[%s] [%s] %s %d\n",
                        time_str,
                        ip_str,
                        protocol_str[protocol],
                        port);
                break;
        }
    }
}

int main(int argc, char** argv) {

    if (argc == 1) {
        puts("cplf: missing operand");
        printf("Try '%s --help' for more information\n", argv[0]);
        return 1;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
        printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);
        puts("Convert cplf files to a human-readable log format.");
        puts("A filename of - will be interpreted as stdin."); 
        puts("");
        puts("  -h, --help      Display this help and exit");
        puts("");
        puts("Examples:");
        printf("  %s /var/log/packets.cplf\n", argv[0]);
        puts("");
        puts("Author: Yamakaja (2017), License: MIT");

        return 0;
    }

    for (int i = 1; i < argc; i++)
        process_file(argv[i]);

    return 0;
}

