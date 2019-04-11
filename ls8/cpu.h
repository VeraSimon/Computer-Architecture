#ifndef _CPU_H_
#define _CPU_H_

// CPU macro constants
#define MAX_REGISTERS 8
#define MAX_RAM 256
#define SP 7
#define IVT 0xF4

// Holds all information about the CPU
struct cpu
{
    // program counter
    unsigned char *PC;

    // registers
    // * R0-R4 are free for general use
    // * R5 is reserved as the interrupt mask (IM)
    // * R6 is reserved as the interrupt status (IS)
    // * R7 is reserved as the stack pointer (SP)
    unsigned char registers[MAX_REGISTERS];

    // ram
    unsigned char ram[MAX_RAM];

    // TODO: something from the spec in the CPU
    // FL;
};

// ### Instructions ### //

// These use binary literals. If these aren't available with your compiler, hex
// literals should be used.

//# Other #//
#define HLT 0b00000001
#define LDI 0b10000010
#define POP 0b01000110
#define PRN 0b01000111
#define PUSH 0b01000101

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
