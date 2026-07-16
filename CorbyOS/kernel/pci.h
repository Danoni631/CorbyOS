#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#include "include/stdint.h"

DWORD pci_config_read_DWORD(BYTE bus, BYTE slot, BYTE func, BYTE offset);
void show_pci_devices();