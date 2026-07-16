/*
    CODED by Danoni631, 0n 07/13/2026

    This code is MUCH necessary for CorbyUI. Is a mouse driver
    (Work with Touchpad)
*/

#include "ports.h"
#include "graphics.h"
#include "idt.h"
#include "timer.h"
#include "corby_ui/gui.h"
#include "corby_ui/winsys.h"

#include "mouse.h"

BYTE cursor_bitmap[] =
{
    0b10000000,
    0b11000000,
    0b11100000,
    0b11110000,
    0b11111000,
    0b11111100,
    0b11111110,
    0b11111111,
    0b10111101,
    0b00011110,
    0b00001111,
    0b00000111,
};

DWORD MouseArea[WCURSOR * HCURSOR];
DWORD CursorAfter[WCURSOR * HCURSOR];
DWORD MousePreview[WCURSOR * HCURSOR];

BYTE MouseData;
BYTE MouseCycle;
BYTE MousePacket[4];

int WasPressed = 0;

INT PrevCurX = 0;
INT PrevCurY = 0;

int mouseX = 0;
int mouseY = 0;

void clear_trails(INT prevcurX, INT prevcurY);

void init_mouse()
{
    outb(0x64, 0xA7);
    outb(0x64, 0xA8);

    while (inb(0x64) & 1) inb(0x60);

    IRQinstall_handler(12, mouse_handler);
    outb(0x64, 0x20);
    BYTE status = (inb(0x60) | 2);
    outb(0x64, 0x60);
    outb(0x60, status);

    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
}

void mouse_handler(struct InterruptRegisters *regs)
{
    (void)regs;
    MouseData = inb(0x60);

    if (MouseCycle == 0 && !(MouseData & 0x08))
    {
        return;
    }

    MousePacket[MouseCycle++] = MouseData;

    if (MouseCycle == 3)
    {
        MouseCycle = 0;

        WasPressed = (MousePacket[0] & 0x1);

        int deltaX, deltaY;

        deltaX = (int)((char)MousePacket[1]);
        deltaY = (int)((char)MousePacket[2]);

        INT prevMouseX = mouseX;
        INT prevMouseY = mouseY;

        mouseX += deltaX;
        mouseY -= deltaY;

        mouseX = (mouseX < 0) ? 0 :
        (
            (mouseX > SCREEN_WIDTH - 16)
            ? SCREEN_WIDTH - 16 : mouseX
        );

        mouseY = (mouseY < 0) ? 0 :
        (
            (mouseY > SCREEN_HEIGHT - 16)
            ? SCREEN_HEIGHT - 16 : mouseY
        );

        clear_trails(prevMouseX, prevMouseY);
        save_cur_area();
        draw_mouse(0xFFFFFFFF);
    }
}

void mouse_wait()
{
    while
    (
        inb(0x64)
        & 0x02
    );
}

void restore_preview_cur()
{
    for (INT y = 0; y < HCURSOR; y++)
    {
        for (INT x = 0; x < WCURSOR; x++)
        {
            set_pixel
            (
                mouseX + x,
                mouseY + y,
                MouseArea[y * WCURSOR + x]
            );
        }
    }
}

void save_cur_area()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            DWORD color = get_pixel(mouseX + x, mouseY + y);

            MouseArea[y * WCURSOR + x] = color;
        }
    }
}

void draw_mouse(DWORD color)
{
    BYTE byte;

    for (int y = 0; y < HCURSOR; y++)
    {
        int x = 0;

        for (int i = 0; i < 2; i++)
        {   
            byte = cursor_bitmap[y * 2 + 1];

            for (int j = 7; j >= 0; j--)
            {
                if (byte & (1 << j))
                {
                    set_pixel
                    (
                        mouseX + x,
                        mouseY + y,
                        color
                    );
                }

                x++;
            }
        }
    }
}

void get_state(int* x, int* y, int* pressed)
{
    *x = mouseX;
    *y = mouseY;
    *pressed = WasPressed;
}

void clear_trails(INT prevcurX, INT prevcurY)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            set_pixel
            (
                prevcurX + x,
                prevcurY + y,
                MouseArea[y * WCURSOR + x]
            );
        }
    }
}