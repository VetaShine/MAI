#include <stdio.h>
#include "set.h"

char Word[20], S[2*N], H[1],             
Vowels[]="AEIOUYaeiouy", Sv[2*N], 
Ou[]="OUou", Sou[2*N], S1[2*N],  
Cons[]="BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz", Sc[2*N], 
Lt[]="LTlt", Slt[2*N], S2[2*N], 
R[]="DdHhRr", Sr[2*N], S3[2*N]; 

int main() {
  int i, n, m=0; char oldc=' ', c, k=' ';
  CreateSet(Vowels,Sv); CreateSet(Ou,Sou); CreateSet(Cons,Sc); CreateSet(Lt,Slt); CreateSet(R,Sr);
  while(i!=EOF) {
    i=getchar(); c=i;
    if(!Letter(oldc)&&Letter(c)){n=0;Word[0]=c;}
    if(Letter(oldc)&&Letter(c)) {H[0]=c;Word[++n]=c;}
    if(Letter(oldc)&&!Letter(c)) {
      Word[++n]='\0';
      CreateSet(H,S3); CreateSet(Word,S); IntersSet(S,Sv,S1); IntersSet(S,Slt,S2);
	   if(!EmptySet(S)&&KonSet(S3,Sr)){
       if(!EmptySet(S1)&&NoIncludeSet(S1,Sou)){ 
	   if(!EmptySet(S2)&&IncludeSet(S2,Slt)){
                     if (m==0) printf("Result:\n");
	      printf("%s ",Word); if(!(++m%10)) printf("\n"); }}}}
   oldc=c;}}
