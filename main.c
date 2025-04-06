#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "mhowf.h"

static void usage(char *prg) {
    fprintf(stderr, "%s <memory> <hardness>\n", prg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) usage(argv[0]);

    uint64_t memory = atoll(argv[1]);
    uint64_t hardness = atoll(argv[2]);

    if (memory < 65 || hardness == 0) usage(argv[0]);

    uint8_t data[64];
    if (fread(&data, 64, 1, stdin) != 1) {
        fprintf(stderr, "cannot read 64 bytes from stdin\n");
        return 1;
    }

    uint8_t *buffer = malloc(memory);
    if (buffer == NULL) {
        fprintf(stderr, "cannot allocate buffer\n");
        return 1;
    }

    mhowf((uint8_t *) &data, (uint8_t *) &data, buffer, memory, hardness);

    if (fwrite(&data, 64, 1, stdout) != 1) {
        fprintf(stderr, "cannot write 64 bytes to stdout\n");
        return 1;
    }

    free(buffer);
    return 0;
}
