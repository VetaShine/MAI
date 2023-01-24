#include <stdio.h>
int main() {
    int i=1,n=0,f1=0,f2=0; char c=' '; 
    while(i!=EOF) { 
        i=getchar();c=i;
        if(c>='A'&&c<='Z'&&f1==0&&f2==0) {f1+=1;}
        else if (c>='a'&&c<='z'&&f1==1) continue; 
             else if ((c==' '||c=='\n')&&f1==1){ f1=0; f2=0; n++;}
                  else if (c==' '||c=='\n') { f1=0; f2=-1;}
                       else f1=0;
        f2++; }
    printf("\n"); printf("Number of nouns = %d \n",n);
    return 0;}
