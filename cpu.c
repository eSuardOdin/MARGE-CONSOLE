#include "cpu.h"
#include <stdint.h>


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
        return 0;
    }
    return 0;
}


e_inst_type get_instruction_type(uint8_t opcode)
{
    switch(opcode)
    {
        case 0x33:
            return R_TYPE;
        case 0x13:
        case 0x3:
        case 0x67:
        case 0x73:
            return I_TYPE;
        case 0x23:
            return S_TYPE;
        case 0x63:
            return B_TYPE;
        case 0x37:
        case 0x17:
            return U_TYPE;
        case 0x6F:
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
    switch (funct3) {
        case 0x0:
            switch (funct7) {
            case 0x00: // ADD
                printf("ADD ");
                cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] + cpu->x[rs2];
                break;
            case 0x20: // SUB
                printf("SUB ");
                cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] - cpu->x[rs2];
                break;
            case 0x01: // MUL (signed * signed)
                printf("MUL ");
                cpu->x[rd] = rd == 0 ? 0 : (cpu->x[rs1] * cpu->x[rs2]) & 0xFFFFFFFF;
                break;
            }
            break;


        case 0x1:
            switch (funct7) {
            case 0x00: // Shift Left Logical
            {
                printf("SLL ");
                // Get only the 5 lower bits for the shift value
                int shift = cpu->x[rs2] & 0x1F;
                cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] << shift;
            }
            break;
            case 0x01: // MUL High (signed * signed)
            {
                printf("MUL High ");
                int64_t mul = (int64_t)cpu->x[rs1] * (int64_t)cpu->x[rs2];
                cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                break;
            }
            }
            break;


        case 0x2:
            switch (funct7) {
                case 0x00: // Set Less Than
                {
                    printf("SLT ");
                    if (rd != 0) 
                    {
                        cpu->x[rd] = cpu->x[rs1] < cpu->x[rs2] ? 1 : 0;
                    }
                    break;
                }
                case 0x01: // MUL High (signed * unsigned)
                {
                    printf("MUL High S*U ");
                    int64_t mul = (int64_t)cpu->x[rs1] * (int64_t)(uint32_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                    break;
                }
            }


        case 0x3:
            switch (funct7) {
            case 0x00: // Set Less Than (Unsigned)
                if (rd != 0) 
                {
                    printf("SLT U ");
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] < (uint32_t)cpu->x[rs2] ? 1 : 0;
                }
                break;
            case 0x01: // MUL High (U)
                if (rd != 0) 
                {
                    printf("MUL High U ");
                    uint64_t mul = (int64_t)(uint32_t)cpu->x[rs1] * (int64_t)(uint32_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                }
                break;
            }


        case 0x4:
            switch (funct7) {
            case 0x00: // XOR
                if (rd != 0) 
                {
                    printf("XOR ");
                    cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
                }
                break;
            case 0x01: // DIV
                if (rd != 0) 
                {
                    printf("DIV ");
                    cpu->x[rd] = cpu->x[rs1] / cpu->x[rs2];
                    // TODO : Check what need to happen when rs2 == x0
                }
                break;
            }

        case 0x5:
            switch (funct7) {
                case 0x00: // SRL (Shift Right Logical)
                {
                    if(rd != 0)
                    {
                        printf("SRL ");
                        // Get only the 5 lower bits for the shift value
                        int shift = cpu->x[rs2] & 0x1F;
                        cpu->x[rd] = (int32_t)((uint32_t)cpu->x[rs1] >> shift);
                    }
                    break;
                }
                case 0x20: // SRA (Shift Right Arithmetic - Extends sign bit)
                {
                    if(rd != 0)
                    {
                        printf("SRA ");
                        // Get only the 5 lower bits for the shift value
                        int shift = cpu->x[rs2] & 0x1F;
                        cpu->x[rd] = cpu->x[rs1] >> shift;
                    }
                    break;
                }
                case 0x01: // DIV
                {
                    printf("DIV ");
                    cpu->x[rd] = cpu->x[rs1] / cpu->x[rs2];
                    break;
                }
            }

        case 0x6:
            switch (funct7) {
                case 0x00: // OR
                {
                    cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
                    break;
                }
                case 0x01: // REM (modulo)
                {
                    cpu->x[rd] = cpu->x[rs1] % cpu->x[rs2];
                    break;
                }
            }

        case 0x7:
            switch (funct7) {
                case 0x00: // AND
                {
                    cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
                }
                    break;
                case 0x01: // REMU (unsigned modulo)
                {
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] % (uint32_t)cpu->x[rs2];
                }
                break;
            }
    }

    printf(" x%d, x%d, x%d\n", rd, rs1, rs2);
    printf("** VALUES AFTER **\n");
    printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[rs2 - x%d] %8X\n", 
        rd, cpu->x[rd], rs1, cpu->x[rs1], rs2, cpu->x[rs2]);
    return 0;
}










int i_type(cpu_t *cpu)
{
    #ifdef DEBUG
    printf("[instruction: 0x%08X] ", cpu->ir);
    #endif
    // Extract values from instruction
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t rd  =       (cpu->ir & 0x00000F80) >> 7;
    uint8_t funct3 =    (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    int32_t imm =       (cpu->ir & 0xFFF00000) >> 20;
    // Sign extend immediate value
    int32_t sign_imm =  ((imm & 0x800) == 0x800) ? imm | 0xFFFFF000 : imm;
    
    #ifdef DEBUG
    printf("| imm %d | rs1 %d | funct3 %d | rd %d | opcode %d |\n", imm, rs1, funct3, rd, (uint8_t) cpu->ir & 0xFF);
    #endif
    printf("** VALUES BEFORE **\n");
    printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[imm] %8X\n", rd, cpu->x[rd], rs1, cpu->x[rs1], imm);
    

    switch (opcode)
    {
        case 0x13:
        {
            switch (funct3) 
            {
                case 0x0:   // ADDI
                    cpu->x[rd] = cpu->x[rs1] + sign_imm;
                    break;
                case 0x1:   // SLLI
                    cpu->x[rd] = cpu->x[rs1] << (imm & 0x1F);
                    break;
                case 0x2:   // SLTI rd = (rs1 < imm)?1:0
                    cpu->x[rd] = cpu->x[rs1] < sign_imm ? 1 : 0;
                    break;
                case 0x3:   // SLTI U
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] < (uint32_t)imm ? 1 : 0;
                    break;
                case 0x4:   // XORI
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] ^ sign_imm;
                    break;
                case 0x5:
                {
                    int shift = imm & 0x1F;
                    if(cpu->ir & 0x40000000)    // SRAI - Sign extension
                    {
                        cpu->x[rd] = cpu->x[rs1] >> shift;
                    }
                    else                        // SRLI
                    {
                        cpu->x[rd] = (int32_t)((uint32_t)cpu->x[rs1] >> shift);
                    }
                    break;
                }
                case 0x6:   // ORI
                {
                    cpu->x[rd] = cpu->x[rs1] | sign_imm;
                    break;
                }
                case 0x7:   // ANDI
                {
                    cpu->x[rd] = cpu->x[rs1] & sign_imm;
                    break;
                }
            }
        }
        case 0x3:
        {
            switch(funct3)
            {
                case 0x0:   // LB: rd = Memory[rsd + imm] 
                {
                    int32_t imm_8b = imm & 0xFF;
                    if(imm_8b & 0x80)
                    {
                        imm_8b |= 0xFFFFFF00;
                    }
                    cpu->x[rd] = read_memory(rs1 + imm);
                    break;
                }
                case 0x1:
                {
                    break;
                }
                case 0x2:
                {
                    break;
                }
                case 0x4:
                {
                    break;
                }
                case 0x5:
                {
                    break;
                }

            }   
        }
        case 0x67:
        case 0x73:

        return 0;
    }
    switch (funct3) {
        case 0x0:
        break;


        case 0x1:
            break;


        case 0x2:
            break;


        case 0x3:
            break;


        case 0x4:
            break;

        case 0x5:
            break;

        case 0x6:
            break;

        case 0x7:
            break;
    }

    printf(" x%d, x%d, %d\n", rd, rs1, imm);
    printf("** VALUES AFTER **\n");
    printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[imm] %8X\n", rd, cpu->x[rd], rs1, cpu->x[rs1], imm);
    return 0;

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

