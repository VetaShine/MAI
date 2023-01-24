#include <stdio.h>
#include <time.h>
#include "allocator_list.h"
#include "allocator_mkk.h"

typedef struct request_structure {
  void* address;
  size_t bytes;
} request;

size_t parse_size(const char* string) {
  size_t size = 0;

  while(*string != '\0') {
    if(*string < '0' || *string > '9') {
      return 0;
    }

    size = size * 10 + *string - '0';
    ++string;
  }

  return size;
}

int main(int argument_count, char* argument_vector[]) {
  const size_t NUMBER_REQUESTS = 1000;
  const size_t MAX_BYTES = 5000;
  clock_t first_time;
  clock_t second_time;
  size_t first_index;
  size_t second_index;
  size_t third_index;
  size_t argument;
  size_t query = 0;
  size_t total = 0;
  size_t* permute = (size_t*)malloc(sizeof(size_t) * NUMBER_REQUESTS);
  request* requests = (request*)malloc(sizeof(request) * NUMBER_REQUESTS);

  srand((unsigned int)time(0));

  if(argument_count < 2) {
    printf("Usage: %s <SIZE>\n", argument_vector[0]);

    return 0;
  }

  argument = parse_size(argument_vector[1]);

  if(!initialization_list(argument)) {
    printf("Error. No memory\n");

    return 0;
  }

  if(!initialization_mkk(argument)) {
    printf("Error. No memory\n");

    return 0;
  }

  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    requests[first_index].bytes = 1 + rand() % MAX_BYTES;
    permute[first_index] = first_index;
  }

  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    second_index = rand() % NUMBER_REQUESTS;
    third_index = rand() % NUMBER_REQUESTS;
    argument = permute[second_index];
    permute[second_index] = permute[third_index];
    permute[third_index] = argument;
  }

  printf("Alloc requests: %zu\n", NUMBER_REQUESTS);
  printf("Bytes: 1 to %zu\n\n", MAX_BYTES);
  printf("Allocator LIST:\n");

  first_time = clock();

  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    requests[first_index].address = malloc_list(requests[first_index].bytes);
  }
  
  second_time = clock();

  printf("Alloc time: %lf\n", (double)(second_time - first_time) / CLOCKS_PER_SEC);

  query = get_request_list();
  total = get_total_list();

  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    if(requests[permute[first_index]].address == NULL) {
      continue;
    }
    
    list_free(requests[permute[first_index]].address);
  }
  
  first_time = clock();

  printf("Free time: %lf\n", (double)(first_time - second_time) / CLOCKS_PER_SEC);
  printf("Usage factor: %lf\n\n", (double)query / total);
  printf("Allocator MKK:\n");

  first_time = clock();
  
  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    requests[first_index].address = malloc_mkk(requests[first_index].bytes);
  }
  
  second_time = clock();

  printf("Alloc time: %lf\n", (double)(second_time - first_time) / CLOCKS_PER_SEC);

  query = get_request_mkk();
  total = get_total_mkk();

  for(first_index = 0; first_index < NUMBER_REQUESTS; ++first_index) {
    if(requests[permute[first_index]].address == NULL) {
      continue;
    }

    free_mkk(requests[permute[first_index]].address);
  }
  
  first_time = clock();

  printf("Free time: %lf\n", (double)(first_time - second_time) / CLOCKS_PER_SEC);
  printf("Usage factor: %lf\n", (double)query / total);
  
  destroy_list();
  destroy_mkk();

  free(requests);
  free(permute);

  return 0;
}
