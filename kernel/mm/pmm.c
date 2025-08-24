#include <stdint.h>
#include <stddef.h>
#include <pmm.h>

#define PAGE_SIZE 4096
#define MAX_PAGES 32768

static uint8_t pmm_bitmap[MAX_PAGES / 8];
static uint32_t total_pages = 0;

void pmm_init(uint32_t mem_size) {
    total_pages = mem_size / PAGE_SIZE;
    for (uint32_t i = 0; i < MAX_PAGES / 8; i++) pmm_bitmap[i] = 0;
}

void* pmm_alloc(void) {
    for (uint32_t i = 0; i < total_pages; i++) {
        uint32_t byte = i / 8, bit = i % 8;
        if (!(pmm_bitmap[byte] & (1 << bit))) {
            pmm_bitmap[byte] |= (1 << bit);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return NULL;
}

void pmm_free(void* addr) {
    uint32_t i = (uint32_t)addr / PAGE_SIZE;
    uint32_t byte = i / 8, bit = i % 8;
    pmm_bitmap[byte] &= ~(1 << bit);
}
