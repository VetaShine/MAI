#include "realization.h"
#include <stdio.h>

int main() {
  int command = 0;
  
  while(scanf("%d", &command) != EOF) {
    switch (command) { 
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
          printf("%s\n", translation(numeric)); 
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
