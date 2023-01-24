#include <stdio.h>
#include <unistd.h>

void print_char(char symbol) {
  printf("%c", symbol);
  fflush(stdout);
}

int main() {
  char symbol;

  while ((symbol = getchar()) != EOF) {
    print_char(symbol);

    if (symbol == ' ') {
      while ((symbol = getchar()) == ' ')
        print_char(0);
      print_char(symbol);
    }
  }

    return 0;
}
