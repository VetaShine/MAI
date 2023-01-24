#include "lab26.h"
int main(){
    Stack S, S1, S2; Init(S); Tvalue v; int p=1,n,k;
    printf("\nChoose action:"
	"\n1. Create Random Stack\n2. Print Stack\n3. Size of Stack"
	"\n4. Insert of Stack"
	"\n5. Delete from front of Stack"
	"\n6. Sorting Stack\n7. Clear Stack"
	"\n0. Exit");
	 while(p){
	  printf("\nInput number => "); scanf("%d",&p);
    if(p==1)
      { printf("Input number of elements => "); scanf("%d",&n);
      Init(S1);
      for(int i=0;i<n/2;i++){v=rand()%10; Push(S1,v);}
      Display(S1);
      Init(S2);
      for(int i=0;i<n-n/2;i++){v=rand()%10; Push(S2,v);}
      Display(S2); Cat(S1,S2); Append(S,S1); Display(S);}
    if(p==2) Display(S);
    if(p==3) { n=Size(S); printf("\nSize of Stack = %d\n",n); }
    if(p==4) { printf("\nInput value of element => "); scanf("%d",&v); Push(S,v);}
    if(p==5) { v=Pop(S); printf("\nElement = %d is deleted\n",v); }
    if(p==6) Sort(S);
    if(p==7) Init(S);
    if(p==0) break;}}
