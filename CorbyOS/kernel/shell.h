#ifndef SHELL_H
#define SHELL_H

#include "include/stdint.h"

extern INT Shell;
extern INT ShellWindowX;
extern INT ShellWindowY;


void start_shell_no_gui();
void shell_window();
void print_welcome_message();
void show_system_directory();
void command_proc(char* command, int x, int y);
void run_proc(char* process);

VOID save_shell_screen();

#endif