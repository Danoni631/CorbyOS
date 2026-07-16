#include "../include/stdint.h"
#include "../graphics.h"
#include "../timer.h"

#include "icons.h"
#include "gui.h"
#include "userspace.h"

extern DWORD Terminal[];
extern DWORD Directory[32 * 32];
extern DWORD Program[32 * 2];
extern DWORD Text[32 * 32];
extern DWORD UserIcon[32 * 32];
extern BYTE Sprite[128 * 64];

void draw_start_menu(WINDOW window)
{
    draw_rectangle
    (
        window.x,
        window.y,
        window.w,
        window.h,
        window.color
    );

    setcursorX(window.x / 2 + 50);
    setcursorY(window.y + 20);

    print("User", 0xFFFFFFFF);
}

void draw_window_message
(
    WINDOW window, const char* message,
    WindowIcon icon, WindowButton button
)
{
    int boxX = window.x + 16;
    int boxY = window.y + 40;
    int boxW = window.w - 32;
    int boxH = window.h - 56;

    draw_rectangle(boxX, boxY, boxW, boxH, 0xFF202020);
    draw_rectangle(boxX, boxY, boxW, 24, 0xFF444444);

    int oldX = getcursorX();
    int oldY = getcursorY();

    setcursorX(boxX + 8);
    setcursorY(boxY + 6);
    print("Message", 0xFFFFFFFF);

    int iconColor = 0xFFFFFFFF;
    switch (icon)
    {
        case ICON_ERROR:    iconColor = 0xFFFF0000; break;
        case ICON_WARNING:  iconColor = 0xFFFFFF00; break;
        case ICON_ASTERISK: iconColor = 0xFF00FF00; break;
        case ICON_QUESTION: iconColor = 0xFF00FFFF; break;
    }

    draw_rectangle(boxX + 8, boxY + 36, 24, 24, iconColor);

    setcursorX(boxX + 40);
    setcursorY(boxY + 40);
    print(message, 0xFFFFFFFF);

    const char* buttonText = "OK";
    switch (button)
    {
        case BUTTON_CANCEL: buttonText = "Cancel"; break;
        case BUTTON_YES:    buttonText = "Yes"; break;
        case BUTTON_NO:     buttonText = "No"; break;
        case BUTTON_OK:
        default:            buttonText = "OK"; break;
    }

    int buttonW = 80;
    int buttonH = 24;
    int buttonX = boxX + boxW - buttonW - 8;
    int buttonY = boxY + boxH - buttonH - 8;

    draw_rectangle(buttonX, buttonY, buttonW, buttonH, 0xFF555555);
    setcursorX(buttonX + 8);
    setcursorY(buttonY + 6);
    print(buttonText, 0xFFFFFFFF);

    setcursorX(oldX);
    setcursorY(oldY);
}

void draw_button
(
    int x, int y, int size,
    const char* text, DWORD color
)
{
    draw_rectangle(x, y, size, size, color);

    int oldX = getcursorX();
    int oldY = getcursorY();

    setcursorX(x + 4);
    setcursorY(y + (size / 2) - 4);
    print(text, 0xFFFFFFFF);

    setcursorX(oldX);
    setcursorY(oldY);
}

void draw_desktop_icon
(
    DesktopIcon exticon, char* name,
    int pos_x, int pos_y
)
{
    const DWORD* iconBitmap = NULL;

    switch (exticon)
    {
        case ICON_TXT:    iconBitmap = Text; 
        break;
        
        case ICON_FOLDER: iconBitmap = Directory; 
        break;
        
        case ICON_DOOM: iconBitmap = Doom_Icon;
        break;
        
        case ICON_TERMINAL: iconBitmap = Terminal;
        break;
        
        case ICON_PROGRAM:      iconBitmap = Program;
        break;

        case ICON_USER:   iconBitmap = UserIcon;
        break;
        
        default: return;
    }

    for (int y = 0; y < 32; y++)
    {
        for (int x = 0; x < 32; x++)
        {
            if (iconBitmap[y * 32 + x] == 0xFF000000)
            {
                continue;
            }

            set_pixel
            (
                pos_x + x + 16,
                pos_y + y,
                iconBitmap[y * 32 + x]
            );
        }
    }

    setcursorX(pos_x + 20);
    setcursorY(pos_y + 36);
    print(name, 0xFFFFFFFF);
}



