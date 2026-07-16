#include "../include/stdint.h"
#include "../graphics.h"
#include "../include/math.h"

#define WAVE_AMPLITUDE 3
#define WAVE_FREQUENCY 10
#define SCROLL_SPEED 20

void _start()
{
    const CHAR* string = "String program";
    INT msglenght = 13;
    int intial_x = SCREEN_WIDTH;

    while (intial_x > -(27 * 8 * 3))
    {
        clsscreen();

        for (int i = 0; i < msglenght; i++)
        {
            INT charX = intial_x + (i * 8);
            INT y_offset = (INT)
            (
                WAVE_AMPLITUDE *
                sin
                (
                    (charX + SCREEN_WIDTH)
                    / WAVE_FREQUENCY
                )
            );

            setcursorX(charX);
            setcursorY((SCREEN_HEIGHT / 2) + y_offset);
            printout(string[i], 0x20 + i);
        }

        intial_x -= SCROLL_SPEED;
        for (volatile INT i = 0; i < 100000; i++);
    }
}