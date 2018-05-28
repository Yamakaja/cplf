//
// Created by Yamakaja on 27.05.18.
//

#ifndef PROJECT_UTIL_H
#define PROJECT_UTIL_H

#include <stdint.h>
#include <stdio.h>

#define CPLF_OTHER          0b0000000u
#define CPLF_ICMP           0b0000001u
#define CPLF_TCP            0b0000010u
#define CPLF_UDP            0b0000011u
#define CPLF_PROTO_MASK     0b0000011u

#define CPLF_ICMP_PING      0b0000100u
#define CPLF_ICMP_OTHER     0b0001000u
#define CPLF_ICMP_MASK      0b0001100u

#define CPLF_TCP_SYN        0b0000100u
#define CPLF_TCP_ACK        0b0001000u
#define CPLF_TCP_PSH        0b0010000u
#define CPLF_TCP_RST        0b0100000u
#define CPLF_TCP_FIN        0b1000000u
#define CPLF_TCP_MASK       0b1111100u

#define KEY_TCP_SYN         0
#define KEY_TCP_ACK         1
#define KEY_TCP_PSH         2
#define KEY_TCP_RST         3
#define KEY_TCP_FIN         4

#define CPLF_MODE_HELP      0b001u
#define CPLF_MODE_PRINT     0b010u
#define CPLF_MODE_REPAIR    0b100u
#define CPLF_MODE_MASK      0b110u

#define CPLF_TIMESERIES_TCP 0b01
#define CPLF_TIMESERIES_UDP 0b10

enum {
    CPLF_FMT_TEXT,
    CPLF_FMT_TIMESERIES
};

struct packet {
    uint8_t type;
    uint32_t time;
    uint32_t src_ip;
    uint16_t port;
};

extern const char *protocol_str[];

extern const char *protocol_icmp_type[];

extern const char *protocol_tcp_flag[];

extern int output_format;

extern int time_series_data;

extern uint32_t time_bound_lower;
extern uint32_t time_bound_upper;
extern uint32_t time_step;

int hashString(char *);

void print_file(char *file);

void print_help(char *const *argv);

void repair_file(char *file);

int parse_options(int argc, char *const argv[]);

int read_packet(FILE *stream, struct packet *pkt);

void print_timeseries(char *file);

void print_packet_ts(FILE *file, struct packet *pkt);

#endif //PROJECT_UTIL_H
