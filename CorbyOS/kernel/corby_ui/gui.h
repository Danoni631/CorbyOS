#ifndef GUI_H
#define GUI_H

#include "winsys.h"

typedef enum
{
    ICON_ERROR, ICON_WARNING, ICON_ASTERISK, ICON_QUESTION
} WindowIcon;

typedef enum
{
    BUTTON_OK, BUTTON_CANCEL, BUTTON_YES, BUTTON_NO
} WindowButton;

typedef enum
{
    ICON_USER, ICON_FOLDER, ICON_FILE, ICON_PROGRAM, ICON_TXT, ICON_DOOM, ICON_PNG, ICON_JPG,
    ICON_BMP, ICON_PCM, ICON_MP4, ICON_VAD, ICON_TERMINAL
} DesktopIcon;

void draw_start_menu(WINDOW window);

void draw_window_message
(
    WINDOW window, const char* message,
    WindowIcon icon, WindowButton button
);

void draw_button
(
    int x, int y, int size,
    const char* text, DWORD color
);

void draw_desktop_icon
(
    DesktopIcon exticon, char* name,
    int pos_x, int pos_y
);

#endif