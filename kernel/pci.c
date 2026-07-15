/*
    CODED BY Danoni631, on 07/13/2026 (1st day after my cirurgy)

    The PCI (Peripheral Component Interconnect) is kinda of hardware bus
    to conect peripheral devices, like Video boards, sound boards and
    network boards. In this code we read the PCI and shows
    the devices connected.
*/

#include "ports.h"
#include "graphics.h"

#include "pci.h"

DWORD pci_config_read_DWORD(BYTE bus, BYTE slot, BYTE func, BYTE offset)
{
    DWORD Addr;
    DWORD lbus = (DWORD)bus;
    DWORD lslot = (DWORD)slot;
    DWORD lfunc = (DWORD)func;
    DWORD temp = 0;

    Addr =
    (DWORD)
    (
        (lbus << 16) | (lslot << 11) | (lfunc << 8) |
        (offset & 0xfc) | ((DWORD)0x80000000)
    );

    outl(PCI_CONFIG_ADDRESS, Addr);
    temp = inl(PCI_CONFIG_DATA);

    return temp;
}

void show_pci_devices()
{
    for (BYTE device = 0x00; device < 32; device++)
    {
        for (BYTE func = 0x00; func < 8; func++)
        {
            DWORD data = pci_config_read_DWORD(0, device, func, 0);
            WORD vendor_id = (uint16_t)(data & 0xFFFF);
            DWORD device_id = (uint16_t)(data >> 16);

            if (vendor_id != 0xFFFF)
            {
                debug("PCI Device found at", 2);
                printint(device, 0xFFFFFFFF);
                print(", ", 0xFFFFFFFF);
                printint(func, 0xFFFFFFFF);
                print(": Vendor ID = ", 0xFFFFFFFF);
                printhex(vendor_id, 0xFFFFFFFF);
                print(", Device ID = ", 0xFFFFFFFF);
                printhex(device_id, 0xFFFFFFFF);
                print("\n", 0x0F);
            }
        }
    }
}