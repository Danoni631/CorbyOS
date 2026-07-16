/*
    CODED ORIGINALLY BY ArTicZera(a.k.a JhoPro), on 06/??/2026
*/

#include "include/stdint.h"
#include "graphics.h"
#include "syscall.h"

void syscall_handler(struct InterruptRegisters* regs)
{
    switch(regs->eax)
    {
        case 4:
        {
            break;
        }

        case 45:
        {
            break;
        }

        case 5:
        {
            break;
        }

        case 6:
        {
            break;
        }

        case 19:
        {
            break;
        }

        case 1:
        {
            break;
        }

        default:
        {
            regs->eax = -1;
            break;
        }
    }
}