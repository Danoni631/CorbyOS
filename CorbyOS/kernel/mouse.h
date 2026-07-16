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

extern BYTE cursor_bitmap[];