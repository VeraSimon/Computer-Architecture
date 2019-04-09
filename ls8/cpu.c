#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

// #define DATA_LEN 6

/**
 * --> Northbridge goes here <--
 */
unsigned char *cpu_ram_read(struct cpu *cpu, unsigned int address)
{
    if (address >= 0 && address < MAX_RAM)
    {
        return &cpu->ram[address];
    }
    else
    {
        return NULL;
    }
}

void cpu_ram_write(struct cpu *cpu, unsigned int address, unsigned char value)
{
    if (address >= 0 && address < MAX_RAM)
    {
        cpu->ram[address] = value;
    }
}

/**
 * Load the binary bytes from an .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *ls8_file)
{
    // char data[DATA_LEN] = {
    //     // From print8.ls8
    //     0b10000010, // LDI R0,8
    //     0b00000000,
    //     0b00001000,
    //     0b01000111, // PRN R0
    //     0b00000000,
    //     0b00000001 // HLT
    // };

    // int address = 0;

    // for (int i = 0; i < DATA_LEN; i++)
    // {
    //     // Find out if address++ increments before or after evaluation here?
    //     // "A post-increment operator is used to increment the value of variable
    //     // after executing expression completely in which post increment is used."
    //     cpu->ram[address++] = data[i];
    // }

    // Replace this with something less hard-coded

    // Arbitrary large number. Setting shorter than a line's length in file
    // causes issues with expty indexes due to the strtoul conversion. Don't
    // set this to an instruction's length + 1.
    int instr_len = MAX_RAM;

    FILE *fp;
    char line[instr_len];
    fp = fopen(ls8_file, "r");

    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(2);
    }

    int address = 0;

    while (fgets(line, instr_len, fp) != NULL)
    {
        char *endptr;
        unsigned char val = strtoul(line, &endptr, 2);
        printf("address: %i\n", address);
        cpu->ram[address++] = val;
    }

    fclose(fp);
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op)
    {
    case ALU_MUL:
        // TODO: ALU_MUL
        break;

        // TODO: implement more ALU ops
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    // Initialize PC
    cpu->PC = cpu_ram_read(cpu, 0);

    // `IR`, the _Instruction Register_
    unsigned char IR;

    unsigned int operandA;
    unsigned int operandB;

    int running = 1; // True until we get a HLT instruction
    while (running)
    {
        // ~~ Debugging/testing ~~//
        printf("Registers:\n[ ");
        for (int i = 0; i < MAX_REGISTERS; i++)
        {
            printf("%u ", cpu->registers[i]);
        }
        printf("]\n");
        printf("RAM:\n[ ");
        for (int i = 0; i < MAX_RAM; i++)
        {
            if (cpu->ram[i] != 0)
            {
                printf("%i:%u ", i, cpu->ram[i]);
            }
        }
        printf("]\n");
        printf("PC: %u\n\n", cpu->PC);
        // ~~ End debugging/testing ~~//

        // 1. Get the value of the current instruction (in address PC).
        // strtoul(const char *str, char **endptr, int base)
        // IR = *cpu->PC;
        char *endptr;
        IR = strtoul(cpu->PC, &endptr, 2);

        // 2. Figure out how many operands this next instruction requires
        int shifted;
        int operands = (IR >> 6) & 0b11;

        // 3. Get the appropriate value(s) of the operands following this instruction
        switch (operands)
        {
        case 1:
            operandA = *cpu_ram_read(cpu, IR + 1);
            break;
        case 2:
            operandA = *cpu_ram_read(cpu, IR + 1);
            operandB = *cpu_ram_read(cpu, IR + 2);
            break;
        case 3:
            // do something maybe? this is dependent on the last 2 digits, so 0, 1, 2, & 3 are all possibilities.
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
            fprintf(stdout, "%i", cpu->registers[operandA]);
            break;
        default:
            fprintf(stderr, "ERROR: Invalid instruction %u!\n", IR);
            exit(-1);
        }

        // 6. Move the PC to the next instruction.
        cpu->PC = &cpu->PC[operands + 1];
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers

    memset(cpu->registers, 0, sizeof(char) * 7);
    cpu->registers[7] = 0xF4;
    cpu->PC = 0;
    // cpu->FL = 0;
    memset(cpu->ram, 0, sizeof(char) * 256);
}
