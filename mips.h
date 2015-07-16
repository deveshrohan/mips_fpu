#ifndef MIPS_H_INCLUDED
#define MIPS_H_INCLUDED

#include "systemc.h"

#define DWORD           32      // Dataword size
#define SIGNEXTENDBIT   16      // Half of dataword size
#define AWORDREG        5       // Address word size of registerfile

#define RAMSIZE         128     // Size (in bytes) of RAM
#define ROMSIZE         128     // Size (in bytes) of ROM
#define REGSIZE         32      // Size (in words) of registerfile

#define SHIFTBIT        2       // Nr position word shifted in shifter

// ALU operations
#define ALU_AND         0
#define ALU_OR          1
#define ALU_ADD         2
#define ALU_SUB         6
#define ALU_SLT         7
#define ALU_SUI			5

// MIPS opcodes
#define OP_RTYPE        0        // R-type instruction
#define OP_BEQ          4        // Branch on equal instruction
#define OP_LW           35       // Load word instruction
#define OP_SW           43       // Store word instruction
#define OP_J		02	 // J-type instruction
#define OP_FLOAT	20	 // opcode for jump instruction
#define OP_SUI		63		//opcode for sui instruction

// MIPS function codes
#define FUNC_ADD        0x20
#define FUNC_SUB        0x22
#define FUNC_AND        0x24
#define FUNC_OR         0x25
#define FUNC_SLT        0x2A

// RAM/ROM signals
#define W_MEMREAD       0x1
#define W_MEMWRITE      0x1

// Floating Point Unit (FPU) signals
#define SIGNBIT			1
#define EXPONENT		8
#define FRACTION		23

#endif
