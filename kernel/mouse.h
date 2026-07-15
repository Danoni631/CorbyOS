#define WCURSOR 8
#define HCURSOR 12

#include "include/stdint.h"

struct InterruptRegisters;

void init_mouse();
void mouse_handler(struct InterruptRegisters *regs);
void mouse_wait();
void restore_preview_cur();
void save_cur_area();
void draw_mouse(DWORD color);
void get_state(int* x, int* y, int* pressed);

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