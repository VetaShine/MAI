#include "unistd.h"
#include "stdio.h"
#include <fcntl.h>
#include <string.h>
#include "structure.h"

int create_child(char* file_name) {
  switch(fork()) {
    case -1: {
      return -1;
    }

    case 0:{
      char* args[] = {NULL};
      execv(file_name, args);
      return -1;
    }
  
    default: {
      break;
    }
  }

  return 0;
}

int main() {
  structure parent_child1;
  structure child1_child2;
  structure child2_parent;
  int number_string, count_string = 0;

  if(
    structure_create(&parent_child1, "parent_child1", 1) || 
    structure_create(&child1_child2, "child1_child2", 1) || 
    structure_create(&child2_parent, "child2_parent", 1)
  ) {
    printf("error: cannot create shared memory\n");
    return 1;
  }

  create_child("./child1");
  create_child("./child2");
  printf("Number of string:\n");
  scanf("%d", &number_string);
  int space;
  while((space = getchar()) != '\n' && number_string != EOF);
  printf("Enter string:\n");
  char buffer[256];

  while(1){
    fgets(buffer, 255, stdin);
    int string_length = strlen(buffer);
    structure_write(&parent_child1, buffer, string_length + 1);
    char *input = structure_read(&child2_parent, &string_length);
    printf("%s", input);
    free(input);
    fflush(stdout);
    count_string += 1;

    if (count_string == number_string) {
      structure_destroy(&parent_child1);
      structure_destroy(&child1_child2);
      structure_destroy(&child2_parent);
      break;
    }
  }

  return 0;
}
