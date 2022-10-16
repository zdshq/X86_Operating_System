__asm__(".code16gcc");


#include "comm/boot_info.h"

static _boot_info_t boot_info;

static void show_msg (const char *msg)
{
    char c;
    while((c = *msg++) != '\0')
    {
        asm(
            "mov $0xe, %%ah\n\t"
            "mov %[ch], %%al\n\t"
            "int $0x10"::[ch]"r"(c)
        );
    }
}

void loader_entry(void)
{
    show_msg("........loadeing");
    for(;;){}
}