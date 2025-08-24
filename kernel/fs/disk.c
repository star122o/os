#include <stdint.h>
#include <stdbool.h>
#include <io.h>

#define ATA_PRIMARY_IO_BASE 0x1F0
#define ATA_PRIMARY_CONTROL_BASE 0x3F6
#define ATA_SECTOR_SIZE 512

void ata_wait_ready() {
    while (inb(ATA_PRIMARY_IO_BASE + 7) & 0x80);
}

int read_sector(uint32_t lba, uint8_t *buffer) {
    ata_wait_ready();

    outb(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO_BASE + 2, 1);
    outb(ATA_PRIMARY_IO_BASE + 3, lba & 0xFF);
    outb(ATA_PRIMARY_IO_BASE + 4, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO_BASE + 5, (lba >> 16) & 0xFF);

    outb(ATA_PRIMARY_IO_BASE + 7, 0x20);

    ata_wait_ready();

    for (int i = 0; i < ATA_SECTOR_SIZE / 2; i++) {
        uint16_t data = inw(ATA_PRIMARY_IO_BASE);
        buffer[i * 2] = data & 0xFF;
        buffer[i * 2 + 1] = (data >> 8) & 0xFF;
    }

    return 0;
}
