#include "cpu.h"


int init_cpu(cpu_t* cpu)
{
    for(int32_t i = 0; i < 32; i++)
    {
        cpu->x[i] = i;
    }
    cpu->pc = 0;
    cpu->ir = 0;
    return 0;
}


int fetch_instruction(cpu_t* cpu, uint8_t* rom)
{
    #ifdef DEBUG
    printf("[PC: 0x%08X] ", cpu->pc);
    #endif
    
    // Get current pointed to instruction
    uint32_t inst = rom[cpu->pc]             |
                    rom[cpu->pc + 1]  << 8   |
                    rom[cpu->pc + 2]  << 16  |
                    rom[cpu->pc + 3]  << 24;
    
    // Put instruction in instruction register + increment program counter
    cpu->ir = inst;
    printf(" FETCHED [%08X] ", cpu->ir);
    cpu->pc += 4;
    return 0;
}

int decode_execute_instruction(cpu_t* cpu)
{
    uint8_t opcode = cpu->ir & 0b1111111;
    e_inst_type inst_type = get_instruction_type(opcode);
    printf("\nInstruction type is %d", inst_type);
    // switch instruction type
    switch(inst_type)
    {
        case R_TYPE:
            return r_type(cpu);

        case I_TYPE:
        case S_TYPE:
        case B_TYPE:
        case U_TYPE:
        case J_TYPE:
        case UNKNOWN_TYPE:
    }
    return 0;
}


e_inst_type get_instruction_type(uint8_t opcode)
{
    switch(opcode)
    {
        case 0b0110011:
            return R_TYPE;
        case 0b0010011:
        case 0b0000011:
        case 0b1100111:
        case 0b1110011:
            return I_TYPE;
        case 0b0100011:
            return S_TYPE;
        case 0b1100011:
            return B_TYPE;
        case 0b0110111:
        case 0b0010111:
            return U_TYPE;
        case 0b1101111:
            return J_TYPE;
        default:
            return UNKNOWN_TYPE;
        
    }
}





int r_type(cpu_t *cpu)
{
    #ifdef DEBUG
    printf("[instruction: 0x%08X] ", cpu->ir);
    #endif
    // Extract values from instruction
    uint8_t rd  =       (cpu->ir & 0x00000F80) >> 7;
    uint8_t funct3 =    (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    uint8_t rs2 =       (cpu->ir & 0x01F00000) >> 20;
    uint8_t funct7 =    (cpu->ir & 0xFE000000) >> 25;
    
    #ifdef DEBUG
    printf("| funct7 %d | rs2 %d | rs1 %d | funct3 %d | rd %d | opcode %d |\n", funct7, rs2, rs1, funct3, rd, (uint8_t) cpu->ir & 0xFF);
    #endif
    printf("** VALUES BEFORE **\n");
    printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[rs2 - x%d] %8X\n", 
        rd, cpu->x[rd], rs1, cpu->x[rs1], rs2, cpu->x[rs2]);
    switch(funct3)
    {
        case 0x0:
            switch(funct7)
            {
                case 0x00: // ADD
                   cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] + cpu->x[rs2];
                   break;
                case 0x20: // SUB
                   cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] - cpu->x[rs2];
                   break;
                case 0x01: // MUL (signed * signed)
                    cpu->x[rd] = rd == 0 ? 0 : (cpu->x[rs1] * cpu->x[rs2]) & 0xFFFFFFFF;
                    break;
            }
            break;
        case 0x1:
            switch(funct7)
            {
                case 0x00: // Shift Left Logical
                   cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] << cpu->x[rs2];
                   break;
                case 0x01: // MUL High (signed * signed)
                    int64_t mul = (int64_t)cpu->x[rs1] * (int64_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                    break;
            }
            break;
        case 0x2:
            switch(funct7)
            {
                case 0x00: // Set Less Than
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] < cpu->x[rs2] ? 1 : 0;      
                    }
                   break;
                case 0x01: // MUL High (signed * unsigned)
                    int64_t mul = (int64_t)cpu->x[rs1] * (int64_t)(uint32_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                    break;
            }
    }

    printf("** VALUES AFTER **\n");
    printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[rs2 - x%d] %8X\n", 
        rd, cpu->x[rd], rs1, cpu->x[rs1], rs2, cpu->x[rs2]);
    return 0;
}

int i_type(cpu_t *cpu)
{
    return 0;   
}

int s_type(cpu_t *cpu)
{
    return 0;
}

int b_type(cpu_t *cpu)
{
    return 0;
}

int u_type(cpu_t *cpu)
{
    return 0;
}

int j_type(cpu_t *cpu)
{
    return 0;
}

