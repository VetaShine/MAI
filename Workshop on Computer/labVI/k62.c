#include<stdio.h>                          
#include<string.h>

int main(int argc, char *argv[]) {
    FILE *fi, *fo;  
    printf("argc = %d\n", argc);
    
    struct  S {
        char fam[20], init[4], gen[1];
        int num, marks;
    }
    
    s, s1;
    int l = sizeof(s); 
    int n = 0;
    
    if(argc == 1) {
        if((fi = fopen("Base.dat", "rb")) == NULL) {
            printf("Cannot input"); 
            return 1;
        }
        
        if((fo = fopen("out.txt", "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    else if(argc == 2) {
        if((fi = fopen(argv[1], "rb")) == NULL) {
            printf("Cannot input"); 
            return 1;
        }
        
        if((fo = fopen("out.txt", "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    } 
    
    else {
        if((fi = fopen(argv[1], "rb")) == NULL) {
            printf("Cannot input"); 
            return 1;
        }
        
        if((fo = fopen(argv[2], "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    int numer = 1; 
    
    while(numer) { 
        printf("Input group number: numer = "); 
        scanf("%d", &numer);
        
        if(numer) { 
            fprintf(fo, "Query %d (For group number=%-4d):\n", ++n, numer);
            printf("=============================================================================\n");
            printf("|  Familija  | Initials | Gender | Group number | Marks for exams and tests |\n");
            printf("=============================================================================\n");
        }
        
        rewind(fi);
        
        while(!feof(fi)) {
            s1 = s;
            fread(&s, l, 1, fi);
            
            if(strcmp(s.fam, s1.fam)) { 
                char g[2] = "G"; 
                if(n && s.num == numer) {
                  if (strcmp(s.gen, g)) {
                    int m = s.marks, k = 0;
                    while(m) {
                      if(m%10 == 3) {
                          k+=1;
                      }
                        
                      m = m/10;
                      if(k>2) {
                          break;
                      }
                    }
                    if (k>2) {
                        fprintf(fo, "fam=%s init=%s gen=%s num=%d marks=%d\n", s.fam, s.init, s.gen, s.num, s.marks);
                        printf("| %-10s |   %.4s    |    %.1s   |     %-4d     |     %-16d      |\n", s.fam, s.init, s.gen, s.num, s.marks);
                    }
                  }
                }
            }
        }
        if(numer) {
            printf("=============================================================================\n");
        }
    } 
    
    return 0;
}
