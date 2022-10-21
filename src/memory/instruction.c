#include "memory/instruction.h"
#include "cpu/mmu.h"
#include "cpu/register.h"
#include <stdio.h>

static uint64_t decode_od(od_t od){
    if(od.type == IMM){
        return od.imm;
    }
    if(od.type == REG){
        return (uint64_t)od.reg1;
    }
    uint64_t paddr = 0;
    if(od.type == MM_IMM){
        paddr = od.imm;
    }else if(od.type == MM_REG){
        paddr = *(od.reg1);
    }else if(od.type == MM_IMM_REG){
        paddr = *(od.reg1) + od.imm;
    }else if(od.type == MM_REG1_REG2){
        paddr = *(od.reg1) + *(od.reg2);
    }else if(od.type == MM_IMM_REG1_REG2){
        paddr = od.imm + *(od.reg1) + *(od.reg2);
    }else if(od.type == MM_REG2_S){
        paddr = *(od.reg2) * od.scale;
    }else if(od.type == MM_IMM_REG2_S){
        paddr = od.imm + *(od.reg2) * od.scale;
    }else if(od.type == MM_REG1_REG2_S){
        paddr = *(od.reg1) + *(od.reg2) * od.scale;
    }else if(od.type == MM_IMM_REG1_REG2_S){
        paddr = od.imm + *(od.reg1) + *(od.reg2) * od.scale;
    }
    return paddr;
}

void instruction_cycle(){
    inst_t *inst = (inst_t *)reg.rip;
    // reg.rip += sizeof(inst_t);
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
    printf("%s\n", inst -> code);
}

void init_handler_table(){
    handler_table[push_reg] = &push_reg_handler;
    handler_table[pop_reg] = &pop_reg_handler;
    handler_table[mov_reg_reg] = &mov_reg_reg_handler;
    handler_table[mov_reg_mem] = &mov_reg_mem_handler;
    handler_table[mov_mem_reg] = &mov_mem_reg_handler;
    handler_table[add_reg_reg] = &add_reg_reg_handler;
    // handler_table[sub_imm_reg] = &sub_imm_reg_handler;
    handler_table[call] = call_handler;
    handler_table[ret] = &ret_handler;
}

void mov_reg_reg_handler(uint64_t src, uint64_t dst){
    /**
     * src: the pointer of src reg
     * dst: the pointer of dst reg
    */
   *((uint64_t *)dst) = *((uint64_t *)src);
   reg.rip += sizeof(inst_t);
}

void call_handler(uint64_t src, uint64_t dst){
    /**
     * src: imm
     * dst: empty
    */
    // 将下一条指令地址入栈
    reg.rsp -= 8;
    write64bits_dram(
        va2pa(reg.rsp),
        reg.rip + sizeof(inst_t)
    );
    reg.rip = src;
}

void push_reg_handler(uint64_t src, uint64_t dst){
    /**
     * src: the pointer of reg
     * dst: empty
    */
   reg.rsp -= 8;
   write64bits_dram(
    va2pa(reg.rsp),
    *((uint64_t *)src)
   );
   reg.rip += sizeof(inst_t);
}

void mov_reg_mem_handler(uint64_t src, uint64_t dst){
    /**
     * src: the pointer of reg
     * dst: the virtual address
    */
    write64bits_dram(
        va2pa(dst),
        *((uint64_t *)src)
    );
    reg.rip += sizeof(inst_t);
}

void mov_mem_reg_handler(uint64_t src, uint64_t dst){
    /**
     * src: the virtual address
     * dst: the pointer of reg
    */
    *(uint64_t *)dst = read64bits_dram(va2pa(src));
    reg.rip += sizeof(inst_t);
}

void add_reg_reg_handler(uint64_t src, uint64_t dst){
    /**
     * src: the pointer of reg
     * dst: the pointer of reg
    */
    *(uint64_t *)dst += *((uint64_t *)src);
    reg.rip += sizeof(inst_t);
}

void pop_reg_handler(uint64_t src, uint64_t dst){
    /**
     * src: empty
     * dst: the pointer of reg
    */
    *((uint64_t *)dst) = read64bits_dram(
        va2pa(reg.rsp)
    );
    reg.rsp += 8;
    reg.rip += sizeof(inst_t);
}

void ret_handler(uint64_t src, uint64_t dst){
    /**
     * src: empty
     * dst: empty
    */
    reg.rip = read64bits_dram(
        va2pa(reg.rsp)
    );
    reg.rsp += 8;
}