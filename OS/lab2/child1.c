#include <stdio.h>
#include <unistd.h> 

char to_lower(char symbol) {
  if (symbol >= 'A' && symbol <= 'Z')
    return symbol + ('a' - 'A');

  return symbol;
  }
    
int main() {
  char symbol;

  while ((symbol = getchar()) != EOF) {
    printf("%c", to_lower(symbol));
    fflush(stdout);
  }

  return 0;
}
