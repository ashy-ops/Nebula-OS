#include <stdint.h>
#include "pic.h"
#include "idt.h"
#include "display.h"
#include "bitmap.h"
#include "debug.h"
#include "handler_init.h"

#define ENTRY_COUNT 0x90000
#define MEMORMY_MAP 0x90010


volatile char* VGA_BUFFER = (volatile char*)0xB8000;

typedef struct 
{
  uint64_t max_free_memory;
  uint64_t max_base_addr;
} memory_region_t;

typedef struct __attribute__((packed)) {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t acpi; // ACPI 3.0 extended attributes
} memory_map_entry_t;
//uint8_t count = *(uint8_t*)ENTRY_COUNT; Throws error

typedef struct
{
  uint32_t start_page;
  uint32_t pages_allocated;
} page_allocation_t;

void kernel_main()
{

  idt_initialize(); 
  pic_remap();  
  INIT_HANDLERS();
  CLEAR_SCREEN();


  memory_region_t b = {0,0};
  uint8_t count = *(uint8_t*)ENTRY_COUNT;
  memory_map_entry_t* mem = (memory_map_entry_t*)MEMORMY_MAP;

  //write_to_terminal("Entry Count is:%i",WHITE,count);
  for(int i=0;i<count;i++)
  {

    uint64_t ba = mem[i].base_address;
    uint64_t len = mem[i].length;
    uint32_t type = mem[i].type;
    uint32_t acpi = mem[i].acpi;

    if(type==1 && b.max_free_memory<len)
    {
      b.max_free_memory = len;
      b.max_base_addr = ba;
    }

    //from the output notice  the large chunnk of memory starting from the 1MB mark :)
    //write_to_terminal("Base:%llx, Length:%llu, Type:%u,ACPI:%u",WHITE,ba,len,type,acpi);
  }

  bitmap_t bmp;
  bitmap_init(&bmp,b.max_base_addr,b.max_free_memory);
/*
  page_allocation_t x;
  
  if(bitmap_set_range(&bmp,20,&x.start_page))
  {
    x.pages_allocated = 20;
    write_to_terminal("Allocated at:%u and len:%u",WHITE,x.start_page,x.pages_allocated);
  }
  else write_to_terminal("Allocation Failed!",WHITE);

  bitmap_unset_range(&bmp,x.start_page,x.pages_allocated);

  page_allocation_t y;
  if(bitmap_set_range(&bmp,20,&y.start_page))
  {
    y.pages_allocated = 20;
    write_to_terminal("Allocated at:%u and len:%u",WHITE,y.start_page,y.pages_allocated);
  }
  else write_to_terminal("Allocation Failed!",WHITE);
*/
  while(1)
  {
    
  }
}