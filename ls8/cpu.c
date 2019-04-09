#include "cpu.h"

#define DATA_LEN 6

/** --> Northbridge goes here <--
 * TODO: We'll make use of these helper functions later.
 */
// cpu_ram_read()
int *cpu_ram_read(struct cpu *cpu)
{
    // return mem value or NULL?
    return 0;
}

// cpu_ram_write()
int *cpu_ram_write(struct cpu *cpu)
{
    // return 0 (success), -1 (general failure), or further negative numbers (specific failures)?
    return 0;
}

/**
 * Load the binary bytes from an .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
    char data[DATA_LEN] = {
        // From print8.ls8
        0b10000010, // LDI R0,8
        0b00000000,
        0b00001000,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001 // HLT
    };

    int address = 0;

    for (int i = 0; i < DATA_LEN; i++)
    {
        cpu->ram[address++] = data[i];
    }

    // TODO: Replace this with something less hard-coded
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_MUL:
        // TODO
        break;

        // TODO: implement more ALU ops
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction

    while (running)
    {
        // TODO
        // 1. Get the value of the current instruction (in address PC).
        // 2. Figure out how many operands this next instruction requires
        // 3. Get the appropriate value(s) of the operands following this instruction
        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers

    // void *memset(void *ptr, int x, size_t n);
    // ptr ==> Starting address of memory to be filled
    // x   ==> Value to be filled
    // n   ==> Number of bytes to be filled starting from ptr to be filled

    cpu->PC = 0;
    memset(cpu->registers, 0, sizeof(char) * 8);
    memset(cpu->ram, 0, sizeof(char) * 256);
}
