#ifndef CPU_H
#define CPU_H

#ifndef DEBUG
#define DEBUG
#endif

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


typedef enum
{
    x0,
    x1,
    x2,
    x3,
    x4,
    x5,
    x6,
    x7,
    x8,
    x9,
    x10,
    x11,
    x12,
    x13,
    x14,
    x15,
    x16,
    x17,
    x18,
    x19,
    x20,
    x21,
    x22,
    x23,
    x24,
    x25,
    x26,
    x27,
    x28,
    x29,
    x30,
    x31

} e_register;


/**
 * @brief Defines the type of instructions, the type defines
 * the way the 32bit instruction is to be decoded
 * 
 */
typedef enum
{
    R_TYPE,
    I_TYPE,
    S_TYPE,
    B_TYPE,
    U_TYPE,
    J_TYPE,
    UNKNOWN_TYPE
} e_inst_type;

typedef struct
{
	uint32_t 	pc;
	int32_t 	x[31];		        // Registers
    uint32_t    ir;                 // Fetched instruction register
} cpu_t;

int init_cpu(cpu_t* cpu);


int fetch_instruction(cpu_t* cpu, uint8_t* rom);


/**
 * @brief Executes the instruction in the instruction register
 * 
 * @param cpu Pointer to the cpu
 * @return int 
 */
int decode_execute_instruction(cpu_t* cpu);

/**
 * @brief Get the instruction type depending on the 7bit opcode provided
 * 
 * @param opcode Opcode in the 32bit instruction 
 * @return e_inst_type The type of instruction to execute
 */
e_inst_type get_instruction_type(uint8_t opcode);



/**
 * @brief Executes a R-type instruction formatted
 * as : 
 * 
 * [ funct7 [31:25] | rs2 [24:20] | rs1 [19:15] | funct3 [14:12] |
 * rd [11:7] | opcode [6:0] ]
 * 
 * R-type contains basic arithmetic (ADD, SUB, MUL..) and logic (XOR, AND..) instructions 
 * 
 * @param cpu The CPU to execute on
 * @return int Status code
 */
int r_type(cpu_t *cpu);



/**
 * @brief Executes a I-type instruction formatted
 * as : 
 * 
 * [ imm [31:20] | rs1 [19:15] | funct3 [14:12] |
 * rd [11:7] | opcode [6:0] ]
 * 
 * I-type contains same instructions as R-type but rs2 is replaced
 * by immediate value (imm) ranging from -2048 to 2047 
 * 
 * @param cpu The CPU to execute on
 * @return int Status code
 */
int i_type(cpu_t *cpu);


int s_type(cpu_t *cpu);


int b_type(cpu_t *cpu);
int u_type(cpu_t *cpu);
int j_type(cpu_t *cpu);

#endif

