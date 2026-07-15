#include "include/stdint.h"
#include "corby_ui/winsys.h"

void hex_dump(LPBYTE data, INT size);

#ifndef NPAD_H
#define NPAD_H

VOID show_info(const CHAR* filename);
VOID start_shel_gui(WINDOW* win);
VOID on_win_moved(WINDOW* win);

void start_npad(const char* filename);
void escape_notepad();
void create_text_file(LPBYTE file, LPBYTE buff, int len);

#endif