#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"

/**
 * RAM read & write
 */
unsigned char *cpu_ram_read(struct cpu *cpu, int address)
{
    if (address < MAX_RAM)
    {
        return &cpu->ram[address];
    }
    else
    {
        return NULL;
    }
}

void cpu_ram_write(struct cpu *cpu, int address, unsigned char value)
{
    if (address < MAX_RAM)
    {
        cpu->ram[address] = value;
    }
}

/**
 * Load the binary bytes from an .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *ls8_file)
{
    // Arbitrary large number. Setting shorter than a line's length in file
    // causes issues with zeroed indexes due to the strtoul conversion. Don't
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

        // Skip those pesky single-line comments
        if (line == endptr)
        {
            continue;
        }

        // Find out if address++ increments before or after evaluation here?
        // "A post-increment operator is used to increment the value of variable
        // after executing expression completely in which post increment is used."
        cpu_ram_write(cpu, address++, val);
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
        cpu->registers[regA] *= cpu->registers[regB];
        break;

        // TODO: implement more ALU ops
    }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu, int debug)
{
    // Instruction Register
    unsigned char IR;

    // Operand storage per ls8/README.md step 4
    unsigned int operandA;
    unsigned int operandB;

    int running = 1; // True until we get a HLT instruction
    while (running)
    {
        if (debug)
        {
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
            printf("Program counter: %u\n", cpu->PC);
        }

        // 1. Get the value of the current instruction (in address PC).
        IR = *cpu_ram_read(cpu, cpu->PC);

        // 2. Figure out how many operands this next instruction requires
        int operands = IR >> 6;
        if (debug)
        {
            printf("Instruction register: %i\n", IR);
            printf("Stack pointer: %u\n", cpu->registers[SP]);
            printf("Operand count: %i\n", operands);
        }

        // 3. Get the appropriate value(s) of the operands following this instruction
        switch (operands)
        {
        case 1: // 0b0001
            operandA = *cpu_ram_read(cpu, cpu->PC + 1);
            if (debug)
            {
                printf("Value of operandA: %u\n", operandA);
            }
            break;
        case 2: // 0b0010
            operandA = *cpu_ram_read(cpu, cpu->PC + 1);
            operandB = *cpu_ram_read(cpu, cpu->PC + 2);
            if (debug)
            {
                printf("Value of operandA: %u\n", operandA);
                printf("Value of operandB: %u\n", operandB);
            }
            break;
        case 3: // 0b0011
            // do something maybe? this is dependent on the last 2 digits, so 0, 1, 2, & 3 are all possibilities.
            break;
        default: // 0b0000 or unexpected
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
        case MUL:
            alu(cpu, MUL, operandA, operandB);
            break;
        case POP:
            if (cpu->registers[SP] <= IVT - 1)
            {
                int stack_p = cpu->registers[SP];
                cpu->registers[operandA] = *cpu_ram_read(cpu, stack_p);
                cpu->registers[SP]++;
            }
            else
            {
                fprintf(stderr, "Stack underflow!");
                exit(-1);
            }
            break;
        case PRN:
            if (debug)
            {
                printf("Result: ");
            }
            fprintf(stdout, "%i\n", cpu->registers[operandA]);
            break;
        case PUSH:
            // TODO: Check for a stack overflow
            cpu->registers[SP]--;
            int stack_p = cpu->registers[SP];
            cpu_ram_write(cpu, stack_p, cpu->registers[operandA]);
            break;
        default:
            fprintf(stderr, "ERROR: Invalid instruction %u!\n", IR);
            exit(-1);
        }

        // 6. Move the PC to the next instruction.
        cpu->PC += operands + 1;

        if (debug)
        {
            printf("\n");
        }
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    // TODO: Initialize the PC and other special registers

    memset(cpu->registers, 0, sizeof(char) * 7);
    // 0xF4 / 0b11110100 / 244, where the interrupt vector table begins
    cpu->registers[SP] = IVT;
    cpu->PC = 0;
    // cpu->FL = 0;
    memset(cpu->ram, 0, sizeof(char) * MAX_RAM);
}
