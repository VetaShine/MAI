#include "lab26.h"

void Init(Stack &S){S.first=0; }

int Empty(Stack S){return S.first==0;}

void Push(Stack &S, Tvalue V){ 
    if(S.first==N) printf("STACK IS OVERFLOW");
    else S.body[S.first++]=V;}

Tvalue Pop(Stack &S){
    if(Empty(S)) printf("STACK IS EMPTY");
    else return S.body[--S.first];}

Tvalue Top(Stack S){
    if(Empty(S)) printf("STACK IS EMPTY");
    else return S.body[S.first-1];}

int Size(Stack S){return S.first;}

void Display(Stack S){
    printf("\n[ ");
    for(int i=0;i<S.first;i++) printf("%d ",S.body[i]);
  printf("]\n");}

void Cat(Stack &S1, Stack &S2){
    Tvalue V;
    if(!Empty(S2)){ V=Pop(S2); Cat(S1,S2); Push(S1,V);}}

void Append(Stack &S1, Stack &S2){
    Tvalue V;
    if(Empty(S1)) Cat(S1,S2);
    else { V=Pop(S1); Append(S1,S2); Push(S1,V);}}

void Search_Swap(Stack &S, Stack &S1){
    bool swap=0; int f=Pop(S), s=Top(S1);
    while(f>s && !Empty(S1)){ s=Pop(S1); Push(S,s); 
    if (!Empty(S1)) s=Top(S1); 
    swap=1;}
    if(swap){ Push(S1,f); swap=0;}}
        
void Sort(Stack &S){
    Stack S1; Tvalue V,V1;
    Init(S1); int k=1;
    while (!Empty(S)){
        int e = Pop(S);
        if(Empty(S1) || Top(S1)>=e) Push(S1,e);
        else{ Push(S,e); Search_Swap(S,S1);}}
        while (!Empty(S1)) Push(S,Pop(S1));
    Init(S1);}
