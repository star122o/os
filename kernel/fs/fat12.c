#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include <fat12.h>

struct fat12_bpb {
    uint8_t  jmp[3];
    char     oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_short;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat;
};

static struct fat12_bpb bpb;
static uint8_t fat_table[4096];
static uint8_t root_dir[2048];

extern int read_sector(uint32_t lba, uint8_t* buffer);

int fat12_mount(uint8_t* boot_sector) {
    memcpy(&bpb, boot_sector, sizeof(struct fat12_bpb));
    read_sector(bpb.reserved_sectors, fat_table);
    uint32_t root_lba = bpb.reserved_sectors + bpb.num_fats * bpb.sectors_per_fat;
    read_sector(root_lba, root_dir);
    return 0;
}

int fat12_list_root(struct fat12_dir_entry* entries, int max_entries) {
    int count = 0;
    for (int i = 0; i < bpb.root_entries && count < max_entries; i++) {
        uint8_t* entry = root_dir + i * 32;
        if (entry[0] == 0x00 || entry[0] == 0xE5) continue;
        struct fat12_dir_entry* e = &entries[count++];
        memcpy(e->name, entry, 11);
        e->name[11] = 0;
        e->attr = entry[11];
        e->first_cluster = entry[26] | (entry[27] << 8);
        e->size = entry[28] | (entry[29] << 8) | (entry[30] << 16) | (entry[31] << 24);
    }
    return count;
}

int fat12_read_file(const char* filename, uint8_t* buffer, uint32_t max_size) {
    for (int i = 0; i < bpb.root_entries; i++) {
        uint8_t* entry = root_dir + i * 32;
        if (entry[0] == 0x00 || entry[0] == 0xE5) continue;
        char name[12];
        memcpy(name, entry, 11); name[11] = 0;
        if (strncmp(name, filename, 11) == 0) {
            uint16_t cluster = entry[26] | (entry[27] << 8);
            uint32_t size = entry[28] | (entry[29] << 8) | (entry[30] << 16) | (entry[31] << 24);
            uint32_t bytes_read = 0;
            while (cluster < 0xFF8 && bytes_read < size && bytes_read < max_size) {
                uint32_t data_lba = bpb.reserved_sectors + bpb.num_fats * bpb.sectors_per_fat + (bpb.root_entries * 32 / bpb.bytes_per_sector) + (cluster - 2) * bpb.sectors_per_cluster;
                read_sector(data_lba, buffer + bytes_read);
                bytes_read += bpb.bytes_per_sector * bpb.sectors_per_cluster;
                uint16_t fat_offset = cluster + cluster/2;
                if (cluster & 1)
                    cluster = (fat_table[fat_offset] >> 4) | (fat_table[fat_offset+1] << 4);
                else
                    cluster = fat_table[fat_offset] | ((fat_table[fat_offset+1] & 0x0F) << 8);
                cluster &= 0xFFF;
            }
            return bytes_read;
        }
    }
    return -1;
}