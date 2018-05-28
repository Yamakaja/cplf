//
// Created by Yamakaja on 27.05.18.
//

#include "cplf.h"

#include <getopt.h>
#include <unistd.h>
#include <memory.h>
#include <stdlib.h>
#include <errno.h>

int output_format = CPLF_FMT_TEXT;
int time_series_data = 0;
uint32_t time_bound_lower = 0;
uint32_t time_bound_upper = 0xFFFFFFFF;
uint32_t time_step = 0;

void print_help(char *const *argv) {
    printf("Usage: %s [OPTION]... [FILE]...\n", argv[0]);
    puts("Convert cplf files to a human-readable log format. \n"
         "A filename of - will be interpreted as stdin. \n"
         " \n"
         "  -h         Display this help and exit \n"
         "  -t<OF>     Produce time-series data \n"
         "             OF may be one ore more of the following: \n"
         "               U: UDP port\n"
         "               T: TCP port\n"
         "  -r         Perform guided repair of cplf log files. \n"
         "             Fixed files are saved to <name>.fixed to \n"
         "             preserve the original \n"
         "  -l<epoch>  Time filter - lower bound (inclusive) \n"
         "  -u<epoch>  Time filter - upper bound (exclusive) \n"
         "  -s<step>   Save the time series output into multiple \n"
         "             files which are split to each contain \n"
         "             $step seconds. \n"
         " \n"
         "Examples:");
    printf(" $ %s -p /var/log/packets.cplf\n", argv[0]);
    puts("");
    puts("Author: Yamakaja (2017), License: MIT");
}

int parse_options(int argc, char *const argv[]) {
    int mode = CPLF_MODE_PRINT;

    for (int opt; (opt = getopt(argc, argv, "l:u:t:s:hr")) != -1;)
        switch (opt) {
            case '?':
                return 0;

            case 'h':
                mode = CPLF_MODE_HELP;
                break;

            case 'r':
                mode = CPLF_MODE_REPAIR;
                break;

            case 't': {
                size_t len = strlen(optarg);
                output_format = CPLF_FMT_TIMESERIES;

                for (uint32_t i = 0; i < len; i++)
                    switch (optarg[i]) {
                        case 'U':
                            time_series_data |= CPLF_TIMESERIES_UDP;
                            break;

                        case 'T':
                            time_series_data |= CPLF_TIMESERIES_TCP;
                            break;
                        default:
                            fprintf(stderr, "%s: Unknown time series data: %c\n", argv[0], optarg[i]);
                            return 0;
                    }
                break;
            }

            case 'l':
                time_bound_lower = (uint32_t) strtol(optarg, NULL, 10);
                if (errno) {
                    fprintf(stderr, "%s: Invalid number '%s': %s\n", argv[0], optarg, strerror(errno));
                    return 0;
                }
                break;

            case 'u':
                time_bound_upper = (uint32_t) strtol(optarg, NULL, 10);
                if (errno) {
                    fprintf(stderr, "%s: Invalid number '%s': %s\n", argv[0], optarg, strerror(errno));
                    return 0;
                }
                break;

            case 's':
                time_step = (uint32_t) strtol(optarg, NULL, 10);
                if (errno) {
                    fprintf(stderr, "%s: Invalid number '%s': %s\n", argv[0], optarg, strerror(errno));
                    return 0;
                }
                break;

        }

    return mode;
}
