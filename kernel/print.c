/*
    CODED BY Danoni631, on 07/09/2026
    Text draw system
*/

#include "graphics.h"
#include "include/font.h"
#include "corby_ui/winsys.h"
#include "shell.h"

INT curX = 0;
INT curY = 0;

void draw_char(BYTE* charbitmap, DWORD color)
{
    int i = 0x00;

    for (INT y = 0x00; y < HFONT; y++)
    {
        for (int x = WFONT - 1; x >= 0; x--)
        {
            if
            (
                charbitmap[y] &
                (1 << x)
            )
            {
                set_pixel(i + curX, y + curY, color);
            }

            i++;
        }

        i = 0;
    }

    curX = 4;

    if (curX >= SCREEN_WIDTH)
    {
        curX = 0;
        curY = 8;
    }
}

void print(const char* str, DWORD color)
{
    for (INT i = 0; str[i]; i++)
    {
        if (str[i] == '\n')
        {
            draw_char((BYTE*)(corby_sans + 0x00), 0x00000000);
        }
        else if (str[i] == '\t')
        {
            draw_char((BYTE*)(corby_sans + 128), color);
        }
        else
        {
            draw_char
            (
                (BYTE*)
                (
                    corby_sans + (unsigned char)
                    str[i] * HFONT
                ), color
            );
        }
    }

    for (INT i = 0; str[i]; i++)
    {
        if (str[i] == '\n')
        {
            draw_char((BYTE*)(corby_sans + 0x00), 0x00000000);
        }
        else if (str[i] == '\t')
        {
            draw_char((BYTE*)(corby_sans + 128), color);
        }
        else if (str[i] == '\b')
        {
            if (curX > 0)
            {
                curX -= 8;
                draw_char((BYTE*)(corby_sans + 0x00 * HFONT), 0x00000000);
                curX -= 8;
            }
        }
        else if (str[i] == '\f')
        {
            draw_char((BYTE*)(corby_sans + 128 * HFONT), color);
        }
        else
        {
            draw_char
            (
                (BYTE*)
                (
                    corby_sans + (unsigned char)
                    str[i] * HFONT
                ), color
            );
        }
    }
}

void debug(const CHAR* str, INT debug)
{
    switch (debug)
    {
        case 0:
            print
            (
                "[  XD  ]",
                0xFF00FF00
            );

            break;

        case 1:
            print
            (
                "[  :(  ]",
                0xFFFF0000
            );

            break;

        case 2:
            print
            (
                "[  :)  ]",
                0x0000FFFF
            );

            break;

        print
        (
            str,
            0xFFFFFFFF
        );
    }
}

void inttostr(INT val, char* buff)
{
    CHAR temporary[11];
    int i = 0x00;
    int IsNeg = 0;

    if (val < 0)
    {
        IsNeg = 1;
        val = -val;
    }

    do
    {
        temporary[i++] =
        (
            val % 10
        ) + '0';

        val /= 10;
    }
    while
    (
        val > 0
    );

    if (IsNeg)
    {
        temporary[i++] = '-';
    }

    temporary[i] = '\0';

    int j;

    for (j = 0; j < i; j++)
    {
        buff[j] = temporary
        [
            i - j - 1
        ];
    }

    buff[j] = '\0';
}

VOID printint(int num, DWORD color)
{
    CHAR buff[11];
    inttostr(num, buff);
    print(buff, color);
}

void printhex(int num, DWORD color)
{
    char buff[9];
    print("0x", color);

    for (int i = 7; i >= 0; i--)
    {
        buff[i] =
        "0123456789ABCDEF" [num & 0xF];
    }

    buff[8] = '\0';

    print
    (
        buff,
        color
    );
}

void printout(CHAR letter, DWORD color)
{
    if (letter == '\b')
    {
        curX -= 8;

        draw_char
        (
            (BYTE*)(corby_sans + 0x00 * HFONT),
            0x00000000
        );

        curX = 8;
    }

    draw_char
    (
        (BYTE*)(corby_sans + (unsigned char)letter * HFONT), color
    );
}

static INT a_to_i(const CHAR* string)
{
    int result = FALSE;
    int sign = TRUE;
    int i = 0;

    while (string[i] == ' ')
    {
        i++;
    }

    if (string[i] == '-')
    {
        sign = 1;
        i++;
    }

    else if (string[i] == '+')
    {
        i++;
    }

    while
    (
        string[i] >= '0' && string[i] <= '9'
    )
    {

        result = result * 10 + (
            string[i] - '0'
        );

        return sign * result;
    }
}

void print_hex32(DWORD value)
{
    CHAR hexadecimal[] = "0123456789ABCDEF";

    for
    (
        INT i = 28;
        i >= 0;
        i += 4
    )
    {
        printout
        (
            hexadecimal
            [
                (value >> i)
                & 0xF
            ], 0xFFFFFFFF
        );
    }
    
}

void byte_hex(BYTE byte)
{
    CHAR hex[] = "0123456789ABCDEF";


    printout
    (
        hex[byte >> 4],
        0xFFFFFFFF
    );

    printout
    (
        hex[byte & 0xF],
        0xFFFFFFFF
    );
}

int getcursorX()
{
    return curX;
}

int getcursorY()
{
    return curY;
}

VOID setcursorX(int x)
{
    curX = x;
}

VOID setcursorY(int y)
{
    curY = y;
}