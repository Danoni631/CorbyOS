#ifndef USERSPACE_H
#define USERSPACE_H

void start_userspace();
void draw_wallpaper();
void draw_taskbar();
void update_explorer();
void desktop_icons();
void userspace_state();
void terminal_window();
void draw_boot_screen();
void save_start_area();
void restore_start_area();
void start_button_handler(int x, int y, int pressed);
void draw_loading_bar(int percentage);

#endif