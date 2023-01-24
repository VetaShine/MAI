#include "allocator_mkk.h"

int initialization_mkk(size_t size) {
  size_t index;
  block_mkk* block = NULL;
  
  pages_mkk = get_pages_count_mkk(size);
  pow_mkk = pow_of_size_mkk(PAGE_SIZE_MKK);
  pow_index_minimum = pow_of_size_mkk(sizeof(block_mkk));
  heap_mkk = malloc(pages_mkk * PAGE_SIZE_MKK);
  memory_size_mkk = (size_t*)malloc(sizeof(size_t) * pages_mkk);
  list_mkk = (block_mkk**)malloc(sizeof(block_mkk*) * pow_mkk);

  if(heap_mkk == NULL || memory_size_mkk == NULL || list_mkk == NULL) {
    return 0;
  }
  
  memory_size_mkk[free_state] = free_state;
  list_mkk[free_state] = (block_mkk*)heap_mkk;
  block = list_mkk[free_state];

  for(index = 1; index < pages_mkk; ++index) {
    memory_size_mkk[index] = free_state;
    block->next = (block_mkk*)((PBYTE_MKK)block + PAGE_SIZE_MKK);
    block = block->next;
  }

  block->next = NULL;

  for(index = 1; index < pow_mkk; ++index) {
    list_mkk[index] = NULL;
  }

  return 1;
}

void destroy_mkk() {
  free(heap_mkk);
  free(memory_size_mkk);
  free(list_mkk);
}

void* malloc_mkk(size_t size) {
  size_t pow_index = pow_of_size_mkk(size);
  size_t old_size = size;
  block_mkk* block = NULL;

  if(pow_index < pow_index_minimum) {
    pow_index = pow_index_minimum;
  }

  size = 1 << pow_index;
  
  if(size < PAGE_SIZE_MKK) {
    if(list_mkk[pow_index] == NULL) {
      block = alloc_page_mkk(size);

      if(block == NULL) {
        return NULL;
      }

    split_page_mkk(block, pow_index);
    }
    
    block = list_mkk[pow_index];
    list_mkk[pow_index] = block->next;

    request_mkk += old_size;
    total_mkk += size;

    return (void*)block;
  }
  else {
    request_mkk += old_size;
    total_mkk += size;

    return alloc_page_mkk(size);
  }
}

void free_mkk(void* address) {
  size_t page_index = get_page_index_mkk((block_mkk*)address);
  size_t pow_index = pow_of_size_mkk(memory_size_mkk[page_index]);
  block_mkk* block = (block_mkk*)address;
  
  if(memory_size_mkk[page_index] < PAGE_SIZE_MKK) {
    block->next = list_mkk[pow_index];
    list_mkk[pow_index] = block;
  }
  else {
    free_page_mkk(block);
  }
}

block_mkk* alloc_page_mkk(size_t size) {
  size_t count = 0;
  size_t page_index = 0;
  size_t previous_index = get_page_index_mkk(list_mkk[free_state]);
  size_t pages = get_pages_count_mkk(size);
  block_mkk* current = list_mkk[free_state];
  block_mkk* previous = NULL;
  block_mkk* page = NULL;

  while(current != NULL) {
    page_index = get_page_index_mkk(current);

    if(page_index - previous_index <= 1) {
      if(page == NULL) {
        page = current;
      }

      ++count;
    }
    else {
      page = current;
      count = 1;
    }

    if(count == pages) {
      break;
    }

    previous = current;
    current = current->next;
    previous_index = page_index;
  }

  if(count < pages) {
    page = NULL;
  }

  if(page != NULL) {
    page_index = get_page_index_mkk(page);
    memory_size_mkk[page_index] = size;
    current = (block_mkk*)((PBYTE_MKK)page + (pages - 1) * PAGE_SIZE_MKK);
    
    if (previous != NULL) { 
      previous->next = current->next;
    }
    else {
      list_mkk[free_state] = current->next;
    }
  }

  return page;
}

void free_page_mkk(block_mkk* block) {
  size_t index;
  size_t page_index = get_page_index_mkk(block);
  size_t block_count = memory_size_mkk[page_index] / PAGE_SIZE_MKK;
  block_mkk* left = NULL;
  block_mkk* right = NULL;
  block_mkk* current = block;

  while(current != NULL) {
    if (current < block) {
      left = current;
    }
    else {
      if(current > block) {
        right = current;

        break;
      }
    }

    current = current->next;
  }

  for(index = 1; index < block_count; ++index) {
    block->next = (block_mkk*)((PBYTE_MKK)block + PAGE_SIZE_MKK);
    block = block->next;
  }

  block->next = right;

  if(left != NULL) {
    left->next = block;
  }
  else {
    list_mkk[free_state] = block;
  }
}

void split_page_mkk(block_mkk* block, size_t pow_index) {
  size_t index;
  size_t page_index = get_page_index_mkk(block);
  size_t block_size = 1 << pow_index;
  size_t block_count = PAGE_SIZE_MKK / block_size;

  list_mkk[pow_index] = block;
  memory_size_mkk[page_index] = block_size;
  
  for(index = 1; index < block_count; ++index) {
    block->next = (block_mkk*)((PBYTE_MKK)block + block_size);
    block = block->next;
  }

  block->next = NULL;
}

size_t pow_of_size_mkk(size_t size) {
  size_t pow = 0;

  while(size > ((size_t)1 << pow)) {
    ++pow;
  }

  return pow;
}

size_t get_pages_count_mkk(size_t size) {
  return size / PAGE_SIZE_MKK + (size_t)(size % PAGE_SIZE_MKK != 0);
}

size_t get_page_index_mkk(block_mkk* block) {
  return (size_t)((PBYTE_MKK)block - (PBYTE_MKK)heap_mkk) / PAGE_SIZE_MKK;
}

size_t get_request_mkk() {
  return request_mkk;
}

size_t get_total_mkk() {
  return total_mkk;
}
