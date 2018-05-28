//
// Created by Yamakaja on 28.05.18.
//

#include "cplf.h"

int read_packet(FILE *stream, struct packet *pkt) {
    fread(&pkt->type, sizeof(pkt->type), 1, stream);
    fread(&pkt->time, sizeof(pkt->time), 1, stream);
    fread(&pkt->src_ip, sizeof(pkt->src_ip), 1, stream);

    if ((pkt->type & CPLF_PROTO_MASK) != CPLF_ICMP)
        fread(&pkt->port, sizeof(pkt->port), 1, stream);

    if (feof(stream))
        return 0;

    return 1;
}
