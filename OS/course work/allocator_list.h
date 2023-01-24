#ifndef ALLOCATOR_LIST_H
#define ALLOCATOR_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char* PBYTE_LIST;

typedef struct block_list {
  size_t size;
  struct block_list* previous;
  struct block_list* next;
} block_list;

static block_list* begin_list;
static block_list* free_list;
static size_t size_list;
static size_t request_list = 0;
static size_t total_list = 0;

int initialization_list(size_t size);
void destroy_list();
void* alloc_block_list(block_list* block, size_t size);
void* malloc_list(size_t size);
void list_free(void* address);
size_t get_request_list();
size_t get_total_list();

#endif
