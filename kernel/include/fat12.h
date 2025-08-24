#ifndef FAT12_H
#define FAT12_H
#include <stdint.h>

#define FAT12_SECTOR_SIZE 512
#define FAT12_MAX_FILENAME 12

struct fat12_dir_entry {
    char name[FAT12_MAX_FILENAME];
    uint8_t attr;
    uint16_t first_cluster;
    uint32_t size;
};

int fat12_mount(uint8_t* boot_sector);
int fat12_list_root(struct fat12_dir_entry* entries, int max_entries);
int fat12_read_file(const char* filename, uint8_t* buffer, uint32_t max_size);

#endif
