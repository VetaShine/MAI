#include "lab261.h"

void Init(Stack &S){ S.first=0; S.size=0;}

int Empty(Stack S){return S.first==0;}

void Push(Stack &S, Tvalue V){
    El_St *t=new El_St;
    t->next=S.first; S.first=t;
    S.first->V=V; S.size++;}

Tvalue Pop(Stack &S){
    if(Empty(S)) printf("STACK IS EMPTY");
    else { Tvalue V=S.first->V; S.first=S.first->next; S.size--; return V;}}

Tvalue Top(Stack S){
    if(Empty(S)) printf("STACK IS EMPTY");
    else return S.first->V;}

int Size(Stack S){return S.size;}

void Cat(Stack &S1, Stack &S2){
    Tvalue V; 
    if(!Empty(S2)){ V=Pop(S2); Cat(S1,S2); Push(S1,V);}}

void Append(Stack &S1, Stack &S2){
    Tvalue V;
    if(Empty(S1)) Cat(S1,S2);
    else{ V=Pop(S1); Append(S1,S2); Push(S1,V);}}

void Display(Stack S){
    if(Empty(S)) printf("\nSTACK IS EMPTY\n");
    else{
        printf("\n["); El_St *t=S.first;
        while(t) { printf("%d ",t->V); t=t->next;}
	    printf("]\n");}}
	    
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
