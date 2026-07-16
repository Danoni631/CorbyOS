/*
    CODED BY Danoni631, on 07/14/2026

    This is other things for terminal
*/

#include "kb.h"
#include "mouse.h"
#include "apm.h"
#include "cpu.h"
#include "gpu.h"
#include "cmos.h"
#include "graphics.h"
#include "mem.h"
#include "timer.h"
#include "fs.h"
#include "include/math.h"

#include "corby_ui/gui.h"
#include "corby_ui/userspace.h"

#include "elf.h"

#include "shell_extras.h"
#include "shell.h"

extern INT shell_no_gui;
extern INT WinShX;
extern INT WinShY;
extern INT WinShW;
extern INT WinShH;
extern INT MaxY;
extern WINDOW* terminal;
char* file;
int exit_notepad = 0;

void create_text_file(LPBYTE file, LPBYTE buff, int len)
{
    make_file(file, buff, len, PERM_R | PERM_W);
}

VOID show_info(const CHAR* filename)
{
    setcursorX(0x00);
    setcursorY(86 * 8);

    for (INT x = 0; x < SCREEN_WIDTH; x++)
    {
        for (INT y = 86 * 8; y < 88 * 8; y++)
        {
            set_pixel(x, y, 0x0000AA00);
        }
    }

    file = (char*) filename;
    print("Editing file: ", 0xFF00AA00);

    if (strcmp(filename, "") == 0x00)
    {
        print("Untitled file", 0xFFFFFFFF);
        file = "Untitled";
    }

    setcursorX(0);
    setcursorY(88 * 8);

    print("ESC = exit | ^S = save", 0xFFFFFFFF);

    setcursorX(0);
    setcursorY(0);
}

void escape_notepad()
{
    exit_notepad = 0x01;
}

void start_npad(const char* filename)
{
    clsscreen();
    show_info(filename);
    kb_state(0x04);

    while (!exit_notepad)
    {
        keyboard_handler(NULL);
    }

    kb_state(0x02);
}

VOID start_shel_gui(WINDOW* win)
{
    terminal = win;
    shell_no_gui = 0;
    WinShX = win->x;
    WinShY = win->y + 20;
    WinShW = win->w;
    WinShH = win->h;
    MaxY = win->h;

    draw_rectangle
    (
        win->x,
        win->y + 20,
        win->w,
        win->h,
        0xFF000000
    );

    print("Welcome to CorbyOS Shell in GUI mode!\n\n", 0xFFFFFFFF);
    print("Type 'help' ", 0xFF00FFFF);
    print("to start using the terminal.\n\n", 0xFFFFFFFF);
    print_curr_dir();
    kb_state(0x02);
    save_shell_screen();
}

VOID on_win_moved(WINDOW* win)
{
    if (win == terminal)
    {
        WinShX = win->x;
        WinShY = win->y + 20;
        setcursorY(WinShY + 40);
        blit(win->x, win->y, win->w, win->h, terminal->front_buff);
        save_shell_screen();
    }
}

void hex_dump(LPBYTE data, INT size)
{
    print("\n", 0x00);
    
    for(int i = 0; i < size; i += 16)
    {
        print_hex32(i);
        print("  ", 0x00);

        for(int j = 0; j < 16; j++)
        {
            if(i + j < size) byte_hex(data[i+j]);
            else print("  ", 0x00);
            printout(' ', 0xFFFFFFFF);
            if(j == 7) printout(' ', 0xFFFFFFFF);
        }

        print(" |", 0xFFFFFFFF);

        for(INT j = 0; j < 16 && i+j < size; j++)
        {
            CHAR c = data[i+j];

            if(c >= 32 && c <= 126)
            {
                printout(c, 0xFFFFFFFF);
            }

            else
            {
                printout('.', 0xFFFFFFFF);
            }
        }

        print("\n", 0x00);
    }
}