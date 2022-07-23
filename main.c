#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "cache.h"

int main(int argc, char **argv) {

    if (argc < 5) {
        printf("main trace_file cache_size block_size set_degree\n");
        exit(0);
    }

    FILE *fptr = fopen(argv[1], "r");
    if (!fptr) {
        printf("No file named %s\n", argv[1]);
        exit(0);
    }
    printf("%s, %s, %s", argv[2], argv[3], argv[4]);

    system_init(argv + 2);

    while (fscanf(fptr, "%x", &mem_address) != EOF) {
        cycle++;
        hit += cache_address_access(mem_address);
    }

    printf(", %lf%%\n",(((double)cycle - (double)hit) * 100.0 / (double)cycle));
    system_terminate();

    return 0;
}