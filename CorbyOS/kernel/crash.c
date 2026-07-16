/*
    CODE BY Danoni631, on 07/06/2026
    Is a Cyan error screen
*/

#include "include/stdint.h"
#include "graphics.h"
#include "idt.h"

#include "crash.h"

void crash_display(const char* error)
{
    draw_rectangle
    (
        0, 0, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT,
        0x0000FFFF
    );

    setcursorX(0x00);
    setcursorY(0x00);

    print("FATAL ERROR!\nCorbyOS finded a error and needs shutdown or restart!\n\n", 0xFFFF0000);
    print("Possible causes:\nAn unexpected event disrupted kernel execution.\nA critical system component failed to respond.\nIllegal instructions or unauthorized memory access occurred\nA bug started\n\n", 0x00000000);
    print("If this is the first time of you see this screen, restart your machine.\nIf problems persist, contact DaniloSensei on discord, he is the CorbyOS creator", 0xFFFFFFFF);
    print("Error description", 0xFFFF0000);
    print(error, 0xFFFFFFFF);
    print("\n\nRegisters: ", 0x00000000);
}