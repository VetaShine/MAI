#include "unistd.h"
#include "stdio.h"
#include <string.h>
#include "structure.h"

void replace_spaces(char* string, int* length) {
  for(int external_index = 0; external_index  < *length - 1; external_index ++) {
    if(string[external_index] == ' ') {
      while(string[external_index + 1] == ' ') {
        for(int internal_index = external_index + 1; internal_index < *length - 1; internal_index++) {
          string[internal_index] = string[internal_index + 1];
        }
        *length -= 1;
      }
    }
  }
}

int main() {
  structure child1_child2;
  structure child2_parent;

  if(structure_create(&child1_child2, "child1_child2", 0) || structure_create(&child2_parent, "child2_parent", 0)) {
    printf("error: cannot connect to shared memory\n");
    return 1;
  }

  while(1) {
    int input_length;
    char* input = structure_read(&child1_child2, &input_length);
    replace_spaces(input, &input_length);
    structure_write(&child2_parent, input, strlen(input) + 1);
    free(input);
  }

  return 0;
}
