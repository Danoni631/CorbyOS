#include "include/stdint.h"

void reset_sb();
void write_dsp(BYTE cmd);
void soundblaster_handler();
void soundblaster_init();
void soundblaster_disable();
void setupdma(LPBYTE buffer, WORD size);
void soundblaster_play(LPBYTE buffer, WORD size);
void setup_sb();
VOID play_bytebeat(LPBYTE buffer, WORD size);