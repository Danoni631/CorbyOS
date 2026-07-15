#include "../include/stdint.h"
#include "../graphics.h"
#include "../kb.h"
#include "../mem.h"

#include "gui.h"
#include "winsys.h"
#include "login_screen.h"

extern CHAR user[];

CHAR userpass[16];
INT loginloop = 1;

void login_screen()
{
    draw_rectangle
    (
        0, 0,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0xF2F2F2F2
    );

    INT w = 350;
    INT h = 150;

    int x = SCREEN_WIDTH / 2 - w / 2;
    int y = SCREEN_HEIGHT / 2 - h / 2 - 50;

    y += h / 4;

    for (int ys = 0; ys < 32; ys++)
    {
        for (int xs = 0; xs < 32; xs++)
        {
            BYTE color = user[ys * 32 + (xs + 8)];

            if (color != 0x00 && color != 28)
            {
                set_pixel(x + xs * 2, y + ys * 2, color ? 0xFFFFFFFF : 0x00);
                set_pixel(x + xs * 2 + 1, y + ys * 2, color ? 0xFFFFFFFF : 0x00);
                set_pixel(x + xs * 2, y + ys * 2 + 1, color ? 0xFFFFFFFF : 0x00);
                set_pixel(x + xs * 2 + 1, y + ys * 2 + 1, color ? 0xFFFFFFFF : 0x00);
            }
        }
    }

    setcursorX(x + 70);
    setcursorY(y + 20);
    print("Hello, user\n", 0xFFFFFFFF);
    setcursorX(x);
    setcursorY(y + 80);
    print("Create a password", 0xFFFFFFFF);
    draw_round_rect(x, y + 100, 300, 20, 5, 0xFFFFFFFF);
    setcursorX(x + 5);
    setcursorY(y + 100);

    while (loginloop)
    {
    }
}

void set_password(char* password)
{
    memcpy(userpass, password, sizeof(userpass));
    loginloop = 0x00;
}