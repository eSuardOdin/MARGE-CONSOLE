#include "cpu.h"
#include "bus.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <string.h>


int init_cpu(cpu_t* cpu, bus_t* bus)
{
    cpu->bus = bus;
    cpu->pc = 0;
    cpu->ir = 0;
    return 0;
}


int fetch_instruction(cpu_t* cpu, uint8_t* rom)
{
    cpu->x[0] = 0;
    // printf("[PC: 0x%08X] ", cpu->pc);
    
    
    // Get current pointed to instruction
    uint32_t inst = rom[cpu->pc]             |
                    rom[cpu->pc + 1]  << 8   |
                    rom[cpu->pc + 2]  << 16  |
                    rom[cpu->pc + 3]  << 24;
    
    // Put instruction in instruction register + increment program counter
    cpu->ir = inst;
    // printf(" FETCHED [%08X] \n", cpu->ir);
    cpu->pc += 4;
    return 0;
}

int decode_execute_instruction(cpu_t* cpu)
{
    //print_registers(cpu);
    uint8_t opcode = cpu->ir & 0b1111111;
    e_inst_type inst_type = get_instruction_type(opcode);
    //printf("\nInstruction type is %d", inst_type);
    // switch instruction type
    switch(inst_type)
    {
        case R_TYPE:
            return r_type(cpu);

        case I_TYPE:
            return i_type(cpu);
        case S_TYPE:
            return s_type(cpu);
        case B_TYPE:
            return b_type(cpu);
        case U_TYPE:
            return u_type(cpu);
        case J_TYPE:
            return j_type(cpu);
        case F_TYPE:
            return f_type(cpu);
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
        case 0x7:
        case 0x27:
        case 0x4F:
        case 0x43:
        case 0x45:
        case 0x47:
        case 0x4B:
        case 0x53:
        case 0x55:
            return F_TYPE;
        default:
        return UNKNOWN_TYPE;
        
    }
}

static inline float bits_to_float(int32_t bits)
{
    float f;
    memcpy(&f, &bits, sizeof(float));
    return f;
}

static inline int32_t float_to_bits(float f)
{
    int32_t bits;
    memcpy(&bits, &f, sizeof(int32_t));
    return bits;
}


int f_type(cpu_t *cpu)
{

    // Decode the instruction
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t rd =        (cpu->ir & 0x00000F80) >> 7;
    uint8_t rm =        (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    uint8_t rs2 =       (cpu->ir & 0x01F00000) >> 20;
    uint8_t fmt =       (cpu->ir & 0x06000000) >> 25;   // Used to check if float or double (not sure we implement doubles)
    uint8_t rs3_f5 =    (cpu->ir & 0xF8000000) >> 27;   // May be used as rs3 or funct5 depending on opcode

    // Offset used in flw and fsw instructions
    int32_t imm =       (cpu->ir & 0xFFF00000) >> 20;
    // Sign extend immediate value
    int32_t sign_imm =  ((imm & 0x800) == 0x800) ? imm | 0xFFFFF000 : imm;

    // Switch opcode
    switch(opcode)
    {
        case 0x7: // FLW
        {
            float val =  (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 3) << 24) |
                                    (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 2) << 16) |
                                    (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 1) << 8) |
                                    read_memory(cpu->bus, cpu->x[rs1] + sign_imm); 
                            

            cpu->x[rd] = val;
            printf("Loading %.2f from memory [%08X]\n", bits_to_float(cpu->x[rd]), cpu->x[rs1] + sign_imm);
        }
        break;

        case 0x27: // FSW
        {
            uint8_t imm_1 = (cpu->ir & 0x00000F80) >> 7;          // imm[4:0]
            int32_t imm_2 = (int32_t)(cpu->ir & 0xFE000000) >> 25; // imm[11:5], avec extension de signe via le shift arithmétique
            int32_t s_imm = (imm_2 << 5) | imm_1;

            int32_t val = cpu->x[rs2];
            write_memory(cpu->bus, val & 0xFF, cpu->x[rs1] + s_imm);
            write_memory(cpu->bus, (val & 0xFF00) >> 8, cpu->x[rs1] + s_imm + 1);
            write_memory(cpu->bus, (val & 0xFF0000) >> 16, cpu->x[rs1] + s_imm + 2);
            write_memory(cpu->bus, (val & 0xFF000000) >> 24, cpu->x[rs1] + s_imm + 3);

            printf("Putting %.2f in memory [%08X]\n", bits_to_float(val), cpu->x[rs1] + s_imm);
        }
        break;

        case 0x43:
            // FMADD: f[rd] = f[rs1]×f[rs2]+f[rs3]
            cpu->x[rd] = float_to_bits(
                bits_to_float(cpu->x[rs1]) * 
                   bits_to_float(cpu->x[rs2]) +
                      bits_to_float(cpu->x[rs3_f5])
            ); 
            break;
        case 0x47:
            // FMSUB: f[rd] = f[rs1]×f[rs2]-f[rs3]
            cpu->x[rd] = float_to_bits(
                bits_to_float(cpu->x[rs1]) * 
                   bits_to_float(cpu->x[rs2]) -
                      bits_to_float(cpu->x[rs3_f5])
            );
            break;
        case 0x4B:
            // FNMSUB: f[rd] = -f[rs1]×f[rs2]+f[rs3]
            cpu->x[rd] = float_to_bits(
                -bits_to_float(cpu->x[rs1]) * 
                   bits_to_float(cpu->x[rs2]) +
                      bits_to_float(cpu->x[rs3_f5])
            ); 
            break;

        case 0x4F:
            // FNMADD: f[rd] = -f[rs1]×f[rs2]-f[rs3]
            cpu->x[rd] = float_to_bits(
                -bits_to_float(cpu->x[rs1]) * 
                   bits_to_float(cpu->x[rs2]) -
                      bits_to_float(cpu->x[rs3_f5])
            ); 
            break;        

        case 0x53:
            // Ignore double instructions
            if(!fmt)
            {
                switch(rs3_f5)
                {
                    case 0x0:
                        // FADD: f[rd] = f[rs1] + f[rs2]
                        printf("Adding %.2f to %.2f, result: ", bits_to_float(cpu->x[rs1]), bits_to_float(cpu->x[rs2]));
                        cpu->x[rd] = float_to_bits( bits_to_float(cpu->x[rs1]) + bits_to_float(cpu->x[rs2]));
                        printf("%.2f\n", bits_to_float(cpu->x[rd]));
                        break;
                    case 0x1:
                        // FSUB: f[rd] = f[rs1] - f[rs2]
                        cpu->x[rd] = float_to_bits( bits_to_float(cpu->x[rs1]) - bits_to_float(cpu->x[rs2]));
                        break;
                    case 0x2:
                        // FMUL: f[rd] = f[rs1] * f[rs2]
                        cpu->x[rd] = float_to_bits( bits_to_float(cpu->x[rs1]) * bits_to_float(cpu->x[rs2]));
                        break;
                    case 0x3:
                        // FDIV: f[rd] = f[rs1] / f[rs2]
                        cpu->x[rd] = float_to_bits( bits_to_float(cpu->x[rs1]) / bits_to_float(cpu->x[rs2]));
                        break;
                    case 0x4:
                        switch(rm)
                        {
                            case 0x0:
                                // FSGNJ: f[rd] = {f[rs2][31], f[rs1][30:0]}
                                cpu->x[rd] = (cpu->x[rs2] & 0x80000000) | (cpu->x[rs1] & 0x7FFFFFFF);
                                break;
                            case 0x1:
                                // FSGNJN: f[rd] = {~f[rs2][31], f[rs1][30:0]}
                                cpu->x[rd] = ~(cpu->x[rs2] & 0x80000000) | (cpu->x[rs1] & 0x7FFFFFFF);
                                break;
                            case 0x2:
                                // FSGNJX: f[rd] = {f[rs2][31] ^ f[rs1][30:0]}
                                cpu->x[rd] = (cpu->x[rs2] & 0x80000000) ^ (cpu->x[rs1] & 0x7FFFFFFF);
                                break;
                        }
                        break;
                    case 0x5:
                        switch(rm)
                        {
                            case 0x0:
                            {
                                // FMIN: f[rd] = min(f[rs1], f[rs2])
                                float a = bits_to_float(cpu->x[rs1]);
                                float b = bits_to_float(cpu->x[rs2]);
                                cpu->x[rd] = a < b ? float_to_bits(a) : float_to_bits(b);
                                break;
                            }
                            case 0x1:
                            {
                                // FMAX: f[rd] = max(f[rs1], f[rs2])
                                float a = bits_to_float(cpu->x[rs1]);
                                float b = bits_to_float(cpu->x[rs2]);
                                cpu->x[rd] = a > b ? float_to_bits(a) : float_to_bits(b);
                                break;
                            }
                        }
                        break;
                    
                    case 0xB:
                        // FSQRT: f[rd] = sqrt(f[rs1])
                        cpu->x[rd] = float_to_bits(sqrtf(bits_to_float(cpu->x[rs1])));
                        break;
                    case 0x18:
                        switch (rs2) 
                        {
                            case 0x0:
                            {
                                // FCVT.W.S: x[rd] = sext(f32->s32(f[rs1]))
                                float val = bits_to_float(cpu->x[rs1]);
                                int32_t res = (int32_t)val;
                                cpu->x[rd] = res;
                                break;
                            }
                            case 0x1:
                            {
                                // FCVT.WU.: x[rd] = sext(f32->u32(f[rs1]))
                                float val = bits_to_float(cpu->x[rs1]);
                                uint32_t res = (uint32_t)val;
                                cpu->x[rd] = (int32_t)res;
                                break;
                            }
                        }
                        break;
                    case 0x1C:
                        switch(rm)
                        {
                            case 0x0:
                                // FMV.X.W: x[rd] = sext(f[rs1][31:0])
                                cpu->x[rd] = float_to_bits(cpu->x[rs1]);
                                break;
                            case 0x1:
                                // FCLASS: not implemented - check if relevent
                                break;
                        }
                        break;
                    case 0x14:
                        switch(rm)
                        {
                            case 0x0:
                                // FLE: x[rd] = f[rs1] <= f[rs2]
                                cpu->x[rd] = float_to_bits(cpu->x[rs1]) <= float_to_bits(cpu->x[rs2]) ? 1 : 0;
                                break;
                            case 0x1:
                                // FLT: x[rd] = f[rs1] < f[rs2]
                                cpu->x[rd] = float_to_bits(cpu->x[rs1]) < float_to_bits(cpu->x[rs2]) ? 1 : 0;
                                break;
                            case 0x2:
                                // FEQ: x[rd] = f[rs1] == f[rs2]
                                cpu->x[rd] = float_to_bits(cpu->x[rs1]) == float_to_bits(cpu->x[rs2]) ? 1 : 0;
                                break;
                        }
                        break;
                    case 0x1A:
                        switch(rs2)
                        {
                            case 0x0:
                                // FCVT.S.W: f[rd] = f32_{s32}(x[rs1])
                                cpu->x[rd] = float_to_bits((float)(int32_t)cpu->x[rs1]);
                                break;
                            case 0x1:
                                // FCVT.S.WU: f[rd] = f32_{u32}(x[rs1])
                                cpu->x[rd] = float_to_bits((float)(uint32_t)cpu->x[rs1]);
                                break;
                        }
                        break;
                        
                }
            }
            
            break;

        default:

            break;
    }



    return 0;
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
    
    // printf("| funct7 %d | rs2 %d | rs1 %d | funct3 %d | rd %d | opcode %d |\n", funct7, rs2, rs1, funct3, rd, (uint8_t) cpu->ir & 0xFF);
    // printf("** VALUES BEFORE **\n");
    // printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[rs2 - x%d] %8X\n", 
        // rd, cpu->x[rd], rs1, cpu->x[rs1], rs2, cpu->x[rs2]);
    switch (funct3) {
        case 0x0:
            switch (funct7) {
            case 0x00: // ADD
            cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] + cpu->x[rs2];
    //         printf("ADD x%d = x%d(0x%08X) + x%d(0x%08X) = 0x%08X\n",
    //    rd, rs1, cpu->x[rs1], rs2, cpu->x[rs2], cpu->x[rd]);
                break;
            case 0x20: // SUB
                // printf("SUB ");
                cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] - cpu->x[rs2];
                break;
            case 0x01: // MUL (signed * signed)
            cpu->x[rd] = rd == 0 ? 0 : (cpu->x[rs1] * cpu->x[rs2]) & 0xFFFFFFFF;
        //     printf("MUL x%d = x%d(0x%08X) * x%d(0x%08X) = 0x%08X\n",
        //    rd, rs1, cpu->x[rs1], rs2, cpu->x[rs2], cpu->x[rd]);
                break;
            }
            break;


        case 0x1:
            switch (funct7) {
                case 0x00: // Shift Left Logical
                {
                    // printf("SLL ");
                    // Get only the 5 lower bits for the shift value
                    int shift = cpu->x[rs2] & 0x1F;
                    cpu->x[rd] = rd == 0 ? 0 : cpu->x[rs1] << shift;
                    break;
                }
                case 0x01: // MUL High (signed * signed)
                {
                    // printf("MUL High ");
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
                    // printf("SLT ");
                    if (rd != 0) 
                    {
                        cpu->x[rd] = cpu->x[rs1] < cpu->x[rs2] ? 1 : 0;
                    }
                    break;
                }
                case 0x01: // MUL High (signed * unsigned)
                {
                    // printf("MUL High S*U ");
                    int64_t mul = (int64_t)cpu->x[rs1] * (int64_t)(uint32_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                    break;
                }
            }
            break;


        case 0x3:
            switch (funct7) {
            case 0x00: // Set Less Than (Unsigned)
                if (rd != 0) 
                {
                    // printf("SLT U ");
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] < (uint32_t)cpu->x[rs2] ? 1 : 0;
                }
                break;
            case 0x01: // MUL High (U)
                if (rd != 0) 
                {
                    // printf("MUL High U ");
                    uint64_t mul = (int64_t)(uint32_t)cpu->x[rs1] * (int64_t)(uint32_t)cpu->x[rs2];
                    cpu->x[rd] = rd == 0 ? 0 : (mul >> 32) & 0xFFFFFFFF;
                }
                break;
            }

            break;


        case 0x4:
            switch (funct7) {
            case 0x00: // XOR
                if (rd != 0) 
                {
                    // printf("XOR ");
                    cpu->x[rd] = cpu->x[rs1] ^ cpu->x[rs2];
                }
                break;
            case 0x01: // DIV
                if (rd != 0) 
                {
                    // printf("DIV ");
                    cpu->x[rd] = cpu->x[rs1] / cpu->x[rs2];
                    // TODO : Check what need to happen when rs2 == x0
                }
                break;
            }
            break;

        case 0x5:
            switch (funct7) {
                case 0x00: // SRL (Shift Right Logical)
                {
                    if(rd != 0)
                    {
                        // printf("SRL ");
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
                        // printf("SRA ");
                        // Get only the 5 lower bits for the shift value
                        int shift = cpu->x[rs2] & 0x1F;
                        cpu->x[rd] = cpu->x[rs1] >> shift;
                    }
                    break;
                }
                case 0x01: // DIV
                {
                    // printf("DIV ");
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] / cpu->x[rs2];
                    }
                    break;
                }
            }
            break;

        case 0x6:
            switch (funct7) {
                case 0x00: // OR
                {
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] | cpu->x[rs2];
                    }
                    break;
                }
                case 0x01: // REM (modulo)
                {
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] % cpu->x[rs2];
                    }
                    break;
                }
            }
            break;

        case 0x7:
            switch (funct7) {
                case 0x00: // AND
                {
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] & cpu->x[rs2];
                    }
                    break;
                }
                case 0x01: // REMU (unsigned modulo)
                {
                    if(rd != 0)
                    {
                        cpu->x[rd] = (uint32_t)cpu->x[rs1] % (uint32_t)cpu->x[rs2];
                    }
                    break;
                }
            }
            break;
    }

    // printf(" x%d, x%d, x%d\n", rd, rs1, rs2);
    // printf("** VALUES AFTER **\n");
    // printf("[rd - x%d] %8X\n[rs1 - x%d] %8X\n[rs2 - x%d] %8X\n", 
        // rd, cpu->x[rd], rs1, cpu->x[rs1], rs2, cpu->x[rs2]);
    return 0;
}









int i_type(cpu_t *cpu)
{
    // printf("[instruction: 0x%08X] ", cpu->ir);
    
    // Extract values from instruction
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t rd  =       (cpu->ir & 0x00000F80) >> 7;
    uint8_t funct3 =    (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    int32_t imm =       (cpu->ir & 0xFFF00000) >> 20;
    // Sign extend immediate value
    int32_t sign_imm =  ((imm & 0x800) == 0x800) ? imm | 0xFFFFF000 : imm;
    
    // printf("| imm %d | rs1 %d | funct3 %d | rd %d | opcode %d |\n", imm, rs1, funct3, rd, (uint8_t) cpu->ir & 0xFF);
 

    switch (opcode)
    {
        case 0x73:      // SBREAK / EBREAK
            return 1;
        case 0x13:
        {
            switch (funct3) 
            {
                case 0x0:   // ADDI
                {

                    int32_t rs1_val = cpu->x[rs1];
                    if(rd != 0)
                    {
                        // if(imm == 4)
                        // {
                            //     printf("%d added to x%d\n", imm, rd);
                            //     exit(EXIT_FAILURE);
                            // }
                        cpu->x[rd] = rs1_val + sign_imm;
                    }
                //         printf("ADDI x%d = x%d(0x%08X) + %d = 0x%08X\n",
                //    rd, rs1, rs1_val, sign_imm, cpu->x[rd]);
                }
                break;
                case 0x1:   // SLLI
                    // printf("SLLI ");
                    if(rd != 0)
                    {
                        // printf("  ** Shifting %08X << %d  -  ", cpu->x[rs1], (imm & 0x1F));
                        // printf("  ** Shifting %08X << %0b  -  ", cpu->x[rs1],imm);
                        cpu->x[rd] = cpu->x[rs1] << (imm & 0x1F);
                        // printf(" RESULT: x%d: %08X   **     ", rd, cpu->x[rd]);
                    }
                    break;
                case 0x2:   // SLTI rd = (rs1 < imm)?1:0
                    // printf("SLTI ");
                    if(rd != 0)
                    {
                        cpu->x[rd] = cpu->x[rs1] < sign_imm ? 1 : 0;
                    }
                    break;
                case 0x3:   // SLTI U
                    // printf("SLTI U ");
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] < (uint32_t)imm ? 1 : 0;
                    break;
                case 0x4:   // XORI
                    // printf("XORI ");
                    cpu->x[rd] = (uint32_t)cpu->x[rs1] ^ sign_imm;
                    break;
                case 0x5:
                {
                    int shift = imm & 0x1F;
                    if(cpu->ir & 0x40000000)    // SRAI - Sign extension
                    {
                        // printf("SRAI ");
                        cpu->x[rd] = cpu->x[rs1] >> shift;
                    }
                    else                        // SRLI
                    {
                        // printf("SRLI ");
                        cpu->x[rd] = (int32_t)((uint32_t)cpu->x[rs1] >> shift);
                    }
                    break;
                }
                case 0x6:   // ORI
                {
                    // printf("ORI ");
                    cpu->x[rd] = cpu->x[rs1] | sign_imm;
                    break;
                }
                case 0x7:   // ANDI
                {
                    // printf("ANDI ");
                    cpu->x[rd] = cpu->x[rs1] & sign_imm;
                    break;
                }
            }
            break;
        }
        case 0x3:
        {
            switch(funct3)
            {
                case 0x0:   // LB: rd = Memory[rsd + imm] 
                {
                    // int32_t offset = imm & 0xFF;
                    // if(offset & 0x80)
                    // {
                    //     offset |= 0xFFFFFF00;
                    // }
                    // sign extend readen value (check if a cast does not do it by itself)
                    int32_t val= read_memory(cpu->bus, cpu->x[rs1] + sign_imm);
                    if(val & 0x80)
                    {
                        val |= 0xFFFFFF00;
                    }
                    cpu->x[rd] = val;
                    break;
                }
                case 0x1:   // LH
                {
                    // int32_t offset = imm & 0xFF;
                    // if(imm & 0x80)
                    // {
                    //     offset |= 0xFFFFFF00;
                    // }
                    int32_t val =  (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 1) << 8) |
                                    read_memory(cpu->bus, cpu->x[rs1] + sign_imm); 
                    // sign extend readen value (check if a cast does not do it by itself)
                    if(val & 0x8000)
                    {
                        val |= 0xFFFF0000;
                    }
                    cpu->x[rd] = val;
                    break;
                }
                case 0x2:   // LW
                {
                    int32_t val =   (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 3) << 24) |
                                    (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 2) << 16) |
                                    (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 1) << 8) |
                                    read_memory(cpu->bus, cpu->x[rs1] + sign_imm); 
                    
                    
                    cpu->x[rd] = val;
                    // printf("LW x%d = mem[0x%08X] = 0x%08X\n", rd, cpu->x[rs1] + sign_imm, val);
                    break;
                }
                case 0x4:       // LB (unsigned)
                {
                    int32_t val= read_memory(cpu->bus, cpu->x[rs1] + sign_imm);
                    if(val & 0x80)
                    {
                        val &= 0x000000FF;
                    }
                    cpu->x[rd] = val;
                    break;
                }
                case 0x5:
                {
                    int32_t val =  (read_memory(cpu->bus, cpu->x[rs1] + sign_imm + 1) << 8) |
                                    read_memory(cpu->bus, cpu->x[rs1] + sign_imm); 
                    // zero extend readen value (check if a cast does not do it by itself)
                    if(val & 0x8000)
                    {
                        val &= 0x0000FFFF;
                    }
                    cpu->x[rd] = val;
                    break;
                }

            } 
            break;  
        }
        case 0x67:
            switch(funct3)
            {
                case 0x0:
                {
                    cpu->x[rd] = cpu->pc;// Was : cpu->pc + 4, but fetch already increments PC, check if return address is ok
                    cpu->pc = cpu->x[rs1] + sign_imm;
                    break;
                }
            }
            break;

        return 0;
    }
    
    return 0;   
}

int s_type(cpu_t *cpu)
{
    // printf("[instruction: 0x%08X] ", cpu->ir);
    // Extract values from instruction
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t imm_1  =    (cpu->ir & 0x00000F80) >> 7;
    uint8_t funct3 =    (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    uint8_t rs2 =       (cpu->ir & 0x01F00000) >> 20;
    int32_t imm_2 =     (int32_t)(cpu->ir & 0xFE000000) >> 25;
    
    int32_t imm = (imm_2 << 5) | imm_1;

    // printf("| imm[11:5] %d | rs2 %d | rs1 %d | funct3 %d | imm %d | opcode %d |\n", imm_1, rs2, rs1, funct3, imm_2, (uint8_t) cpu->ir & 0xFF);
    
    switch(funct3)
    {
        case 0x0:       // SB
        {
            // printf("SB: write %02X to %08X\n", cpu->x[rs2] & 0xFF, cpu->x[rs1] + imm);
            write_memory(cpu->bus, cpu->x[rs2] & 0xFF, cpu->x[rs1] + imm);
            break;
        }
        case 0x1:       // SH
        {
            // printf("SH \n");
            write_memory(cpu->bus, cpu->x[rs2] & 0xFF, cpu->x[rs1] + imm);
            write_memory(cpu->bus, (cpu->x[rs2] & 0xFF00) >> 8, cpu->x[rs1] + imm + 1);
            break;
        }
        case 0x2:       // SW
        {
            // printf("SW %08x at [%08x]\n", cpu->x[rs2], cpu->x[rs1] + imm);
            write_memory(cpu->bus, cpu->x[rs2] & 0xFF, cpu->x[rs1] + imm);
            write_memory(cpu->bus, (cpu->x[rs2] & 0xFF00) >> 8, cpu->x[rs1] + imm + 1);
            write_memory(cpu->bus, (cpu->x[rs2] & 0xFF0000) >> 16, cpu->x[rs1] + imm + 2);
            write_memory(cpu->bus, (cpu->x[rs2] & 0xFF000000) >> 24, cpu->x[rs1] + imm + 3);
            break;
        }
    }

    return 0;
}

int b_type(cpu_t *cpu)
{
    // printf("[instruction: 0x%08X] ", cpu->ir);
    // Extract values from instruction
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t funct3 =    (cpu->ir & 0x00007000) >> 12;
    uint8_t rs1 =       (cpu->ir & 0x000F8000) >> 15;
    uint8_t rs2 =       (cpu->ir & 0x01F00000) >> 20;
    int32_t sign_imm;
    int32_t imm = ((cpu->ir >> 31) & 0x1)  << 12    
            | ((cpu->ir >>  7) & 0x1)  << 11        
            | ((cpu->ir >> 25) & 0x3F) << 5      
            | ((cpu->ir >>  8) & 0xF)  << 1;        
    // printf("imm is %b\n", imm);
    if(imm & 0x1000)
    {
        sign_imm = imm | 0xFFFFE000;
    }
    else
    {
        sign_imm = imm;
    }
    imm &= 0x00001FFF;
    switch(funct3)
    {
        case 0x0:       // BEQ
        {
            if(cpu->x[rs1] == cpu->x[rs2])
            {
                cpu->pc += sign_imm - 4;
            }
            break;
        }
        case 0x1:       // BNE
        {
            // printf("BNE\n");
            if(cpu->x[rs1] != cpu->x[rs2])
            {
                // printf("x[%d]: %08X\nx[%d]: %08X\nBranching from %d to %d + %d\n",
                // rs1, cpu->x[rs1], rs2, cpu->x[rs2], cpu->pc, cpu->pc, imm);
                cpu->pc += sign_imm - 4;
                // printf("PC is now %08X\n", cpu->pc);
            }
            break;
        }
        case 0x4:       // BLT
        {
            if(cpu->x[rs1] < cpu->x[rs2])
            {
                cpu->pc += sign_imm - 4;
            }
            break;
        }
        case 0x5:       // BGE
        {
            if(cpu->x[rs1] >= cpu->x[rs2])
            {
                cpu->pc += sign_imm - 4; 
            }
            break;
        }
        case 0x6:       // BLTU
        {
            if(cpu->x[rs1] < cpu->x[rs2])
            {
                cpu->pc += imm - 4;
            }
            break;
        }
        case 0x7:
        {
            if(cpu->x[rs1] >= cpu->x[rs2])
            {
                cpu->pc += imm - 4;
            }
            break;
        }


    }
    return 0;
}

int u_type(cpu_t *cpu)
{
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t rd =        (cpu->ir & 0x00000F80) >> 7;
    uint32_t imm =      (cpu->ir & 0xFFFFF000) >> 12;    
    switch (opcode) 
    {

        case 0x17:      // AUIPC
        {
            cpu->x[rd] = (cpu->pc - 4) + ((imm << 12) & 0xFFFFF000);
            break;
        }

        case 0x37:      // LUI
        {
            cpu->x[rd] = (imm << 12) & 0xFFFFF000;
            // printf("LUI x%d [%08x]\n", rd, cpu->x[rd]);
            break;
        }
    }
    return 0;
}




int j_type(cpu_t *cpu)
{
    uint8_t opcode =    (cpu->ir & 0x0000007F);
    uint8_t rd =        (cpu->ir & 0x00000F80) >> 7;
    int32_t imm = ((cpu->ir >> 31) & 0x1)  << 20    
                | ((cpu->ir >>  21) & 0x3FF)  << 1        
                | ((cpu->ir >> 20) & 0x1) << 11      
                | ((cpu->ir >>  12) & 0xFF)  << 12;

    // Sign extend immediate value
    if(imm & 0x100000)
    {
        imm |= 0xFFE00000;
    }
    // JAL
    cpu->x[rd] = cpu->pc;
    cpu->pc += imm - 4;

    return 0;
}




void print_registers(cpu_t* cpu)
{
    printf("********************************\nIR: [%08X]\t\tPC: [%08X]\n", cpu->ir, cpu->pc);
    for(uint32_t i = 0; i < 32; i += 8)
    {
        printf("[x%2d] %08X | [x%2d] %08X | [x%2d] %08X | [x%2d] %08X | [x%2d] %08X | [x%2d] %08X | [x%2d] %08X | [x%2d] %08X\n",
        i, cpu->x[i], i+1, cpu->x[i+1], i+2, cpu->x[i+2], i+3, cpu->x[i+3], i+4, cpu->x[i+4], i+5, cpu->x[i+5], i+6, cpu->x[i+6], i+7, cpu->x[i+7]);
    }
}
