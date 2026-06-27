#include "bitmap.h"
#include "display.h"

//few things to note, when you declare a value as a pointer, type *x
//then just x represents the memory address of x

inline bool bitmap_bit_free(bitmap_t *bmp, uint32_t bit)
{
    uint32_t word = bit / BITS_PER_WORD;
    uint32_t offset = bit % BITS_PER_WORD;

    return !((bmp->map[word] >> offset) & 1ULL);
    //grabbing the desired bit and bringing it to the 0th index or you can bring 1 th the offset index and do &
}

void bitmap_set(bitmap_t *bmp, uint32_t page_num)
{
    uint32_t word = page_num / BITS_PER_WORD;
    uint32_t offset = page_num % BITS_PER_WORD;
    bmp->map[word] |= (1ULL << offset);
}

void bitmap_unset(bitmap_t *bmp, uint32_t page_num)
{
  uint32_t word = page_num / BITS_PER_WORD;
  uint32_t offset = page_num % BITS_PER_WORD;
  bmp->map[word] &= ~(1ULL << offset);
}

//returns the starting page number of the contiguous memory asked for
bool bitmap_set_range(bitmap_t *bmp, uint32_t page_cnt, uint32_t *page_strt)
{

    uint32_t strt = 0;
    uint32_t len = 0;

    for (uint32_t i = bmp->free_page; i < bmp->total_pages; i++)
    {
        if (bitmap_bit_free(bmp, i))
        {
            if (len == 0) strt = i;

            len++;
            if (len == page_cnt)
            {
                for (uint32_t j = 0; j < page_cnt; j++) bitmap_set(bmp, strt + j);

                *page_strt = strt;
                bmp->free_page = strt+page_cnt;
                return true;
            }
        }
        else len = 0;
    }

    return false; 
}

void bitmap_unset_range(bitmap_t *bmp, uint32_t page_start,uint32_t page_cnt)
{
  for(uint32_t i = 0;i<page_cnt;i++) bitmap_unset(bmp,page_start+i);
  if(page_start<bmp->free_page) bmp->free_page = page_start;
}


void bitmap_init(bitmap_t *bmp, uintptr_t base_addr, uint64_t free_ram_bits)
{
    bmp->free_page = 0;
    bmp->map = (WORD_SIZE *)base_addr;
    uint64_t ram_in_bytes = free_ram_bits/8;
    uint32_t tot_pages = ram_in_bytes/PAGE_SIZE;
    bmp->word_cnt = (tot_pages+BITS_PER_WORD-1)/BITS_PER_WORD;
    bmp->total_pages = tot_pages;
    uint32_t bitmap_bytes  = (tot_pages+7)/8;
    uint32_t bitmap_pages = (bitmap_bytes + PAGE_SIZE - 1) / PAGE_SIZE;

    
    write_to_terminal("Base Address of BITMAP: %llx",WHITE,base_addr);
    write_to_terminal("Word Count of BITMAP: %u",WHITE,bmp->word_cnt);
    write_to_terminal("Total Pages Available: %u",WHITE,bmp->total_pages);

    //memset to 0
    for (uint32_t i = 0; i < bmp->word_cnt; i++) bmp->map[i] = 0;
    
    write_to_terminal("RESERVED PAGES: %u",WHITE,bitmap_pages);


    //reserving pages for the bitmap itself!
    uint32_t strt_page;
    if (bitmap_set_range(bmp, bitmap_pages, &strt_page))
    {
        write_to_terminal("HEHE",WHITE);
        write_to_terminal("Allocation of Bitmap Successfull!", WHITE);
    }
    else
    {
        write_to_terminal("Allocation of Bitmap Failed!", WHITE);
        write_to_terminal("Not enough memory to store the BITMAP!", WHITE);
    }
}