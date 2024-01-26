#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

//#define BASE_ADDRESS 0x80010000  // Replace with your actual base address
#define PAGE_SZ 4096

#define MAX_RW 16

int main(int argc, char *argv[]) {
  //if (argc < 3) {
  //  printf("Usage: %s <phys_addr> <len>\n", argv[0]);
  //  return 0;
  //}

  int i;

  uint8_t rw_counter = 0;
  bool is_read_array[MAX_RW];
  uint64_t offset = 0;// = strtoul(argv[1], NULL, 0);
  bool offset_defined = false;
  uint32_t write_data_array[MAX_RW];

  int pagesize = sysconf(_SC_PAGE_SIZE);
  uint64_t page_base;
  uint64_t page_offset;
  int offset_in_page;
  uint32_t write_value;

  int mem_fd;

  void *map_base;
  void *virt_addr;

  for( i = 0 ; i < argc ; i++ )
  {
    if( strcmp("-o", argv[i]) == 0 )
    {
      offset = strtoul(argv[i+1], NULL, 0);
      offset_defined = true;
    }
    if( strcmp("-r", argv[i]) == 0 )
    {
      is_read_array[rw_counter] = 1;
      rw_counter++;
    }
    if( strcmp("-w", argv[i]) == 0 )
    {
      write_data_array[rw_counter] = (uint32_t)strtoul(argv[i+1], NULL, 0);
      rw_counter++;
    }
  }

  if( offset_defined == false )
  {
    printf("offset undefined, use \"-o\" switch");
  }

  //bool is_read;
  //bool is_write;
  

  page_base = (offset / pagesize) * pagesize;
  page_offset = offset - page_base;

  if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
      perror("Error opening /dev/mem");
      return EXIT_FAILURE;
  }

  map_base = mmap(NULL,
        PAGE_SZ,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        mem_fd,
        page_base);	// phys_addr should be page-aligned.	

  if (map_base == MAP_FAILED) {
      perror("Error mapping memory");
      close(mem_fd);
      return EXIT_FAILURE;
  }

  offset_in_page = page_offset;
  virt_addr = (char *)map_base + offset_in_page;

  
  for( i = 0 ; i < rw_counter ; i++ )
  {
    if (is_read_array[i])
    {
      int read_result = *(volatile uint32_t*)virt_addr;
      printf("0x%08X\n", read_result);
    }
    else
    {	// write
      write_value = write_data_array[i];
      *(volatile uint32_t*)virt_addr = write_value;
    }
  }

  munmap((void *)map_base, PAGE_SZ);
  close(mem_fd);

  return 0;

}
