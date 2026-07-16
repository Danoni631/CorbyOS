/*
    CODED BY Danoni631, on 07/14/2026

    This is CorbyOS terminal, like MS-DOS
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
#include "programs.h"

#include "corby_ui/winsys.h"
#include "corby_ui/gui.h"
#include "corby_ui/userspace.h"

#include "elf.h"

#include "shell_extras.h"
#include "shell.h"

INT shell_no_gui = 1;
INT WinShX, WinShY = 0;
INT WinShW = 1280;
INT WinShH = 720;
INT MaxY = 720;

WINDOW* terminal;

void start_shell_no_gui()
{
    print_welcome_message();
    print_curr_dir();
    kb_state(0x02);
}

void print_welcome_message()
{
    setcursorX(0x00);
    setcursorY(0x00);
    print("Welcome to CorbyOS Shell!\n\n", 0xFFFFFFFF);
    print("Type 'gui' to start using ", 0xFF00FFFF);
    print("CorbyUI.\n", 0xFFFFFFFF);
    print("Type 'help' ", 0xFF00FFFF);
    print("to start using the terminal.\n\n", 0xFFFFFFFF);
}

void command_proc(char* command, int x, int y)
{
    char cmd[16] = {0};
    char args[2][16] = {{0}};
    INT command_index, arg_index, argument_count, i;
    command_index = 0;
    arg_index = 0;
    argument_count = 0;
    i = 0;

    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[command_index++] = command[i++];
    }

    cmd[command_index] = '\0';

    if (command[i] == ' ') i++;

    while (command[i] != '\0' && argument_count < 2)
    {
        if (command[i] == ' ')
        {
            args[argument_count][arg_index] = '\0';
            argument_count++;
            arg_index = 0;
        }
        else
        {
            args[argument_count][arg_index++] = command[i];
        }
        i++;
    }

    if (arg_index > 0 && argument_count< 2)
    {
        args[argument_count][arg_index] = '\0';
        arg_index++;
    }

    if (getcursorX() >= MaxY)
    {
        setcursorY(WinShY + 20 + y);

        draw_rectangle
        (
            WinShX,
            WinShY,
            WinShW,
            WinShH,
            0xFF000000
        );
    }

    if (strcmp(cmd, "help") == 0x00)
    {
        print("\n\ncls - Clear the screen", 0xFFFFFFFF);
        print("\nhelp - Show this list", 0xFFFFFFFF);
        print("\nhexdump - Hex dump", 0xFFFFFFFF);
        print("\nshutdown - Power off machine", 0xFFFFFFFF);
        print("\nrestart - Reboot machine", 0xFFFFFFFF);
        print("\nsystem - Show machine specifications", 0xFFFFFFFF);
        print("\ngui - Open CorbyUI", 0xFFFFFFFF);
        print("\nmf - Create file", 0xFFFFFFFF);
        print("\ncat - Read file content", 0xFFFFFFFF);
        print("\ninfo - Show file informations", 0xFFFFFFFF);
        print("\nrename - Rename a file", 0xFFFFFFFF);
        print("\ndf - Delete a file", 0xFFFFFFFF);
        print("\ndir - Show all root", 0xFFFFFFFF);
        print("\nmd - Create a directory", 0xFFFFFFFF);
        print("\ndd - Delete a directory", 0xFFFFFFFF);
        print("\ncd - Change current directory", 0xFFFFFFFF);
        print("\nnpad - Terminal notepad", 0xFFFFFFFF);
        print("\nrun - Run a program", 0xFFFFFFFF);
        print("\nabout - About of CorbyOS", 0xFFFFFFFF);
    }

    else if (strcmp(cmd, "about") == 0x00)
    {
        print("\n\nDeveloper: ", 0xFFFF0000);
        print("Danoni631/Danilo Sensei", 0xFFFFFFFF);
        print("\nDevelopment started on: ", 0xFFFF0000);
        print("06/30/2026", 0xFFFFFFFF);

        print
        (
            "\n\nThis OS is made for educational purposes,",
            0xFF00FFFF
        );

        print("\nand is my first C OS", 0xFFFFFFFF);

        print("\n\nCredits: ", 0xFFFF0000);
        print("JhoPro | Mell-o-tron | OS Dev Wiki", 0xFFFFFFFF);

    }

    else if (strcmp(cmd, "cls") == 0x00)
    {
        setcursorX(WinShX);
        setcursorY(WinShY);

        draw_rectangle
        (
            WinShX,
            WinShY,
            WinShW,
            WinShH,
            0x00000000
        );
    }

    else if (strcmp(cmd, "shutdown") == 0x00)
    {
        print("Leaving...", 0xFFFFFFFF);
        sleep(10);
        shutdown();
    }

    else if (strcmp(cmd, "restart") == 0x00)
    {
        print("Rebooting...", 0xFFFFFFFF);
        sleep(10);
        restart();
    }

    else if (strcmp(cmd, "system") == 0x00)
    {
        print("\n\n", 0x0F);
        print("==================================\n", 0xFFFFFFFF);
        print("=                                =\n", 0xFFFFFFFF);
        print("=                                =\n", 0xFFFFFFFF);
        print("=     ", 0xFFFFFFFF);
        print("LOCAL_DISK/CORBY_CORE/", 0xFF00FFFF);
        print("     =\n", 0xFFFFFFFF);
        print("     -----------------------     =\n\n", 0xFFFFFFFF);
        print("OS: ", 0xFF00FFFF); print("CorbyOS\n", 0xFFFFFFFF);
        print("Kernel: ", 0xFF00FFFF); print("1.0\n", 0xFFFFFFFF);
        print("Architeture: ", 0xFF00FFFF);
        print("32-bit\n", 0xFFFFFFFF);
        print("Resolution: ", 0xFF00FFFF);
        print("1920x1080\n", 0xFFFFFFFF);
        print("CPU: ", 0xFF00FFFF); show_cpu_name();
        print("Date: ", 0xFF00FFFF); get_cmos_date();
        print("\n", 0x00);
        print("\f\f", 0xFF0E1A14);
        print("\f\f", 0xFF1B2A21);
        print("\f\f", 0xFF23382C);
        print("\f\f", 0xFF2F4A38);
        print("\f\f", 0xFF3F5F49);
        print("\f\f", 0xFF5F7566);
        print("\f\f", 0xFF8A9A90);
        print("\f\f\n", 0xFFB7C0B9);
    }

    else if (strcmp(cmd, "gui") == 0x00)
    {
        userspace_state();
    }

    else if (strcmp(cmd, "mf") == 0x00)
    {
        make_file
        (
            args[0],
            args[1],
            sizeof(args[1]),
            PERM_R | PERM_W
        );
    }

    else if (strcmp(cmd, "md") == 0x00)
    {
        make_folder(args[0]);
    }

    else if (strcmp(cmd, "cat") == 0x00)
    {
        BYTE buffer[128];
        DWORD size;

        int result = read_file(args[0], buffer, &size);
        
        if (result != -1)
        {
            print("\n\n", 0x00);
            print(buffer, 0xFFFFFFFF);
        }
    }

    else if (strcmp(cmd, "info") == 0x00)
    {
        file_info(args[0]);
    }

    else if (strcmp(cmd, "rename") == 0x00)
    {
        rename_file(args[0], args[1]);
    }

    else if (strcmp(cmd, "df") == 0x00)
    {
        del_file(args[0]);
    }

    else if (strcmp(cmd, "dd") == 0x00)
    {
        del_folder(args[0]);
    }

    else if (strcmp(cmd, "dir") == 0x00)
    {
        print("Local Disk\n\n", 0xFFFFFFFF);
        show_all_files();
        show_all_folders();
    }

    else if (strcmp(cmd, "cd") == 0x00)
    {
        change_dir(args[0]);
        print("Shell dir was changed", 0xFFFFFFFF);
    }

    else if (strcmp(cmd, "run") == 0x00)
    {
        run_program(args[0]);
    }

    else if (strcmp(cmd, "hexdump") == 0x00)
    {
        BYTE buff[512];
        DWORD buff_size;
        INT size = read_file(args[0], buff, &buff_size);

        if (size < 0)
        {
            print("Don't found file!", 0xFFFF0000);
            return;
        }

        hex_dump(buff, buff_size);
    }

    else if (strcmp(cmd, "npad") == 0x00)
    {
        start_npad(args[0]);
    }

    else
    {
        print("Invalid command!\n", 0xFFFF0000);
        print("Try again or type 'help'", 0xFFFFFFFF);
    }

    print("\n\n", 0x00);
    print_curr_dir();
}

void run_proc(char* process)
{
    if (strcmp(process, "doom") == 0x00)
    {
        force_close_window(terminal);
        WINDOW* Doom =
        create_window
        (
            "Doom",
            320, 160,
            640, 400,
            0xFF1A1A1A   
        );

        kb_state(0xFF);
        start_elf(Doom, 1);
        force_close_window(Doom);
        kb_state(0xFE);
    }

    else
    {
        print("\n\nInvalid Program", 0xFFFF0000);
    }
}

VOID save_shell_screen()
{
    for (int y = 0; y < terminal->h; y++)
    {
        for (int x = 0; x < terminal->w; x++)
        {
            terminal->front_buff[y * terminal->w + x] =
            get_pixel
            (
                x + terminal->x,
                y + terminal->y
            );
        }
    }
}