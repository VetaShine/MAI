#include "unistd.h"
#include "stdio.h"
#include <string.h>
#include <ctype.h>
#include "structure.h"

void to_lower(char* string) {
  int string_length = strlen(string);

  for(int number = 0; number < string_length; number++) {
    string[number] = tolower(string[number]);
  }
}

int main() {
  structure parent_child1;
  structure child1_child2;

  if(structure_create(&parent_child1, "parent_child1", 0) || structure_create(&child1_child2, "child1_child2", 0)) {
    printf("error: cannot connect to shared memory\n");
    return 1;
  }

  while(1) {
    int input_length;
    char* input = structure_read(&parent_child1, &input_length);
    to_lower(input);
    structure_write(&child1_child2, input, strlen(input) + 1);
    free(input);
  }

  return 0;
}
