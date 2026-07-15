#include "include/stdint.h"

struct multiboot_info;

uint32_t get_RAM_capacity(struct multiboot_info* mb_info);
const char* get_RAM_type(VOID);
void init_ram(struct multiboot_info* mb_info, int verbose);
