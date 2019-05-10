#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>
#include "../drivers/isr.h"

#define FRAME_SIZE              4096

#define PAGE_TABLE_SIZE         1024

#define PAGE_DIRECTORY_SIZE     1024

#define PAGE_NOT_PRESENT        0
#define PAGE_PRESENT            1 
#define PAGE_READ_ONLY          0
#define PAGE_READ_WRITE         1
#define PAGE_USER               0
#define PAGE_SUPERVISOR         0
#define PAGE_SIZE_4KB           0
#define PAGE_SIZE_4MB           1

// Assume physical memory is 16 MB 
#define SIZE_OF_PHYSICAL_MEMORY 0x10000000

struct page {
  /* Format of a 32-bit page table entry that maps a 4 KB page
   * (Intel Manual pg. 104)
   * | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9..11 | 12 .. 31 |
   * | P |R/W|U/S|PWT|PCD| A | D |PAT| G | IGN   |  FRAME   |
   *  P: Present. Must be 1 to map a 4-KB page
   *  R/W: Read/Write. (See Intel Manual 4.6)
   *  U/S: User/supervisor. If 0, user-mode accesses are not
           allowed to the 4 KB page referenced by this entry
   *  PWT: Page-level write-through (See Intel Manual 4.9)
   *  PCD: Page-level cache-disable (See Intel Manual 4.9)
   *  A: Accessed. Indicates whether the software has accessed
   *     the 4 KB page referenced by this entity.
   *  G: Global. Determines whether translation is global
   *     (See Intel Manual 4.10)
   *  IGN: Ignored
   *  FRAME: Physical address of 4KB frame referenced by 
   *           this entry. (shifted right 12 bits)
   */ 
  uint8_t present:1;     
  uint8_t rw:1;          
  uint8_t us:1;          
  uint8_t pwt:1;
  uint8_t pcd:1; 
  uint8_t a:1;     
  uint8_t d:1; 
  uint8_t pat:1;    
  uint8_t g:1;
  uint8_t ignored2:3; 
  uint32_t frame:20;
} __attribute__((packed));
typedef struct page page_t;

typedef struct page_table {
  page_t pages[PAGE_TABLE_SIZE];
} page_table_t;

/* For every page directory, we keep 2 arrays. One 
 * is holding the physical addresses of its page tables 
 * (for giving to the CPU), and the other holding the virtual
 * addresses (so we can read/write to them). 
*/
typedef struct page_directory{
  //Array of pointers to page tables.
  page_table_t *page_tables_virtual[PAGE_DIRECTORY_SIZE];
  
  /*
   * Array of pointers to the pagetables above, but gives their *physical*
   * location, for loading into the CR3 register.
   */
  uint32_t page_tables_physical[PAGE_DIRECTORY_SIZE];

} page_directory_t;

/* Sets up the environment, page directories etc and
 * enables paging.
 */
void init_paging();

/* Causes the specified page directory to be loaded 
 * into the CR3 register, where the MMU expects it.
 * Enables paging and flushes the page-directory cache.
 */ 
void enable_paging(page_directory_t *page);

/* Retrieves a pointer to the page required.
 * If make == 1, if the page-table in which this page should
 * reside isn't created, create it!
 */
page_t *get_page(uint32_t address, int make, page_directory_t *dir);

/* Indentity map (physical address = virtual address)
 * from 0x0 to the end of used memory, so we can 
 * access this transparently, as if paging wasn't enabled.
 * Note: we allocate and extra frame so the heap
 *       can be properly initialized.
 */
void identity_map();

void free_frame(page_t *page);

void alloc_frame(page_t *page, int is_supervisor, int is_writeable);
/*
 * Handler for page faults.
 */
void page_fault(registers_t regs);
#endif
