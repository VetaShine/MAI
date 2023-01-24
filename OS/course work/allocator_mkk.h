#include <stdlib.h>

typedef unsigned char* PBYTE_MKK;

typedef enum memory_structure {
  free_state = 0
} memory_state;

typedef struct block_mkk_structure {
  struct block_mkk_structure* next;
} block_mkk;

static const size_t PAGE_SIZE_MKK = 4096;
static void* heap_mkk = NULL;
static size_t* memory_size_mkk = NULL;
static block_mkk** list_mkk = NULL;
static size_t pages_mkk = 0;
static size_t pow_mkk = 0;
static size_t pow_index_minimum = 0;
static size_t request_mkk = 0;
static size_t total_mkk = 0;

int initialization_mkk(size_t size);
void destroy_mkk();
void* malloc_mkk(size_t size);
void free_mkk(void* address);
block_mkk* alloc_page_mkk(size_t size);
void free_page_mkk(block_mkk* block);
void split_page_mkk(block_mkk* block, size_t powIndex);
size_t pow_of_size_mkk(size_t size);
size_t get_pages_count_mkk(size_t size);
size_t get_page_index_mkk(block_mkk* block);
size_t get_request_mkk();
size_t get_total_mkk();

#endif
