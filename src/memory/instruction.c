#include "memory/instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"

static uint64_t decode_od(od_t od){
    if(od.type == IMM){
        return od.imm;
    }
    if(od.type == REG){
        return (uint64_t)od.reg1;
    }
    uint64_t vaddr = 0;
    if(od.type == MM_IMM){
        vaddr = od.imm;
    }else if(od.type == MM_REG){
        vaddr = *(od.reg1);
    }else if(od.type == MM_IMM_REG){
        vaddr = *(od.reg1) + od.imm;
    }else if(od.type == MM_REG1_REG2){
        vaddr = *(od.reg1) + *(od.reg2);
    }else if(od.type == MM_IMM_REG1_REG2){
        vaddr = od.imm + *(od.reg1) + *(od.reg2);
    }else if(od.type == MM_REG2_S){
        vaddr = *(od.reg2) * od.scale;
    }else if(od.type == MM_IMM_REG2_S){
        vaddr = od.imm + *(od.reg2) * od.scale;
    }else if(od.type == MM_REG1_REG2_S){
        vaddr = *(od.reg1) + *(od.reg2) * od.scale;
    }else if(od.type == MM_IMM_REG1_REG2_S){
        vaddr = od.imm + *(od.reg1) + *(od.reg2) * od.scale;
    }
    return vaddr;
}

void cycle_instruction(){
    inst_t *inst = (inst_t *)reg.rip;
    reg.rip += sizeof(inst_t);
    op_t op = inst->op;
    /**
     *   op:
     *   IMM: the value of the immediate number
     *   REG: the point of register
     *   MM:  physical address
    */
    uint64_t src = decode_od(inst->src);
    uint64_t dst = decode_od(inst->dst);
    handler_table[op](src, dst);

}

void init_handler_table(){
    // handler_table[push_reg] = &push_reg_handler;
    // handler_table[pop_reg] = &pop_reg_handler;
    // handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    // handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    // handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    // handler_table[add_reg_reg] = &add_reg_reg_handler;
    // handler_table[sub_imm_reg] = &sub_imm_reg_handler;
    // handler_table[call] = &call_handler;
    // handler_table[ret] = &ret_handler;
}