#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <getopt.h>

#include "cplf.h"

int main(int argc, char **argv) {

    if (argc == 1) {
        fprintf(stderr, "%s: missing operand", argv[0]);
        fprintf(stderr, "%s: See '%s -h' for more information\n", argv[0], argv[0]);
        return 1;
    }

    int mode = parse_options(argc, argv);

    switch (mode) {
        case CPLF_MODE_HELP:
            print_help(argv);
            break;

        case CPLF_MODE_PRINT:
            if (argc == optind) {
                fprintf(stderr, "%s: No input files!\n", argv[0]);
                return 1;
            }

            if (output_format == CPLF_FMT_TEXT)
                for (int i = optind; i < argc; i++)
                    print_file(argv[i]);

            else if (output_format == CPLF_FMT_TIMESERIES)
                for (int i = optind; i < argc; i++)
                    print_timeseries(argv[i]);

            break;

        case CPLF_MODE_REPAIR:
            if (argc == optind) {
                fprintf(stderr, "%s: No input files!\n", argv[0]);
                return 1;
            }

            for (int i = optind; i < argc; i++)
                repair_file(argv[i]);

            break;
    }

    return 0;
}
