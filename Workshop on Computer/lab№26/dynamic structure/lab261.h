#ifndef LAB261_H
#define LAB261_H
#include <stdio.h>
#include <stdlib.h>
#define N 100
#define Tvalue int

struct El_St { 
  Tvalue V; 
  El_St *next;
};

struct Stack { 
  El_St *first; 
  int size;
};

void Init(Stack &S);
int Empty(Stack S);
void Push(Stack &S, Tvalue V);
Tvalue Pop(Stack &S);
Tvalue Top(Stack S);
int Size(Stack S);
void Cat(Stack &S1, Stack &S2);
void Append(Stack &S1, Stack &S2);
void Display(Stack S);
void Search_Swap(Stack &S, Stack &S1);       
void Sort(Stack &S);

#endif
