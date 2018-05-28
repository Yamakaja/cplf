#include "cplf.h"

#include <errno.h>
#include <string.h>
#include <bits/types/time_t.h>
#include <netinet/in.h>
#include <time.h>
#include <stdlib.h>

void repair_file(char *filename) {
    char buffer[1 << 7];
    buffer[0] = '\0';

    strcat(buffer, filename);
    strcat(buffer, ".fixed");

    FILE *input = fopen(filename, "r");
    if (!input) {
        fprintf(stderr, "cplf: Failed to open input file '%s': %s\n", filename, strerror(errno));
        return;
    }

    FILE *output = fopen(buffer, "w");
    if (!output) {
        fprintf(stderr, "cplf: Failed to open output file '%s': %s\n", buffer, strerror(errno));
        fclose(input);
        return;
    }

    uint32_t last_time = 0;
    uint8_t zero_offset = 0;

    while (1) {
        uint8_t type;
        uint32_t time;
        struct in_addr src_ip;
        uint16_t port;

        fread(&type, sizeof(type), 1, input);
        if (feof(input))
            break;

        fread(&time, sizeof(time), 1, input);
        fread(&(src_ip.s_addr), sizeof(uint32_t), 1, input);

        if ((type & CPLF_PROTO_MASK) != CPLF_ICMP)
            fread(&port, sizeof(port), 1, input);

        int32_t dt = (int) ((long) time - (long) last_time);
        if (dt < 0)
            dt = -dt;

        if (!zero_offset && (time < last_time || (last_time != 0 && dt > 60 * 60))) {
            // Suspicious time jump
            time_t time_struct[3];
            char time_string[26 * 3];

            time_struct[0] = last_time;
            time_struct[1] = time;

            ctime_r(&time_struct[0], time_string);
            ctime_r(&time_struct[1], time_string + 26 * 1);

            time_string[0 * 26 + 24] = '\0'; // Remove newline
            time_string[1 * 26 + 24] = '\0'; // Remove newline

            printf("Suspicious time jump from [%s] to [%s]!\n"
                   "Please choose one of the following options to proceed:\n", time_string, time_string + 26);

            long start = ftell(input);

            for (int i = 0; i < 11; i++) {
                uint32_t epoch;
                fseek(input, i + 1, SEEK_CUR);
                fread(&epoch, sizeof(epoch), 1, input);

                time_struct[2] = epoch;
                ctime_r(&time_struct[2], time_string + 26 * 2);
                time_string[2 * 26 + 24] = '\0';

                printf("[%d] %s\n", i, time_string + 2 * 26);
                fseek(input, start, SEEK_SET);
            }

            int correct_offset;
            scanf("%d", &correct_offset);

            if (!correct_offset)
                zero_offset = 1;

            fseek(input, correct_offset, SEEK_CUR);
            continue;
        }

        zero_offset = 0;

        fwrite(&type, sizeof(type), 1, output);
        fwrite(&time, sizeof(time), 1, output);
        fwrite(&src_ip.s_addr, sizeof(uint32_t), 1, output);
        if ((type & CPLF_PROTO_MASK) != CPLF_ICMP)
            fwrite(&port, sizeof(port), 1, output);

        last_time = time;
    }

    fclose(input);
    fclose(output);

}
