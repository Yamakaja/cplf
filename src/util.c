//
// Created by Yamakaja on 27.05.18.
//

#include "cplf.h"

const char *protocol_str[] = {
        "OTHER",
        "ICMP",
        "TCP",
        "UDP"
};

const char *protocol_icmp_type[] = {
        "",
        "PING",
        "OTHER"
};

const char *protocol_tcp_flag[] = {
        " SYN",
        " ACK",
        " PSH",
        " RST",
        " FIN"
};

int hashString(char *str) {
    int h = 0;
    for (; *str; str++)
        h = 31 * h + *str;

    return h;
}
