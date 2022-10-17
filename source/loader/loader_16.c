__asm__(".code16gcc");


#include "loader.h"

static boot_info_t boot_info;

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

static void detect_memory (void) {
    show_msg("try to detect memory");
    uint32_t contID = 0;
    uint32_t signature, bytes;
    boot_info.ram_region_count=0;
    SMAP_entry_t smap_entry;
    
    for(int i = 0; i < BOOT_RAM_REGION_MAX; i++){
        SMAP_entry_t * entry = &smap_entry;

		__asm__ __volatile__ ("int  $0x15" 
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(entry));
        if(signature != 0x534D4150) { //判断是否正确
            show_msg("failed\r\n");
            return ; // error
        }
        if (bytes > 20 && (entry->ACPI & 0x0001) == 0) {
			// ignore this entry
            continue;
        }
        if(entry->Type == 1)
        {
            boot_info.ram_region_cfg[boot_info.ram_region_count].start = entry->BaseL;
            boot_info.ram_region_cfg[boot_info.ram_region_count++].size = entry->LengthL;
        }
        if(contID == 0){
            break;  //检查完毕
        }
    }
    show_msg("ok.\r\n");
}

uint16_t gdt_table[][4] = {
    {0,0,0,0},
    {0xFFFF, 0x0000, 0x9a00, 0x00cf},
    {0xFFFF, 0x0000, 0x9200, 0x00cf}
};

static void enter_protect_mode(void) {
    cli();
    uint8_t v = inb(0x92);
    outb(0x92, v | 0x2);
    lgdt((uint32_t)gdt_table, sizeof(gdt_table));
}

void loader_entry(void)
{
    show_msg("........loadeing");
    detect_memory();
    enter_protect_mode();
    for(;;){}
}