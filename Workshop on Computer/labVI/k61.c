#include<stdio.h>                          
#include<string.h>

int main(int argc, char *argv[]) {
    FILE *fi, *fo;  
    printf("argc = %d\n", argc);
    
    if(argc == 1) {                                      
        if((fi = fopen("in1.txt", "r")) == NULL) {
            printf("Cannot input 1"); 
            return 1;
        }   
        
        if((fo = fopen("Base.dat", "wb")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    else
        if(argc == 2) {                              
            puts(argv[1]); 
            printf("%s\n", argv[1]); 
            
        if((fi = fopen(argv[1], "r")) == NULL) {
            printf("Cannot input 2"); 
            return 1;
        }
            
        if((fo = fopen("Base.dat", "wb")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
        }
    
    else {
        if((fi = fopen(argv[1], "r")) == NULL) {
            printf("Cannot input 3"); 
            return 1;
        }
        
        if((fo = fopen(argv[2], "wb")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    struct  S {
        char fam[20], init[4], gen[1];
        int num, marks;
    }
    
    s, s1;
    int l = sizeof(s);
    int n = 0;
    
    while(!feof(fi)) {
        fscanf(fi, "%s %s %s %d %d",s.fam, s.init, s.gen, &s.num, &s.marks);
        fwrite(&s,sizeof(s), 1, fo); 
        n++;
    }
    
    fclose(fi);
    fclose(fo); 
    printf("n = %d\n", n);
    
    if(argc < 3) {
        if((fi = fopen("Base.dat", "rb")) == NULL) {
            printf("Cannot input");
            return 1;
        }
        
        if((fo = fopen("out.txt", "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    else if(argc == 3) {
        if((fi = fopen(argv[2], "rb")) == NULL) {
            printf("Cannot input"); 
            return 1;
        }
        
        if((fo = fopen("out.txt", "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    else {
        if((fi = fopen(argv[2], "rb")) == NULL) {
            printf("Cannot input"); 
            return 1;
        }
        
        if((fo = fopen(argv[3], "w")) == NULL) {
            printf("Cannot output"); 
            return 1;
        }
    }
    
    printf("=============================================================================\n");
    printf("|  Familija  | Initials | Gender | Group number | Marks for exams and tests |\n");
    printf("=============================================================================\n");
    fprintf(fo, "All in Base:\n");
    
    while(!feof(fi)) {
        s1 = s;
        fread(&s, l, 1, fi);
        
        if(strcmp(s.fam,s1.fam)) {
            fprintf(fo, "fam=%s init=%s gen=%s num=%d marks=%d\n", s.fam, s.init, s.gen, s.num, s.marks);
            printf("| %-10s |   %.4s    |    %.1s   |     %-4d     |     %-16d      |\n", s.fam, s.init, s.gen, s.num, s.marks);
        }
    }
    
    printf("=============================================================================\n");
    return 0;
}
