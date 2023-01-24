#include "realization.h" 
#include <stdlib.h> 
#include <math.h>

float derivative(float point, float increment) {
  return (cosf(point + increment) - cosf(point - increment)) / (2 * increment);
}

char* translation(long numeric){
  char* result = (char*)malloc(sizeof(char)); 
  int index = 0;

  while(numeric > 0) {
    result[index] = numeric % 3 + '0';
    index += 1;
    numeric /= 3;
    result = realloc(result, (index + 1) * sizeof(char));
  }

  result[index] = '\0';
  char symbol;

  for(int permutation_index = 0; permutation_index < index / 2; ++permutation_index) { 
    symbol = result[permutation_index];
    result[permutation_index] = result[index - permutation_index - 1];
    result[index - permutation_index - 1] = symbol; 
  }

  return result; 
}
