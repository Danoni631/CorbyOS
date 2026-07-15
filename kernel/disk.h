#define IDE_PRIMARY_COMMAND_PORT   0x1F0
#define IDE_PRIMARY_CONTROL_PORT   0x3F6
#define IDE_STATUS_READY           0x40
#define IDE_STATUS_ERROR           0x01

#define FLOPPY_STATUS_PORT         0x3F0
#define FLOPPY_READY               0x80

#include "include/stdint.h"

void wait_for_ready(WORD base);
DWORD get_disk_capacity(WORD base);
DWORD get_floppy_capacity(VOID);
void list_all_disks(VOID);