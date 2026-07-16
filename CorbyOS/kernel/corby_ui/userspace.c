/*
    CODED BY Danoni631, on 07/16/2026

    This is the CorbyUI heart, with the main interface functions
*/

#include "../include/stdint.h"
#include "../graphics.h"
#include "../fs.h"
#include "../elf.h"
#include "../programs.h"
#include "../kb.h"
#include "../mouse.h"
#include "../cmos.h"
#include "../mem.h"
#include "../timer.h"
#include "../shell.h"

#include "gui.h"
#include "winsys.h"
#include "desktop.h"
#include "icons.h"
#include "login_screen.h"
#include "userspace.h"

#define START_BUTTON_W 100
#define START_BUTTON_H 28
#define START_BUTTON_X 12
#define START_BUTTON_Y (SCREEN_HEIGHT - 36)

static DWORD startAreaBuffer[START_BUTTON_W * START_BUTTON_H];
static int startAreaSaved = 0;
static int startMenuVisible = 0;

extern BYTE wallpaper[];

static int point_in_rect(int px, int py, int x, int y, int w, int h)
{
    return px >= x && px < x + w && py >= y && py < y + h;
}

static void render_userspace(void)
{
    draw_wallpaper();
    desktop_icons();
    update_explorer();
    terminal_window();
    draw_taskbar();
    save_start_area();
}

void draw_wallpaper()
{
    int bytes_per_pixel = 3;

    int src_w = 800;
    int src_h = 600;

    int src_row = src_w * bytes_per_pixel;
    int padding = (4 - (src_row % 4)) % 4;

    int src_stride = src_row + padding;

    for (int y = 0; y < SCREEN_HEIGHT - 35; y++)
    {
        int src_y = (y * src_h) / SCREEN_HEIGHT;

        int img_y = src_h - 1 - src_y;

        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            int src_x = (x * src_w) / SCREEN_WIDTH;

            int index = (img_y * src_stride) + (src_x * bytes_per_pixel);

            BYTE b = wallpaper[index];
            BYTE g = wallpaper[index + 1];
            BYTE r = wallpaper[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            set_pixel(x - 28, y, color);
        }
    }
}

void draw_taskbar()
{
    draw_rectangle(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40, 0xFF1F2431);
    
    draw_rectangle
    (
        START_BUTTON_X, START_BUTTON_Y,
        START_BUTTON_W, START_BUTTON_H,
        0xFF3A4C74
    );

    setcursorX(START_BUTTON_X + 18);
    setcursorY(START_BUTTON_Y + 8);
    print("Start", 0xFFFFFFFF);

    setcursorX(SCREEN_WIDTH - 220);
    setcursorY(START_BUTTON_Y + 8);
    print("CorbyOS 1.0 -", 0xFF9CB8E0);
    get_cmos_date();
}

void draw_loading_bar(int percentage)
{
    if (percentage < 0) percentage = 0;
    if (percentage > 100) percentage = 100;

    int width = 640;
    int height = 28;
    int x = (SCREEN_WIDTH - width) / 2;
    int y = (SCREEN_HEIGHT - height) / 2 + 80;

    draw_rectangle(x - 2, y - 2, width + 4, height + 4, 0xFFFFFFFF);
    draw_rectangle(x, y, width, height, 0xFF1A243E);
    draw_rectangle(x, y, (width * percentage) / 100, height, 0xFF4E86FF);

    setcursorX(x + 10);
    setcursorY(y - 26);
    print("Loading CorbyOS...", 0xFFFFFFFF);

    setcursorX(x + width / 2 - 16);
    setcursorY(y + 6);
    printint(percentage, 0xFFFFFFFF);
    print("%", 0xFFFFFFFF);
}

void draw_boot_screen()
{
    draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0xFF000000);
    setcursorX(760);
    setcursorY(520);
    print("CorbyOS booting...", 0xFFFFFFFF);
    draw_loading_bar(55);
}

void save_start_area()
{
    for (int y = 0; y < START_BUTTON_H; y++)
    {
        for (int x = 0; x < START_BUTTON_W; x++)
        {
            startAreaBuffer[y * START_BUTTON_W + x] = 
            get_pixel(START_BUTTON_X + x, START_BUTTON_Y + y);
        }
    }
    startAreaSaved = 1;
}

void restore_start_area()
{
    if (!startAreaSaved) return;

    for (int y = 0; y < START_BUTTON_H; y++)
    {
        for (int x = 0; x < START_BUTTON_W; x++)
        {
            set_pixel
            (
                START_BUTTON_X + x,
                START_BUTTON_Y + y,
                startAreaBuffer[y * START_BUTTON_W + x]
            );
        }
    }
}

void start_button_handler(int x, int y, int pressed)
{
    if (!point_in_rect
        (
            x, y,
            START_BUTTON_X, START_BUTTON_Y,
            START_BUTTON_W, START_BUTTON_H
        )
    ) return;

    if (pressed)
    {
        save_start_area();

        draw_rectangle
        (
            START_BUTTON_X, START_BUTTON_Y,
            START_BUTTON_W, START_BUTTON_H,
            0xFF5675C8
        );

        setcursorX(START_BUTTON_X + 18);
        setcursorY(START_BUTTON_Y + 8);
        print("Start", 0xFFFFFFFF);
        startMenuVisible = 1;

        WINDOW menu;
        menu.title = "Start Menu";
        menu.color = 0xFF283448;
        menu.x = START_BUTTON_X;
        menu.y = SCREEN_HEIGHT - 240;
        menu.w = 240;
        menu.h = 200;
        draw_start_menu(menu);
    }
    else
    {
        if (startMenuVisible)
        {
            render_userspace();
            startMenuVisible = 0;
        }
    }
}

void desktop_icons()
{
    char terminalName[] = "Terminal";
    char homeName[] = "Home";
    char doomName[] = "Doom";
    char programName[] = "App";
    char userName[] = "User";

    draw_desktop_icon(ICON_TERMINAL, terminalName, 120, 120);
    draw_desktop_icon(ICON_FOLDER, homeName, 280, 120);
    draw_desktop_icon(ICON_DOOM, doomName, 440, 120);
    draw_desktop_icon(ICON_PROGRAM, programName, 120, 240);
    draw_desktop_icon(ICON_USER, userName, 280, 240);
}

void update_explorer()
{
    draw_wallpaper();
    desktop_icons();
    draw_taskbar();

    for (INT y = SCREEN_HEIGHT - 35; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            set_pixel(x, y, 0xFF1A1A1A);
        }
    }
}

void terminal_window()
{
    int x = 920;
    int y = 104;
    int w = 640;
    int h = 456;

    draw_rectangle(x, y, w, h, 0xFF101A26);
    draw_rectangle(x, y, w, 32, 0xFF283548);

    setcursorX(x + 12);
    setcursorY(y + 8);
    print("Terminal", 0xFFFFFFFF);

    setcursorX(x + 12);
    setcursorY(y + 44);
    print("C:\\> ", 0xFF9EC5FF);
}

void userspace_state()
{
    int mouseX = 0;
    int mouseY = 0;
    int pressed = 0;
    int prevPressed = 0;

    render_userspace();

    while (1)
    {
        get_state(&mouseX, &mouseY, &pressed);

        if (pressed && !prevPressed)
        {
            if
            (
                point_in_rect
                (
                    mouseX, mouseY,
                    START_BUTTON_X,
                    START_BUTTON_Y,
                    START_BUTTON_W,
                    START_BUTTON_H
                )
            )
            {
                start_button_handler(mouseX, mouseY, 1);
            }
            
            else
            {
                handle_mouse_click(mouseX, mouseY, 1);
            }
        }

        if (!pressed && prevPressed)
        {
            if (startMenuVisible)
            {
                start_button_handler(mouseX, mouseY, 0);
            }
        }

        handle_dragging(pressed, mouseX, mouseY);
        prevPressed = pressed;
        sleep(2);
    }
}

void start_userspace()
{
    render_userspace();
    userspace_state();
}