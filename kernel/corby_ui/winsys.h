#ifndef WINSYS_H
#define WINSYS_H

#include "../include/stdint.h"

#define MAX_WINDOWS 50
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct WINDOW
{
    const char* title;
    DWORD color;

    int x, y;
    int w, h;

    int dragOffsetX;
    int dragOffsetY;

    int visible;
    int dragging;
    int focused;

    LPDWORD buffer;
    LPDWORD front_buff;

    struct WINDOW* next;
    struct WINDOW* prev;
} WINDOW;

extern INT start_windows_xy;

void draw_window(WINDOW* window, int nextwin);
void add_window(WINDOW* window);
void bring_to_front(WINDOW* window);
void handle_mouse_click(int x, int y, int button_was_pressed);
void draw_all_windows();
WINDOW* get_window_at_position(int x, int y);
WINDOW* create_window(const char* title, int x, int y, int w, int h, DWORD color);
VOID handle_dragging(INT mousePressed, int mouseX, int mouseY);

void draw_button(int x, int y, int size, const char* icon, DWORD color);
void draw_windows_buttons(WINDOW window);
void close_window(WINDOW* window);
void draw_window_titlebar(WINDOW* window);
VOID force_close_window(WINDOW* window);
void draw_window_content(WINDOW* window, int nextwin);

void SaveBackgroundArea(WINDOW* win);
void SaveWindowArea(WINDOW* win);
void RestoreWindowArea(WINDOW* win);

#endif