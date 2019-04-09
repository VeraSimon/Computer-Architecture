#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

#define DATA_LEN 6

/**
 * --> Northbridge goes here <--
 */
int cpu_ram_read(struct cpu *cpu, int address)
{
    if (address >= 0 && address < MAX_RAM)
    {
        return cpu->ram[address];
    }
    else
    {
        return NULL;
    }
}

void cpu_ram_write(struct cpu *cpu, int address, int value)
{
    if (address >= 0 && address < MAX_RAM)
    {
        cpu->ram[address] = value;
    }
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
        // TODO: Find out if address++ increments before or after evaluation here
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
    // `IR`, the _Instruction Register_
    unsigned int IR;

    unsigned int operandA;
    unsigned int operandB;

    int running = 1; // True until we get a HLT instruction
    while (running)
    {
        // Debugging/testing
        printf("Registers:\n[ ");
        for (int i = 0; i < MAX_REGISTERS; i++)
        {
            printf("%d ", cpu->registers[i]);
        }
        printf("]\n");

        // 1. Get the value of the current instruction (in address PC).
        IR = *cpu->PC;

        // 2. Figure out how many operands this next instruction requires
        int operands = IR & 0b000011;

        // 3. Get the appropriate value(s) of the operands following this instruction
        switch (operands)
        {
        case 1:
            operandA = cpu_ram_read(cpu, IR + 1);
            break;
        case 2:
            operandA = cpu_ram_read(cpu, IR + 1);
            operandB = cpu_ram_read(cpu, IR + 2);
            break;
        case 3:
            // do something with PRN maybe?
            break;
        default:
            // zero or unexpected value
            break;
        }

        // 4. switch() over it to decide on a course of action.
        // 5. Do whatever the instruction should do according to the spec.
        switch (IR)
        {
        case HLT:
            running = 0;
            break;
        case LDI:
            cpu->registers[operandA] = operandB;
            break;
        case PRN:
            break;
        default:
            fprintf(stderr, "ERROR: Invalid instruction %p!\n", IR);
            exit(-1);
        }

        // 6. Move the PC to the next instruction.
        cpu->PC = cpu->PC[operands + 1];
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers

    cpu->PC = cpu->ram[0];
    memset(cpu->registers, 0, sizeof(char) * 8);
    memset(cpu->ram, 0, sizeof(char) * 256);
}
