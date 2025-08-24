#include <stdint.h>
#include <paging.h>

#define PAGE_SIZE 4096
#define PAGE_TABLE_ENTRIES 1024

static uint32_t page_directory[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
static uint32_t page_tables[PAGE_TABLE_ENTRIES][PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

void paging_init(uint32_t mem_size) {
    (void)mem_size;
    for (uint32_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        page_directory[i] = ((uint32_t)page_tables[i]) | 3;
        for (uint32_t j = 0; j < PAGE_TABLE_ENTRIES; j++) {
            page_tables[i][j] = (i * PAGE_TABLE_ENTRIES * PAGE_SIZE + j * PAGE_SIZE) | 3;
        }
    }
    __asm__ volatile("mov %0, %%cr3" : : "r"(page_directory));
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}

void paging_map(uint32_t virt, uint32_t phys) {
    uint32_t pd_idx = virt >> 22;
    uint32_t pt_idx = (virt >> 12) & 0x3FF;
    page_tables[pd_idx][pt_idx] = (phys & 0xFFFFF000) | 3;
}
