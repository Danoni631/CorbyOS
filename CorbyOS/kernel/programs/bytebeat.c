#include "../include/stdint.h"
#include "../ports.h"
#include "../audio.h"

void _start()
{
    static BYTE test[64000];

    for (INT t = 0; t < 64000; t++)
    {
        test[t] = t *
        (   
            (t * (t & 16384 ? 6 : 5) / 4 | t >> 4) *
            (1 + (3 & t >> 11)) >> (3 & t >> 9) |
            t * (t & 16384 ? 6 : 5) *
            (3 + (3 & t >> (t & 2048 ? 334 : 14))) >> 1 +
            (3 & t >> 8) & (t * t | t >> 4 | t >> 3)
        );
    }

    play_bytebeat(test, sizeof(test));
}