#ifndef CPU_INSTR_H
#define CPU_INSTR_H
#include "types.h"


static inline uint8_t inb (uint16_t port) {
    uint8_t rv;
    __asm__ __volatile__("inb %[p], %[v]":[v]"=a"(rv) : [p]"d"(port));
    return rv;
}

static inline void outb (uint16_t port,uint8_t data) {
    // outb al, dx
    __asm__ __volatile__("outb %[v], %[p]"::[p]"d"(port),[v]"a"(data));
}

static inline void cli(void) {  // 只能在调用的此文件里面使用
    __asm__ __volatile__("cli");
}

static inline void sti(void) {  // 关中断
    __asm__ __volatile__("sti");
}
static inline void lgdt(uint32_t start, uint32_t size) {
    struct {
        uint16_t limit;  // 块大小
        uint32_t start15_0;  //起始地址低16位
        uint32_t start31_16;  // 起始地址高16位
    } gdt;
    gdt.start31_16 = start>>16;
    gdt.start15_0 = start&0XFFFF;
    gdt.limit = size - 1;

    __asm__ __volatile__("lgdt %[g]"::[g]"m"(gdt));
}

#endif // !CPU_INSTR_H