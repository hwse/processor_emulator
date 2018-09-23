#include<stdlib.h>
#include<stdio.h>
#include<stdint.h>

#include"processor.h"

processor* init_processor(uint8_t* memory) {
    processor* proc = calloc(sizeof(processor), 1);
    proc->memory = memory;
    proc->prog_counter = memory;
    proc->on = true;
}

void run(processor* proc) {
    while(proc->on) {
        execute_opertion(proc);
    }
}

void run_verbose(processor* proc) {
    print_state(proc);
    while(proc->on) {
        printf("operation: %d next: %d, %d\n", proc->prog_counter[0], proc->prog_counter[1], proc->prog_counter[2]);
        execute_opertion(proc);
        print_state(proc);
    }
}

void print_state(processor* proc) {
    printf("-------STATE-DUMP-------\n");
    printf("- reg_a = %d reg_b = %d reg_c = %d reg_d = %d\n", proc->reg_a, proc->reg_b, proc->reg_c, proc->reg_d);
    printf("- on = %d prog_counter = %d\n", proc->on, proc->prog_counter - proc->memory);
    printf("- zero_flag = %d carry_flag = %d\n", proc->zero_flag, proc->carry_flag);
    printf("------------------------\n");
}

#define MV_CONST(op_name, reg) \
    case op_name: \
        proc->reg = op[1]; \
        proc->prog_counter += 2; \
        break;

#define MV_REGS(op_name, reg_from, reg_to) \
    case op_name: \
        proc->reg_to = proc->reg_from; \
        proc->prog_counter++; \
        break;

#define MV_MEM_REG(op_name, reg) \
    case op_name: \
        temp16 = (op[2] << 8) | op[1]; \
        proc->reg = proc->memory[temp16]; \
        proc->prog_counter += 3; \
        break;

#define MV_REG_MEM(op_name, reg) \
    case op_name: \
        temp16 = (op[2] << 8) | op[1]; \
        proc->memory[temp16] = proc->reg; \
        proc->prog_counter += 3; \
        break;

#define UPDATE_FLAGS \
    proc->carry_flag = temp16 > 0xFF; \
    proc->zero_flag = temp16 == 0; \

// reg_a = reg_a arithm_op second_reg , e.g reg_a = reg_a * reg_b
#define SIMPLE_ARITHMETIC(op_name, arithm_op, second_reg) \
    case op_name: \
        temp16 = proc->reg_a arithm_op proc->second_reg; \
        proc->reg_a = temp16; \
        UPDATE_FLAGS \
        proc->prog_counter++; \
        break;

#define INC(op_name, reg) \
    case op_name: \
        temp16 = proc->reg + 1; \
        proc->reg = temp16; \
        UPDATE_FLAGS \
        proc->prog_counter++; \
        break;

#define DEC(op_name, reg) \
    case op_name: \
        temp16 = proc->reg - 1; \
        proc->reg = temp16; \
        UPDATE_FLAGS \
        proc->prog_counter++; \
        break;

void execute_opertion(processor* proc) {
    operation* op = proc->prog_counter;
    uint16_t temp16;
    switch (*op) {
        case HLT:
            proc->on = false;
            break;
        case NOP: proc->prog_counter++; break;

        //constant move operations
        MV_CONST(MV_CONST_A, reg_a) 
        MV_CONST(MV_CONST_B, reg_b)
        MV_CONST(MV_CONST_C, reg_c)
        MV_CONST(MV_CONST_D, reg_d)

        // move between registers
        MV_REGS(MV_A_B, reg_a, reg_b)
        MV_REGS(MV_A_C, reg_a, reg_c)
        MV_REGS(MV_A_D, reg_a, reg_d)

        MV_REGS(MV_B_A, reg_b, reg_a)
        MV_REGS(MV_B_C, reg_b, reg_c)
        MV_REGS(MV_B_D, reg_b, reg_d)

        MV_REGS(MV_C_A, reg_c, reg_a)
        MV_REGS(MV_C_B, reg_c, reg_b)
        MV_REGS(MV_C_D, reg_c, reg_d)

        MV_REGS(MV_D_A, reg_d, reg_a)
        MV_REGS(MV_D_B, reg_d, reg_b)
        MV_REGS(MV_D_C, reg_d, reg_c)

        //move from memory
        MV_MEM_REG(MV_MEM_A, reg_a)
        MV_MEM_REG(MV_MEM_B, reg_b)
        MV_MEM_REG(MV_MEM_C, reg_c)
        MV_MEM_REG(MV_MEM_D, reg_d)

        //move to memory
        MV_REG_MEM(MV_A_MEM, reg_a)
        MV_REG_MEM(MV_B_MEM, reg_b)
        MV_REG_MEM(MV_C_MEM, reg_c)
        MV_REG_MEM(MV_D_MEM, reg_d)

        // simple arithmetic operations
        SIMPLE_ARITHMETIC(ADD_A, +, reg_a)
        SIMPLE_ARITHMETIC(ADD_B, +, reg_b)
        SIMPLE_ARITHMETIC(ADD_C, +, reg_c)
        SIMPLE_ARITHMETIC(ADD_D, +, reg_d)

        SIMPLE_ARITHMETIC(SUB_A, -, reg_a)
        SIMPLE_ARITHMETIC(SUB_B, -, reg_b)
        SIMPLE_ARITHMETIC(SUB_C, -, reg_c)
        SIMPLE_ARITHMETIC(SUB_D, -, reg_d)

        SIMPLE_ARITHMETIC(MUL_A, *, reg_a)
        SIMPLE_ARITHMETIC(MUL_B, *, reg_b)
        SIMPLE_ARITHMETIC(MUL_C, *, reg_c)
        SIMPLE_ARITHMETIC(MUL_D, *, reg_d)

        INC(INC_A, reg_a)
        INC(INC_B, reg_b)
        INC(INC_C, reg_c)
        INC(INC_D, reg_d)

        DEC(DEC_A, reg_a)
        DEC(DEC_B, reg_b)
        DEC(DEC_C, reg_c)
        DEC(DEC_D, reg_d)

        case JMP:
            temp16 = (op[2] << 8) | op[1];
            proc->prog_counter = proc->memory + temp16;
            break;
        case JZ:
            temp16 = (op[2] << 8) | op[1];
            proc->prog_counter = proc->zero_flag ? proc->memory + temp16 : proc->prog_counter + 3;
            break;
        case JNZ:
            temp16 = (op[2] << 8) | op[1];
            proc->prog_counter = !proc->zero_flag ? proc->memory + temp16 : proc->prog_counter + 3;
            break;
        default: proc->prog_counter++; break; //unkown op is noop
    }
}