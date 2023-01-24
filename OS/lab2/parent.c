#include <stdio.h>
#include <unistd.h>

int creation_child_processes(char *fname, int read, int write) {
  switch (fork()) {
    case -1:
      return -1;

    case 0: { 
      char *args[] = {NULL};

      if (dup2(read, 0) == -1) {
        printf("dup2 error!");
      }

      if (dup2(write, 1) == -1) {
        printf("dup2 error!");
      }

      if (execv(fname, args) == -1) {
        printf("execv error!");
      }

      return 0;
    }

    default:
      break;
  }

    return 0;
}

int main() {
  int pipe1[2], pipe2[2], pipe3[2];

  if (pipe(pipe1) == -1) {
    printf("Pipe1 error!");
  }

  if (pipe(pipe2) == -1) {
    printf("Pipe2 error!");
  }

  if (pipe(pipe3) == -1) {
    printf("Pipe3 error!");
  }

  if (creation_child_processes("./child1", pipe1[0], pipe2[1])){
    perror("fork error");
    return -1;
  }

  if (creation_child_processes("./child2", pipe2[0], pipe3[1])){
    perror("fork error");
    return -1;
  }

  printf("Enter string:\n");
  char symbol;

  while ((symbol = getchar()) != EOF) {
    write(pipe1[1], &symbol, 1);
    read(pipe3[0], &symbol, 1);
    printf("%c", symbol);
    fflush(stdout);
  }

  return 0;
}
