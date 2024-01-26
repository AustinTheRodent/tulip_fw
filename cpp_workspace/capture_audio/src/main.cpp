#include <pthread.h>
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <tgmath.h>
#include <unistd.h>
#include "KR260_TULIP_REGISTERS.h"

#define TULIP_ADDRESS 0x80010000

static volatile bool keep_running = true;

static void* userInput_thread(void*)
{
  while(keep_running)
  {
    if (std::cin.get())
    //if (std::cin.get() == 'q')
    {
      keep_running = false;
    }
  }
  return 0;
}

int main()
{
  void *virt_addr;
  uint64_t offset = 0;
  int pagesize;
  uint64_t page_base;
  uint64_t page_offset;
  int offset_in_page;
  uint32_t read_result;

  void *map_base;
  int mem_fd;

  pthread_t tId;

  pagesize = sysconf(_SC_PAGE_SIZE);
  offset = TULIP_ADDRESS;
  page_base = (offset / pagesize) * pagesize;
  page_offset = offset - page_base;

  if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) {
    perror("Error opening /dev/mem");
    return EXIT_FAILURE;
  }

  map_base = mmap(NULL,
    pagesize,
    PROT_READ | PROT_WRITE,
    MAP_SHARED,
    mem_fd,
    page_base);	// phys_addr should be page-aligned.	

  if (map_base == MAP_FAILED) {
    perror("Error mapping memory");
    close(mem_fd);
    return EXIT_FAILURE;
  }

  virt_addr = (char *)map_base + CONTROL;
  read_result = *(volatile uint32_t*)virt_addr;

  if ( ((read_result & CONTROL_SW_RESETN) == 0) || ((read_result & CONTROL_I2S_ENABLE) == 0) )
  {
    printf("Tulip I2S is not enabled, did you initialize the codec?\n");
    munmap((void *)map_base, pagesize);
    close(mem_fd);
    return 0;
  }

  (void) pthread_create(&tId, 0, userInput_thread, 0);

  while (keep_running)
  {
    //printf("woah\n");
    // this will run until you press "enter"
  }
  printf("exiting\n");

  (void) pthread_join(tId, NULL);

  munmap((void *)map_base, pagesize);
  close(mem_fd);

  return 0;
}