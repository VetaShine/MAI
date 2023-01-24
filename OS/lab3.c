#include <stdio.h> 
#include <pthread.h> 
#include <time.h> 
#include <stdlib.h> 

int *array;

struct task { 
  int number; 
  int left;
  int right;
};

void merge(int low, int mid, int high) {
  int *left_array = malloc((mid - low + 1) * sizeof(int)); 
  int *right_array = malloc((high - mid) * sizeof(int)); 

  for(int i = 0; i < mid - low + 1; i++) {
    left_array[i] = array[i + low]; 
  }

  for(int i = 0; i < high - mid; i++) { 
    right_array[i] = array[i + mid + 1];
  }

  int count = low;
  int i = 0, j = 0; 

  while(i < mid - low + 1 && j < high - mid) {
    if(left_array[i] <= right_array[j]) { 
      array[count] = left_array[i];
      count += 1; 
      i += 1;
    } 
    else {
      array[count] = right_array[j];
      count += 1; 
      j += 1;
    } 
  }

  while(i < mid - low + 1) {
    array[count] = left_array[i];
    count += 1; 
    i += 1;
  }

  while(j < high - mid) {
    array[count] = right_array[j];
    count += 1; 
    j += 1;
  }

  free(left_array);
  free(right_array); 
}

void merge_sort(int low, int high) { 
  int mid = (low + high) / 2; 

  if(low < high) {
    merge_sort(low, mid); 
    merge_sort(mid + 1, high); 
    merge(low, mid, high);
  } 
}

void *merge_sort_thread(void *arg) { 
  struct task *tsk = arg; 
  int mid = (tsk -> left + tsk -> right) / 2; 

  if(tsk -> left < tsk -> right) {
    merge_sort(tsk -> left, mid); 
    merge_sort(mid + 1, tsk -> right); 
    merge(tsk -> left, mid, tsk -> right);
  } 
}

int main(int argv, char *argc[]) {
  struct task *control_param;
  int size, number_threads;
  printf("Number elements: \n");
  scanf("%d", &size);
  number_threads = *argc[1];
  array = malloc(sizeof(int) * size);
  printf("Array of integers: \n");

  for(int i = 0; i < size; i++) {
    scanf("%d", &array[i]);
  }

  pthread_t threads[number_threads];
  struct task tsklist[number_threads];

  if(number_threads > size) {
    number_threads = size;
  }

  int length = size / number_threads;
  int low = 0;

  for(int i = 0; i < number_threads; i++, low += length) {
    control_param = &tsklist[i];
    control_param -> number = i;
    control_param -> left = low;
    control_param -> right = low + length - 1;

    if(i == (number_threads - 1)) {
      control_param -> right = size - 1;
    }
  }

  for(int i = 0; i < number_threads; i++) {
    control_param = &tsklist[i];
    pthread_create(&threads[i], NULL, merge_sort_thread, control_param);
  }

  for(int i = 0; i < number_threads; i++) {
    pthread_join(threads[i], NULL);
  }
        
  struct tsk *tskm = &tsklist[0];
        
  for(int i = 1; i < number_threads; i++) {
    struct task *tsk = &tsklist[i];
    merge(tskm -> left, tsk -> left - 1, tsk -> right);
  } 
        
  printf("Sorted array: ");
        
  for(int i = 0; i < size; i++) {
    printf("%d ", array[i]);
  }

  printf("\n");
  return 0;
}
