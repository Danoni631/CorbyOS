/*
    CODED BY Danoni631, on 07/08/2026
    Have drawing functions
*/

#include "alloc.h"
#include "timer.h"

#include "graphics.h"

DWORD video_memory;
DWORD p;

void init_graphics(DWORD fb, DWORD pitch)
{
    video_memory = fb;
    p = pitch;
}

DWORD get_frame_buff()
{
    return video_memory;
}

DWORD get_pitch()
{
    return p;
}

void set_pixel(int x, int y, DWORD color)
{
    LPDWORD frame_buff = (LPDWORD) video_memory;
    frame_buff[x + (y * p / 4)] = color;
}

DWORD get_pixel(int x, int y)
{
    LPDWORD frame_buff = (LPDWORD) video_memory;
    return frame_buff[x + (y * p / 4)];
}

void clsscreen(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            set_pixel(x, y, 0x00);
        }
    }
}

void draw_rectangle(int x, int y, int w, int h, DWORD color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            set_pixel(j, i, color);
        }
    }
}

void draw_round_rect
(
    int x, int y,
    int w, int h,
    int radius,
    DWORD color
)
{
    int right = x + w;
    int bottom = y + h;

    DWORD pixelColor = color;

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x + radius; j < right - radius; j++) 
        {
            set_pixel(j, i, pixelColor);
        }
    }

    for (int i = x + radius; i < right - radius; i++) 
    {
        for (int j = y; j < y + radius; j++) 
        {
            set_pixel(i, j, pixelColor);
        }

        for (int j = bottom - radius; j < bottom; j++) 
        {
            set_pixel(i, j, pixelColor);
        }
    }

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x; j < x + radius; j++) 
        {
            set_pixel(j, i, pixelColor);
        }
        for (int j = right - radius; j < right; j++) 
        {
            set_pixel(j, i, pixelColor);
        }
    }

    for (int dx = -radius; dx <= radius; dx++) 
    {
        for (int dy = -radius; dy <= radius; dy++) 
        {
            if (dx * dx + dy * dy <= radius * radius) 
            {
                set_pixel
                (
                    x + radius + dx,
                    y + radius + dy,
                    pixelColor
                );

                set_pixel
                (
                    right - radius - 1 + dx,
                    y + radius + dy,
                    pixelColor
                );

                set_pixel
                (
                    x + radius + dx,
                    bottom - radius - 1 + dy,
                    pixelColor
                );

                set_pixel
                (
                    right - radius - 1 + dx,
                    bottom - radius - 1 + dy,
                    pixelColor
                );
            }
        }
    }
}

void draw_circle(int cx, int cy, INT radius, DWORD color)
{
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    while (y >= x)
    {
        set_pixel(cx + x, cy + y, color);
        set_pixel(cx - x, cy + y, color);
        set_pixel(cx + x, cy - y, color);
        set_pixel(cx - x, cy - y, color);
        set_pixel(cx + y, cy + x, color);
        set_pixel(cx - y, cy + x, color);
        set_pixel(cx + y, cy - x, color);
        set_pixel(cx - y, cy - x, color);

        x++;

        if (d < 0)
        {
            d += 2 * x + 1;
        }

        else
        {
            y--;
            d += 2 * (x - y) + 1;
        }
    }
}

void blit(int dstx, int dsty, int w, int h, LPDWORD src)
{
    int startX = 0, startY = 0;
    int endX = w, endY = h;

    if (dstx < 0) startX = -dstx;
    if (dsty < 0) startY = -dsty;
    if (dstx + w > SCREEN_WIDTH) endX = SCREEN_WIDTH - dstx;
    if (dsty + h > SCREEN_HEIGHT) endY = SCREEN_HEIGHT - dsty;

    for (int y = startY; y < endY; y++)
    {
        int screenY = dsty + y;

        LPDWORD srcRow = &src[y * w + startX];
        LPDWORD dstRow = &
        (
            (LPDWORD)
            video_memory
        )
        [
            screenY * SCREEN_WIDTH +
            (dstx + startX)
        ];

        int length = endX - startX;

        for
        (
            int i = 0;
            i < length; i++
        ) dstRow[i] = srcRow[i];
    }
}

void StretchBlt
(
    int x1, int y1, int w1, int h1,
    int x2, int y2, int w2, int h2
)
{
    LPDWORD framebuffer = (LPDWORD) get_frame_buff();

    float sx = (float)w1 / w2;
    float sy = (float)h1 / h2;

    for (int dy = 0; dy < h2; dy++)
    {
        for (int dx = 0; dx < w2; dx++)
        {
            int sx_pos = (int)(x1 + dx * sx);
            int sy_pos = (int)(y1 + dy * sy);

            DWORD color =
            framebuffer
            [
                sy_pos * SCREEN_WIDTH
                + sx_pos
            ];

            framebuffer
            [
                (y2 + dy) * SCREEN_WIDTH 
                + (x2 + dx)
            ] = color;
        }
    }
}