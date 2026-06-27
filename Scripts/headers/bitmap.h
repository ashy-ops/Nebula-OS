#pragma once

#include <stdint.h>
#include <stdbool.h>


#define WORD_SIZE uint64_t //in bytes
#define PAGE_SIZE 4096
#define BITS_PER_WORD (sizeof(WORD_SIZE) * 8) //bits per word

typedef struct
{
    WORD_SIZE *map;         // Pointer to bitmap storage
    uint32_t word_cnt;      // Number of 64-bit words allocated for bitmap
    uint32_t total_pages;   // Actual number of physical pages managed
    uint32_t free_page;     //The latest free page position (to prevent it from scanning from the start)
} bitmap_t;

bool bitmap_bit_free(bitmap_t *bmp, uint32_t page_num);


void bitmap_set(bitmap_t *bmp, uint32_t page_num);
void bitmap_unset(bitmap_t *bmp, uint32_t page_num);
//returns the starting page number of the contiguous memory asked for
bool bitmap_set_range(bitmap_t *bmp, uint32_t page_cnt, uint32_t *page_strt);
void bitmap_unset_range(bitmap_t *bmp, uint32_t page_start,uint32_t page_cnt);

//initializes bitmap and reserves space for itself
void bitmap_init(bitmap_t *bmp, uintptr_t base_addr, uint64_t free_ram_bits);
