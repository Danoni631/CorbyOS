/*
    CODED BY Danoni631, on 07/13/2026

    This code get GPU capactity and model
*/

#include "include/stdint.h"
#include "ports.h"
#include "pci.h"
#include "graphics.h"

#include "gpu.h"

const char* get_gpu_vendor(uint16_t vendor)
{
    switch (vendor)
    {
        case 0x10DE: return "NVIDIA";
        case 0x1002: return "AMD";
        case 0x8086: return "INTEL";
        default: return "Unknown";
    }
}

const char* get_gpu_name(uint16_t vendor, uint16_t device_id)
{
    if (vendor == 0x10DE)
    {
        return "NVIDIA Video cardboard";
    }

    if (vendor == 0x1002)
    {
        return "AMD Radeon";
    }

    if (vendor == 0x8086)
    {
        return "Intel UHD Graphics";
    }

    return "Unknown GPU";
}

void show_gpu_name()
{
    for (uint8_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++) 
        {
            uint32_t data = pci_config_read_DWORD(bus, slot, 0, 0x00);
            uint16_t vendor_id = data & 0xFFFF;
            uint16_t device_id = (data >> 16) & 0xFFFF;
            
            if (vendor_id != 0xFFFF)
            {
                const CHAR* vendor = get_gpu_vendor(vendor_id);
                const CHAR* name = get_gpu_name
                (
                    vendor_id,
                    device_id
                );

                print("GPU: ", 0xFF00FFFF);
                print(vendor, 0xFFFFFFFF);
                print(" - ", 0xFF00FFFF);
                print(name, 0xFFFFFFFF);
                print("\n", 0x00);
                return;
            }
        }
    }
}

void show_gpu_info()
{
    for (uint8_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t slot = 0; slot < 32; slot++)
        {
            uint32_t data = pci_config_read_DWORD(bus, slot, 0, 0x00);
            uint16_t vendor_id = data & 0xFFFF;
            uint16_t device_id = (data >> 16) & 0xFFFF;

            if (vendor_id != 0xFFFF)
            {
                print("Vendor ID: ", 0xFFFFFFFF);
                printhex(vendor_id, 0xFFFFFFFF);
                print(" (", 0xFFFFFFFF);
                print(get_gpu_vendor(vendor_id), 0xFFFFFFFF);
                print(")\n", 0xFFFFFFFF);

                print("Device ID: ", 0xFFFFFFFF);
                printhex(device_id, 0xFFFFFFFF);
                print(" (", 0xFFFFFFFF);
                print(get_gpu_name(vendor_id, device_id), 0xFFFFFFFF);
                print(")\n", 0xFFFFFFFF);
            }
        }
    }
}