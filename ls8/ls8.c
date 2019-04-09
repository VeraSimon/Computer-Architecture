#include <stdio.h>
#include "cpu.h"

/**
 * Main
 */
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Invalid program call. Correct usage: ./ls8 file_name.ls8\n");
        return 1;
    }

    struct cpu cpu;

    cpu_init(&cpu);
    cpu_load(&cpu, argv[1]);
    cpu_run(&cpu);

    return 0;
}