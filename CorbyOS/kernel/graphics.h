#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "include/stdint.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct
{
    LPDWORD buff;
    int width;
    int height;
} Framebuffer;

void init_graphics(DWORD fb, DWORD pitch);
DWORD get_frame_buff();
DWORD get_pitch();
void set_pixel(int x, int y, DWORD color);
DWORD get_pixel(int x, int y);
void clsscreen(void);
void draw_rectangle(int x, int y, int w, int h, DWORD color);

void draw_round_rect
(
    int x, int y,
    int w, int h,
    int radius,
    DWORD color
);

void draw_circle(int cx, int cy, INT radius, DWORD color);
void blit(int dstx, int dsty, int w, int h, LPDWORD src);

#endif

#ifndef CORBYGL
#define CORBYGL

void StretchBlt
(
    int x1, int y1, int w1, int h1,
    int x2, int y2, int w2, int h2
);

#endif

#ifndef TEXT_H
#define TEXT_H

void draw_char(BYTE* charbitmap, DWORD color);
void print(const char* str, DWORD color);
void debug(const CHAR* str, INT debug);
void inttostr(INT val, char* buff);
VOID printint(int num, DWORD color);
void printhex(int num, DWORD color);
void print_hex32(DWORD value);
void byte_hex(BYTE byte);
void printout(CHAR letter, DWORD color);

int getcursorX();
int getcursorY();

VOID setcursorX(int x);
VOID setcursorY(int y);

static INT a_to_i(const CHAR* string);

#endif