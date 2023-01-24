#include <stdio.h> 
#include <stdlib.h> 
#include <dlfcn.h>

typedef enum { 
  first_contract, 
  second_contract,
} contracts; 

contracts contract = first_contract;

const char* first_library_name = "libfirst.so"; 
const char* second_library_name = "libsecond.so";

float (*derivative)(float, float) = NULL; 
char* (*translation)(long) = NULL;

void* lib_handle = NULL;

void load_lib(contracts contract) { 
  const char* name;

  switch (contract) {
    case first_contract: {
      name = first_library_name;
      break; 
    }
        
    case second_contract: {
      name = second_library_name;
      break; 
    }
  }

  lib_handle = dlopen(name, RTLD_LAZY); 
  
  if (lib_handle == NULL) {
    perror("dlopen");
    exit(EXIT_FAILURE); 
  }
}

void load_contract() {
  load_lib(contract); 
  derivative = dlsym(lib_handle, "derivative");
  translation = dlsym(lib_handle, "translation"); 
}

void change_contract() { 
  dlclose(lib_handle); 
  
  switch(contract) {
    case first_contract: {
      contract = second_contract;
      break; 
    }

    case second_contract: { 
      contract = first_contract; 
      break;
    } 
  }

  load_contract(); 
}

int main() {
  load_contract();
  int command = 0;

  while (scanf("%d", &command) != EOF) {
    switch(command) {
      case 0: {
        change_contract();
        printf("Contract has been changed\n"); 
        
        switch(contract) {
          case first_contract: {
            printf("Contract is first\n");
            break; 
          }

          case second_contract: { 
            printf("Contract is second\n");
          } 
        }
        
        break; 
      }

      case 1: {
        float point, increment;
        
        if(scanf("%f %f", &point, &increment) == 2) {
          printf("%.6f\n", derivative(point, increment)); 
        }

        break; 
      }

      case 2: {
        long numeric;
        
        if(scanf("%ld", &numeric) == 1) {
          printf("Translation from 10 to "); 
        
          switch(contract) {
            case first_contract: { 
              printf("2");
              break; 
            }

            case second_contract: { 
              printf("3");
            } 
          }

          printf(" number system: %s\n", translation(numeric)); 
        }
        
      break; 
      }

      default: {
        printf("Invalid request\n");
      }
    } 
  }

  return 0; 
}
