//
// Created by Yamakaja on 28.05.18.
//

#include <errno.h>
#include <string.h>
#include "cplf.h"

uint32_t manage_file(char *input_file, FILE **output, uint32_t day, uint32_t epoch) {
    char buffer[128];
    buffer[0] = '\0';

    if (*output == NULL) {
        sprintf(buffer, "%s.%d", input_file, day);
        *output = fopen(buffer, "w");
    } else if (epoch >= (day + 1) * 60 * 60 * 24) {
        fclose(*output);
        day = epoch / (60 * 60 * 24);
        sprintf(buffer, "%s.%d", input_file, day);
        *output = fopen(buffer, "w");
    }

    return day;
}

void print_timeseries(char *input_file) {
    FILE *input = fopen(input_file, "r");
    if (!input) {
        fprintf(stderr, "cplf: Failed to open input file: %s\n", strerror(errno));
        return;
    }

    FILE *output = NULL;
    if (!time_step)
        output = stdout;

    uint32_t day = time_bound_lower / (60 * 60 * 24);

    while (1) {
        struct packet pkt;

        if (!read_packet(input, &pkt))
            break;

        if ((pkt.type & CPLF_PROTO_MASK) == CPLF_ICMP || pkt.time < day * 60 * 60 * 24)
            continue;

        if (time_step)
            day = manage_file(input_file, &output, day, pkt.time);

        print_packet_ts(output, &pkt);
    }

    if (output)
        fclose(output);
}

void print_packet_ts(FILE *file, struct packet *pkt) {
    switch (pkt->type & CPLF_PROTO_MASK) {
        case CPLF_TCP:
            if (time_series_data & CPLF_TIMESERIES_TCP)
                fprintf(file, "%u,%u\n", pkt->time, pkt->port);

            break;

        case CPLF_UDP:
            if (time_series_data & CPLF_TIMESERIES_UDP)
                fprintf(file, "%u,%u\n", pkt->time, pkt->port);

            break;

        default:
            break;
    }
}

