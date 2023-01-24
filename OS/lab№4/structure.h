#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>

const int structure_block_size = 4096;

typedef struct structure {
  char memory_name[20];
  int file_descriptor;  
  char *buffer;
} structure;

typedef struct state {
  pthread_mutex_t mutex; 
  pthread_mutex_t write_mutex; 
  pthread_cond_t condition; 
  int memory_length; 
} state;

int structure_create(structure *memory_structure, char *memory_name, char host) {
  memory_structure->file_descriptor = shm_open(memory_name, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR); 
  
  if(memory_structure->file_descriptor == -1) { 
    return -1;
  }

  if(host) {
    if(ftruncate(memory_structure->file_descriptor, structure_block_size + sizeof(state))) { 
      return -1;
    }
  }

  memory_structure->buffer = mmap(
    NULL, 
    structure_block_size + sizeof(state), 
    PROT_READ | PROT_WRITE, 
    MAP_SHARED, 
    memory_structure->file_descriptor, 
    0
  ); 
  
  if(memory_structure->buffer == (void*) -1) {
    return -1;
  }

  if(host) {
    pthread_mutexattr_t attribute_mutex;
    pthread_condattr_t attribute_condition;

    if(pthread_mutexattr_init(&attribute_mutex) ||                      
      pthread_mutexattr_setpshared(&attribute_mutex, PTHREAD_PROCESS_SHARED) ||  
      pthread_mutex_init(&(((state*)(memory_structure->buffer))->mutex), &attribute_mutex) ||
      pthread_mutex_init(&(((state*)(memory_structure->buffer))->write_mutex), &attribute_mutex) ||
      pthread_mutex_lock(&(((state*)(memory_structure->buffer))->write_mutex)) ||
      pthread_condattr_init(&attribute_condition) ||                       
      pthread_condattr_setpshared(&attribute_condition, PTHREAD_PROCESS_SHARED) ||
      pthread_cond_init(&(((state*)(memory_structure->buffer))->condition), &attribute_condition)) {
      return -1;
    }
  }

  memcpy(memory_structure->memory_name, memory_name, strlen(memory_name) + 1); 
  return 0;
}

void structure_destroy(structure *memory_structure) {
  pthread_mutex_destroy(&(((state*)(memory_structure->buffer))->mutex));  
  pthread_mutex_destroy(&(((state*)(memory_structure->buffer))->write_mutex));
  pthread_cond_destroy(&(((state*)(memory_structure->buffer))->condition));  
  munmap(memory_structure->buffer, structure_block_size); 
  shm_unlink(memory_structure->memory_name); 
  close(memory_structure->file_descriptor);
  memory_structure->file_descriptor = -1;
}

void structure_write(structure *memory_structure, char *memory_string, int memory_length1) {
  pthread_mutex_lock(&(((state*)(memory_structure->buffer))->write_mutex));
  pthread_mutex_lock(&(((state*)(memory_structure->buffer))->mutex));
  ((state*)(memory_structure->buffer))->memory_length = memory_length1;
  memcpy(memory_structure->buffer + sizeof(state), memory_string, memory_length1);
  pthread_cond_broadcast(&(((state*)(memory_structure->buffer))->condition)); 
  pthread_mutex_unlock(&(((state*)(memory_structure->buffer))->mutex));  
}

char* structure_read(structure *memory_structure, int *length) {
  pthread_mutex_lock(&(((state*)(memory_structure->buffer))->mutex)); 
  pthread_mutex_unlock(&(((state*)(memory_structure->buffer))->write_mutex)); 
  pthread_cond_wait(&(((state*)(memory_structure->buffer))->condition), &(((state*)(memory_structure->buffer))->mutex)); 
  int memory_length1 = (((state*)(memory_structure->buffer))->memory_length);
  *length = memory_length1;
  char* memory = malloc(memory_length1);
  memcpy(memory, memory_structure->buffer + sizeof(state), memory_length1);
  pthread_mutex_unlock(&(((state*)(memory_structure->buffer))->mutex));
  return memory;
}
