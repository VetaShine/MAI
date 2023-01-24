#ifndef LAB26_H
#define LAB26_H
#include <stdio.h>
#include <stdlib.h>
#define N 100
#define Tvalue int

struct Stack {
  int first; 
  Tvalue body[N];
};

void Init(Stack &S);
int Empty(Stack S);
void Push(Stack &S, Tvalue V);
Tvalue Pop(Stack &S);
Tvalue Top(Stack S);
int Size(Stack S);
void Display(Stack S);
void Cat(Stack &S1, Stack &S2);
void Append(Stack &S1, Stack &S2);
void Search_Swap(Stack &S, Stack &S1);
void Sort(Stack &S);

#endif
