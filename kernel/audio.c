/*
    CODED BY Danoni631, on 07/14/2026
    This OS gonna emulate Sound Blast 16 audio driver
*/

#include "ports.h"
#include "graphics.h"

#include "audio.h"

INT Playing;

void reset_sb()
{
    outb(0x226, 0x01);
    outb(0x226, 0x00);
}

void write_dsp(BYTE cmd)
{
    while
    (
        (
            inb(0x22C)
            & 0x80
        ) != 0x00
    );

    outb(0x22C, cmd);
}

void sound_blaster_handler()
{
    inb(0x22E);
    outb(0x20, 0x20);
    Playing = 0x00;
}

void sound_blaster_init()
{
    write_dsp(0xD1);
}

void sound_blaster_disable()
{
    write_dsp(0xD3);
}

void setupdma(LPBYTE buffer, WORD size)
{
    QWORD addr;
    addr = (ULONG)buffer;

    outb(0x0A, 0x05);
    outb(0x0C, 0x00);
    outb(0x0B, 0x49);
    outb(0x02, addr & 0xFF);
    outb(0x02, (addr >> 8) & 0xFF);
    outb(0x83, (addr >> 16) & 0xFF);
    outb(0x03, (size - 1) & 0xFF);
    outb(0x03, (size - 1) >> 8);
    outb(0x0A, 0x01);
}

void soundblaster_play(LPBYTE buffer, WORD size)
{
    Playing = 0x01;
    setupdma(buffer, size);
    write_dsp(0x40);
    write_dsp(256 - (1000000 / 8000));
    write_dsp(0x14);
    write_dsp((size - 1) & 0xFF);
    write_dsp((size - 1) >> 8);
    for (INT i = 0; i < 10000000; i++);
    Playing = 0x00;
}

void setup_sb()
{
    reset_sb();
    sound_blaster_init();
}

VOID play_bytebeat(LPBYTE buffer, WORD size)
{
    soundblaster_play(buffer, size);
    while (Playing);
    sound_blaster_disable();
}
