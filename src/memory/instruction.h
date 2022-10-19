#ifndef inst_guard
#define inst_guard

#include <stdlib.h>
#include <stdint.h>

typedef enum OP
{
    push_reg,
    pop_reg,
    mov_reg_reg,
    mov_reg_mem,
    mov_mem_reg,
    add_reg_reg,
    sub_imm_reg,
    call,
    ret
} op_t;

typedef enum OD_TYPE
{
    EMPTY, IMM, REG, 
    MM_IMM, MM_REG, MM_IMM_REG, MM_REG1_REG2,
    MM_IMM_REG1_REG2, MM_REG2_S, MM_IMM_REG2_S,
    MM_REG1_REG2_S, MM_IMM_REG1_REG2_S
} od_type_t;

typedef struct OD
{
    od_type_t type;
    int64_t imm;
    int64_t scale;
    uint64_t *reg1;
    uint64_t *reg2;
} od_t;

typedef struct INST
{
    op_t op;
    od_t src;
    od_t dst;
    char code[100];
} inst_t;

void (*handler_table[30])(uint64_t src, uint64_t dst);

void cycle_instruction();

void init_handler_table();

void push_reg_handler();

void pop_reg_handler();

void mov_reg_reg_handler();

void mov_reg_mem_handler();

void mov_mem_reg_handler();

void add_reg_reg_handler();

void sub_imm_reg_handler();

void call_handler();

void ret_handler();

#endif