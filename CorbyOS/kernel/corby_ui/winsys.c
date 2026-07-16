/*
    CODED BY Danoni631 and CHAT, on 07/15/2026

    Is a Windows System
*/

#include "../graphics.h"
#include "../mem.h"
#include "../alloc.h"

#include "winsys.h"

INT start_windows_xy = 0;

static WINDOW* window_head = NULL;
static WINDOW* window_tail = NULL;
static WINDOW* dragged_window = NULL;

static int in_window(WINDOW* win, int x, int y)
{
    if (!win || !win->visible) return 0;
    return x >= win->x && x < win->x + win->w && y >= win->y && y < win->y + win->h;
}

static int in_titlebar(WINDOW* win, int x, int y)
{
    return in_window(win, x, y) && y < win->y + 20;
}

static void remove_window_from_list(WINDOW* window)
{
    if (!window) return;

    if (window->prev)
    {
        window->prev->next = window->next;
    }
    else
    {
        window_head = window->next;
    }

    if (window->next)
    {
        window->next->prev = window->prev;
    }
    else
    {
        window_tail = window->prev;
    }

    window->prev = NULL;
    window->next = NULL;
}

static void add_window_to_front(WINDOW* window)
{
    if (!window) return;

    window->prev = NULL;
    window->next = window_head;

    if (window_head)
    {
        window_head->prev = window;
    }
    else
    {
        window_tail = window;
    }

    window_head = window;
}

WINDOW* create_window(const char* title, int x, int y, int w, int h, DWORD color)
{
    if (w <= 0 || h <= 0) return NULL;

    WINDOW* window = (WINDOW*) alloc_memory(sizeof(WINDOW));
    if (!window) return NULL;

    memset(window, 0, sizeof(WINDOW));

    window->title = title;
    window->color = color;
    window->x = x;
    window->y = y;
    window->w = w;
    window->h = h;
    window->visible = 1;

    window->buffer = (LPDWORD) alloc_memory((DWORD)w * h * sizeof(DWORD));
    window->front_buff = (LPDWORD) alloc_memory((DWORD)w * h * sizeof(DWORD));

    if (!window->buffer || !window->front_buff)
    {
        if (window->buffer) free_memory(window->buffer);
        if (window->front_buff) free_memory(window->front_buff);
        free_memory(window);
        return NULL;
    }

    for (int i = 0; i < w * h; i++)
    {
        window->buffer[i] = color;
    }

    add_window_to_front(window);
    SaveBackgroundArea(window);
    draw_window(window, 1);

    return window;
}

void add_window(WINDOW* window)
{
    if (!window) return;
    add_window_to_front(window);
}

void bring_to_front(WINDOW* window)
{
    if (!window || window == window_head) return;
    remove_window_from_list(window);
    add_window_to_front(window);
}

WINDOW* get_window_at_position(int x, int y)
{
    for (WINDOW* win = window_head; win; win = win->next)
    {
        if (in_window(win, x, y))
        {
            return win;
        }
    }

    return NULL;
}

void draw_window_content(WINDOW* window, int nextwin)
{
    if (!window || !window->visible || !window->buffer) return;
    blit(window->x, window->y, window->w, window->h, window->buffer);

    draw_window_titlebar(window);
}

void draw_window(WINDOW* window, int nextwin)
{
    if (!window || !window->visible) return;
    draw_window_content(window, nextwin);
}

void draw_window_titlebar(WINDOW* window)
{
    if (!window || !window->visible) return;

    int titlebarHeight = 20;
    draw_rectangle(window->x, window->y, window->w, titlebarHeight, 0xFF333333);

    int oldX = getcursorX();
    int oldY = getcursorY();

    setcursorX(window->x + 4);
    setcursorY(window->y + 4);

    if (window->title)
    {
        print(window->title, 0xFFFFFFFF);
    }

    setcursorX(oldX);
    setcursorY(oldY);
}

void draw_all_windows()
{
    WINDOW* current = window_tail;
    while (current)
    {
        draw_window(current, current == window_head ? 1 : 0);
        current = current->prev;
    }
}

void handle_mouse_click(int x, int y, int button_was_pressed)
{
    WINDOW* win = get_window_at_position(x, y);
    if (!button_was_pressed)
    {
        dragged_window = NULL;
        return;
    }

    if (!win) return;

    if (in_titlebar(win, x, y))
    {
        win->dragging = 1;
        win->dragOffsetX = x - win->x;
        win->dragOffsetY = y - win->y;
        dragged_window = win;
        bring_to_front(win);
    }
    else
    {
        bring_to_front(win);
    }
}

VOID handle_dragging(INT mousePressed, int mouseX, int mouseY)
{
    if (!dragged_window) return;

    if (!mousePressed)
    {
        dragged_window->dragging = 0;
        dragged_window = NULL;
        return;
    }

    int newX = mouseX - dragged_window->dragOffsetX;
    int newY = mouseY - dragged_window->dragOffsetY;

    if (newX == dragged_window->x && newY == dragged_window->y) return;

    RestoreWindowArea(dragged_window);
    dragged_window->x = newX;
    dragged_window->y = newY;
    SaveBackgroundArea(dragged_window);
    draw_window(dragged_window, 1);
}

void draw_windows_buttons(WINDOW window)
{
    (void)window;
}

void close_window(WINDOW* window)
{
    if (!window) return;
    RestoreWindowArea(window);
    remove_window_from_list(window);

    if (window->buffer) free_memory(window->buffer);
    if (window->front_buff) free_memory(window->front_buff);
    free_memory(window);
}

VOID force_close_window(WINDOW* window)
{
    close_window(window);
}

void SaveBackgroundArea(WINDOW* win)
{
    if (!win || !win->front_buff) return;

    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            int screenX = win->x + x;
            int screenY = win->y + y;
            if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT)
            {
                win->front_buff[y * win->w + x] = get_pixel(screenX, screenY);
            }
            else
            {
                win->front_buff[y * win->w + x] = 0;
            }
        }
    }
}

void SaveWindowArea(WINDOW* win)
{
    SaveBackgroundArea(win);
}

void RestoreWindowArea(WINDOW* win)
{
    if (!win || !win->front_buff) return;

    for (int y = 0; y < win->h; y++)
    {
        for (int x = 0; x < win->w; x++)
        {
            int screenX = win->x + x;
            int screenY = win->y + y;
            if (screenX >= 0 && screenX < SCREEN_WIDTH && screenY >= 0 && screenY < SCREEN_HEIGHT)
            {
                set_pixel(screenX, screenY, win->front_buff[y * win->w + x]);
            }
        }
    }
}