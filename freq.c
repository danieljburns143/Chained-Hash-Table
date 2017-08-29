/* freq.c */

#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Globals */
char * PROGRAM_NAME = NULL;

/* Functions */
void usage(int status) {
    fprintf(stderr, "Usage: %s\n", PROGRAM_NAME);
    fprintf(stderr, "    -f FORMAT        Output format (KEY, KEY_VALUE, VALUE, VALUE_KEY)\n");
    fprintf(stderr, "    -l LOAD_FACTOR   Load factor for hash table\n");
    exit(status);
}

void freq_stream(FILE *stream, double load_factor, DumpMode mode) {
    Map* map = map_create(0, load_factor);
    char buffer[BUFSIZ];
    char* word;
    while (fgets(buffer, BUFSIZ, stream)) {
            word = strtok(buffer, " \t\n");
            while (word != NULL) {
                Entry* tempEntry = map_search(map, word);
                int i = 1;
                if (tempEntry != NULL) {
                    i = tempEntry->value.number + 1;
                }
                map_insert(map, word, (Value)(int64_t)i, NUMBER);
                word = strtok(NULL, " \t\n");
            }
    }
    map_dump(map, stdout, mode);
    map_delete(map);
}

/* Main Execution */
int main(int argc, char *argv[]) {
    int argind = 1;
    char* format = "VALUE_KEY";
    double load_factor = 0;
    DumpMode mode;
    /* Parse command line arguments */
    PROGRAM_NAME = argv[0];
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        char* arg = argv[argind++];
        switch (arg[1]) {
            case 'h':
                usage(0);
                break;
            case 'f':
                format = argv[argind++];
                break;
            case 'l':
                load_factor = atof(argv[argind++]);
                break;
            default:
                usage(1);
                break;
        }
    }
    /* Set the mode */
    if (strcmp(format, "KEY") == 0) {
        mode = KEY;
    }
    else if (strcmp(format, "KEY_VALUE") == 0) {
        mode = KEY_VALUE;
    }
    else if (strcmp(format, "VALUE") == 0) {
        mode = VALUE;
    }
    else {
        mode = VALUE_KEY;
    }
    /* Compute frequencies of data from stdin */
    freq_stream(stdin, load_factor, mode);
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
