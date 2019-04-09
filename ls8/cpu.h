#ifndef _CPU_H_
#define _CPU_H_

#define MAX_REGISTERS 8
#define MAX_RAM 256

// Holds all information about the CPU
struct cpu
{
    // PC
    unsigned int *PC;

    // registers (array)
    // * R5 is reserved as the interrupt mask (IM)
    // * R6 is reserved as the interrupt status (IS)
    // * R7 is reserved as the stack pointer (SP)
    unsigned char registers[MAX_REGISTERS];

    // ram (array)
    unsigned char ram[MAX_RAM];
};

// ALU operations
enum alu_op
{
    ALU_MUL
    // Add more here
};

// Instructions

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

#define LDI 0b10000010
#define HLT 0b00000001
#define PRN 0b01000111
// TODO: more instructions here. These can be used in cpu_run().

// Function declarations

extern void cpu_load(struct cpu *cpu);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu);

#endif
