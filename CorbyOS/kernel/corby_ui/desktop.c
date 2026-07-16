/*
    CODED BY ArTicZera, on 01/12/2025

    This is the desktop items functions

    I get from ArTic NovaOS desktop system
*/

#include "../include/stdint.h"
#include "../graphics.h"
#include "../mem.h"
#include "../fs.h"
#include "../shell.h"
#include "../shell_extras.h"
#include "../elf.h"
#include "../kb.h"

#include "winsys.h"
#include "gui.h"
#include "userspace.h"
#include "desktop.h"

extern CHAR doom[];

DesktopItem items[MAXITEMS];
INT total_items = 0;

DesktopItem* GetClickedItem(int x, int y)
{
    for (INT i = 0; i < total_items; i++)
    {
        if
        (
            x >= items[i].x && x < items[i].x + 100
            && y >= items[i].y && items[i].y + 85
        )
        {
            return &items[i];
        }
    }

    return 0;
}

void HandleDesktopClick(int x, int y, int pressed)
{
    static INT LastPressed;
    LastPressed = 0;

    static DesktopItem* last_item;
    last_item = NULL;

    static DWORD last_click_time = 0;

    if (pressed && !LastPressed)
    {
        DesktopItem* item = GetClickedItem(x, y);

        if (item)
        {
            if (item == last_item)
            {
                if (strcmp(item->name, "Shell") == 0)
                {
                    WINDOW* window =
                    create_window
                    (
                        "Corby Shell",
                        80, 80, 650, 432,
                        0xFF1A1A1A
                    );

                    start_shel_gui(window);
                }

                else if (strcmp(item->name, "Doom") == 0)
                {
                    create_window
                    (
                        "DOOM",
                        320, 160,
                        640, 400,
                        0xFF1A1A1A
                    );

                    kb_state(6);
                    start_elf(doom, 0);
                }

                last_item = NULL;
                last_click_time = 0;
            }

            else
            {
                last_item = item;
                last_click_time = 0;
            }
        }

        else
        {
            last_item = NULL;
        }
    }

    LastPressed = pressed;
}

void SetDesktopIndex(char* name, int type, int x, int y)
{
    if (total_items < MAXITEMS)
    {
        strncpy(items[total_items].name, name, 15);

        items[total_items].name[15] = '\0';
        items[total_items].type = type;
        items[total_items].x = x;
        items[total_items].y = y;

        total_items++;
    }
}