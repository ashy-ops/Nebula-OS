#include "bitmap.h"
#include "display.h"

//few things to note, when you declare a value as a pointer, type *x
//then just x represents the memory address of x

inline bool bitmap_bit_free(BITMAP *bmp, uint32_t bit)
{
    uint32_t word = bit / BPW;
    uint32_t offset = bit % BPW;

    return !((bmp->map[word] >> offset) & 1ULL); 
    //grabbing the desired bit and bringing it to the 0th index or you can bring 1 th the offset index and do &
}

void bitmap_set(BITMAP *bmp, uint32_t bit)
{
    uint32_t word = bit / BPW;
    uint32_t offset = bit % BPW;

    bmp->map[word] |= (1ULL << offset);
}

//returns the starting page number of the contiguous memory asked for 
bool bitmap_set_range(BITMAP *bmp, uint32_t page_cnt, uint32_t *page_strt)
{
    uint32_t tot_pages = bmp->word_cnt * BPW;

    uint32_t strt = 0;
    uint32_t len = 0;

    for (uint32_t i = 0; i < tot_pages; i++)
    {
        if (bitmap_bit_free(bmp, i))
        {
            if (len == 0)
                strt = i;

            len++;

            if (len == page_cnt)
            {
                for (uint32_t j = 0; j < page_cnt; j++)
                {
                    bitmap_set(bmp, strt + j);
                }

                *page_strt = strt;
                return true;
            }
        }
        else
        {
            len = 0;
        }
    }

    return false; 
    //this gets treated as max value 0xFFFFFFFF as unsigned
}

void bitmap_init(BITMAP *bmp, WORD_SIZE base_addr, WORD_SIZE free_ram_bits)
{
    bmp->map = (WORD_SIZE *)base_addr;
    bmp->word_cnt = free_ram_bits / BPW; 

    write_to_terminal("Base Address of BITMAP: %llx",WHITE,base_addr);
    write_to_terminal("Base Address of BITMAP: %llx",WHITE,bmp->map);

    for (uint32_t i = 0; i < bmp->word_cnt; i++)
    {
        bmp->map[i] = 0;
    }

    //no of bits occupied by the bitmap itself, if there n pages in ram, we need n bits to represent them!
    //hence we do pages/8 = xbytes
    //in my implementation each word is 64 bits each representing a page hence total pages is word_cnt*BPW, 
    //hence I need to reserve total/8byte = res bytes

    // Total pages represented = word_cnt * BPW
    // Bitmap size in bytes = (word_cnt * BPW) / 8
    // Since sizeof(WORD_SIZE) == BPW / 8,
    // bitmap_bytes = word_cnt * sizeof(WORD_SIZE)

    uint32_t bitmap_bytes = bmp->word_cnt * sizeof(WORD_SIZE);
    uint32_t bitmap_pages = (bitmap_bytes + PAGE_SIZE - 1) / PAGE_SIZE;

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