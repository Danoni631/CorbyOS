/*
    CODED BY mell-o-tron and ArTic
    (mell-o-tron on MellOS on 12/20/2020)
    (ArTic on NovaOS on 01/12/2025)
*/

#include "ports.h"
#include "alloc.h"
#include "graphics.h"

#include "drive.h"

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF  0x20
#define STATUS_ERR 0x01

static inline VOID ata_delay_400ns(void) 
{
    inb(0x3F6);
    inb(0x3F6);
    inb(0x3F6);
    inb(0x3F6);
}

void wait_BSY()
{
	while(inb(0x1F7) & STATUS_BSY){};
}

void wait_DRQ()
{
	while(!(inb(0x1F7) & STATUS_RDY)){};
}

BYTE check_ERR()
{
	return ((inb(0x1F7) & STATUS_ERR) != 0);
}

LPWORD identify_ata(BYTE drive)
{
	outb(0x1F6, drive);

	ata_delay_400ns();

	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);

	outb(0x1F7, 0xEC);

	ata_delay_400ns();
	
	BYTE status = inb(0x1F7);

	if (status == 0)
    {
		debug("DX: This drive don't exist\n", 0x01);
        return 0;
	}

	if (status == 0xFF)
    {
		debug("ERROR: Floating bus (Wheres the connected drives?)\n", 0x01);
		return 0;
	}

	wait_BSY();

	if((inb(0x1F4) | inb(0x1F5)) != 0)
    {
		debug("ERROR: This drive is not ATA\n", 0x01);
		return 0;
	}

	wait_DRQ();

	if (check_ERR())
    {
		debug("ATA Identify Error :(\n", 0x01);
		return 0;
	}

	LPWORD res = alloc_memory(sizeof(LPWORD) * 256);

	for (int i = 0; i < 256; i++)
    {
		res[i] = inw(0x1F0);
	}

	return res;
}

void ata_print_error(BYTE error) 
{
    debug("Error details: ", 0x01);

    if(error & 0x01) print("AMNF ", 0xFFFFFFFF);  // Address Mark Not Found
    if(error & 0x02) print("TK0NF ", 0xFFFFFFFF); // Track 0 Not Found
    if(error & 0x04) print("ABRT ", 0xFFFFFFFF);  // Aborted Command
    if(error & 0x08) print("MCR ", 0xFFFFFFFF);   // Media Change Request
    if(error & 0x10) print("IDNF ", 0xFFFFFFFF);  // ID Not Found
    if(error & 0x20) print("MC ", 0xFFFFFFFF);    // Media Changed
    if(error & 0x40) print("UNC ", 0xFFFFFFFF);   // Uncorrectable Data
    if(error & 0x80) print("BBK ", 0xFFFFFFFF);   // Bad Block

    print("\n", 0x00);
}

void check_ata_error(void) 
{
    BYTE status = inb(0x1F7);

    if (status & 0x01) 
    {
        BYTE error = inb(0x1F1);
        debug("ATA command error: status = ", 0x02);
        printhex(status, 0xFFFFFFFF);
        print(", error = ", 0xFFFFFFFF);
        printhex(error, 0xFFFFFFFF);
        print("\n", 0x00);
        ata_print_error(error);
    }
}

void read_sector(BYTE drive, DWORD lba, DWORD num_sectors, LPWORD address)
{
	identify_ata(drive);

	lba = lba & 0x0FFFFFFF;

    wait_BSY();

    outb(0x1F6, drive | ((lba >> 24) & 0xF));

	ata_delay_400ns();

	outb(0x1F1, 0x00);
    outb(0x1F2, num_sectors);
    outb(0x1F3, (BYTE) lba);
    outb(0x1F4, (BYTE)(lba >> 8));
	outb(0x1F5, (BYTE)(lba >> 16)); 
	outb(0x1F7, 0x20);

	ata_delay_400ns();

	LPWORD tmp = address;
	
    for (int j = 0; j < num_sectors; j ++)
    {
		wait_BSY();
		wait_DRQ();
		
		for(int i = 0; i < 256; i++)
        {
            tmp[i] = inw(0x1F0);
        }

		tmp += 256;
	}

	check_ata_error();
}


void write_sector(BYTE drive, DWORD lba, DWORD num_sectors, LPWORD buffer)
{
	identify_ata(drive);

	lba = lba & 0x0FFFFFFF;
	
	wait_BSY();

	outb(0x1F6, drive | ((lba >> 24) & 0xF));		// send drive and bits 24 - 27 of LBA

	ata_delay_400ns();

	outb(0x1F1, 0x00);								// ?
	outb(0x1F2, num_sectors);							// send number of sectors
	outb(0x1F3, (BYTE) lba);						// send bits 0-7 of LBA
	outb(0x1F4, (BYTE) (lba >> 8));				// 8-15
	outb(0x1F5, (BYTE) (lba >> 16)); 			// 16-23
	outb(0x1F7,0x30); 								// 0x30 = 'Write' Command

	ata_delay_400ns();

	LPWORD tmp = buffer;
	
	for (int j = 0; j < num_sectors; j++)
    {
		wait_BSY();
		wait_DRQ();
		
		for(int i = 0; i < 256; i++)
        {
			outw(0x1F0, tmp[i]);
		}

		tmp += 256;
	}

	check_ata_error();
}