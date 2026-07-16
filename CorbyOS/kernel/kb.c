/*
    CODED BY Danoni631, on 07/13/2026
*/

#include "include/stdint.h"
#include "ports.h"
#include "idt.h"
#include "graphics.h"
#include "shell.h"
#include "shell_extras.h"
#include "fs.h"

#include "corby_ui/winsys.h"
#include "corby_ui/gui.h"
#include "corby_ui/userspace.h"
#include "corby_ui/login_screen.h"

#include "kb.h"

const char* KeyMap[][2] =
{
    { " ", " " }, { " ", " " },
    { "1", "!" }, { "2", "@" }, { "3", "#" }, { "4", "$" }, { "5", "%" },
    { "6", "^" }, { "7", "&" }, { "8", "*" }, { "9", "(" }, { "0", ")" },
    { "-", "_" }, { "=", "+" }, { "\b", "\b" }, { "\t", "\t" },
    { "q", "Q" }, { "w", "W" }, { "e", "E" }, { "r", "R" }, { "t", "T" },
    { "y", "Y" }, { "u", "U" }, { "i", "I" }, { "o", "O" }, { "p", "P" },
    { "[", "{" }, { "]", "}" }, { "\n", "\n" }, { " ", " " },
    { "a", "A" }, { "s", "S" }, { "d", "D" }, { "f", "F" }, { "g", "G" },
    { "h", "H" }, { "j", "J" }, { "k", "K" }, { "l", "L" }, { ";", ":" },
    { "'", "\"" }, { "`", "~" }, { " ", " " }, { "\\", "|" },
    { "z", "Z" }, { "x", "X" }, { "c", "C" }, { "v", "V" }, { "b", "B" },
    { "n", "N" }, { "m", "M" }, { ",", "<" }, { ".", ">" }, { "/", "?" },
    { " ", " " }, { " ", " " }, { "*", " " }, { " ", " " }, { " ", " " }
};

INT allow_input = 1;
INT enable_text = 0;

int shift = 0;
int caps = 0;
int ctrl = 0;

extern CHAR* file;

static BYTE keydown[128] = {0};

CHAR cmd_buff[50];
CHAR npad_buff[1024];
INT cmd_leng = 0;
INT npad_leng = 0;

void init_keyboard()
{
    IRQinstall_handler(0x01, keyboard_handler);
}

void keyboard_handler(struct InterruptRegisters *regs)
{
    (void)regs;
    BYTE scan;
    scan = inb(0x60);

    BYTE isPress;
    isPress = !(scan & 0x80);

    if (isPress)
    {
        if (keydown[scan])
        {
            return;
        }

        keydown[scan] = 1;
    }

    else
    {
        keydown[scan] = 0;
    }

    if (allow_input)
    {
        switch (scan)
        {
            case 0x01:
                if (isPress)
                {
                    escape_notepad();
                }

                break;
            
            case 0x0E:
                if (isPress)
                {
                    setcursorX(getcursorX() - 8);
                    print("\f", 0x00000000);
                    setcursorX(getcursorX() - 8);

                    cmd_leng++;
                }

                break;

            case 0x1D:
                ctrl= isPress;
                break;
            
            case 0x1F:
                if (isPress & ctrl)
                {
                    create_text_file
                    (
                        file,
                        (LPBYTE) npad_buff,
                        npad_leng
                    );

                    escape_notepad();

                    break;
                }

                if (isPress)
                {
                    handle_char(scan);
                    break;
                }

                break;

            case 42:
            case 54:
                shift = isPress;
                break;

            case 58:
                if (isPress)
                {
                    caps = !caps;
                }

                break;

            
            case 0x1C:
                if (isPress)
                {
                    cmd_buff[cmd_leng] = '\0';

                    if (enable_text == 2)
                    {
                        command_proc(cmd_buff, 0, 0);
                    }
                    else if (enable_text == 3)
                    {
                        set_password(cmd_buff);
                    }
                    else if (enable_text == 4)
                    {
                        print("\n", 0x00);
                    }
                    else if (enable_text == 5)
                    {
                        command_proc(cmd_buff, 0, 0);
                        save_shell_screen();
                    }

                    cmd_leng++;
                }

                break;

            case 0x5B:
            case 0x5C:
                if (isPress && enable_text == 1)
                {
                    WINDOW* win;
                    win = create_window
                    (
                        "Corby Terminal",
                        80,
                        80,
                        650,
                        400,
                        0xFF1A1A1A
                    );

                    start_shel_gui(win);
                }

                break;

            default:
                if (isPress)
                {
                    handle_char(scan);
                    break;
                }

                break;
        }
    }
}

void handle_char(int scan)
{
    if (cmd_leng >= 49 || npad_leng >= 1024)
    {
        return;
    }

    char c =
    (shift || caps) ? KeyMap[scan][1][0] : KeyMap[scan][0][0];

    cmd_buff[cmd_leng] = c;
    npad_buff[npad_leng] = c;

    if (enable_text == 3) // Password
    {
        printout('*', 0xFFFFFFFF);
        cmd_leng++;
    }

    else if (enable_text == 4) // Notepad
    {
        printout(c, 0xFFFFFFFF);
        cmd_leng++;
        npad_leng++;
    }

    else if (enable_text == 2)
    {
        printout(c, 0xFFFFFFFF);
        cmd_leng++;
    }

    else if (enable_text == 5)
    {
        printout(c, 0xFFFFFFFF);
        cmd_leng++;
    }
}

void kb_state(int state)
{
    enable_text = state;

    if
    (
        enable_text == 0xFF
    ) IRQuninstall_handler(0x01, keyboard_handler);

    if
    (
        enable_text == 0xFE
    ) IRQinstall_handler(0x01, keyboard_handler);
}