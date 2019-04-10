#ifndef _CPU_H_
#define _CPU_H_

#define MAX_REGISTERS 8
#define MAX_RAM 256

// Holds all information about the CPU
struct cpu
{
    // PC
    unsigned char *PC;

    // registers (array)
    // * R5 is reserved as the interrupt mask (IM)
    // * R6 is reserved as the interrupt status (IS)
    // * R7 is reserved as the stack pointer (SP)
    unsigned char registers[MAX_REGISTERS];

    // ram (array)
    unsigned char ram[MAX_RAM];

    // TODO: something from the spec in the CPU
    // FL;
};

// ### Instructions ### //

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

//# Baseline #//
#define HLT 0b00000001
#define LDI 0b10000010
#define PRN 0b01000111

//# ALU #//
#define MUL 0b10100010

// ALU operations
enum alu_op
{
    ALU_MUL = MUL
};

// ### Function declarations ### //

extern void cpu_load(struct cpu *cpu, char *ls8_file);
extern void cpu_init(struct cpu *cpu);
extern void cpu_run(struct cpu *cpu, int debug);

#endif
