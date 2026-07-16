/*
    CODED BY Danoni631, on 07/10/2026
    
    APM (Advanced Power Management) is a driver that
    controls the hardware to make changes according to the requests

    (Start UP, Shutdown and restart)
*/

#include "ports.h"

#include "apm.h"

void restart()
{
    outw(0x64, 0xFE);
    while (1) asm("hlt");
}

void shutdown()
{
    outw(0x604, 0x2000);
    while (1) asm("hlt");
}