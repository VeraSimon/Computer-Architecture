#include <stdio.h>
#include <string.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Invalid program call. Correct usage: ./ls8 file_name.ls8\n");
        return 1;
    }

    struct cpu cpu;

    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    if (argc == 3 && (strcmp(argv[2], "1") == 0 || strcmp(argv[2], "true") == 0))
    {
        cpu_run(&cpu, 1);
    }
    else
    {
        cpu_run(&cpu, 0);
    }

    return 0;
}
