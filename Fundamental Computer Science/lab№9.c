#include <stdio.h>

int max(int x, int y) { return (x > y) ? x : y;}

int sign(int x) {
  if (x > 0) return 1; 
  else { if (x < 0)  return -1;
         else return 0;}}

int abs(int x){
  if (x>=0)  return x;
  else  return x*(-1);}

int check(int x, int y) {
  if (((x - 10) * (x - 10) + (y - 10) * (y - 10) >= 25) && ((x - 10) * (x - 10) + (y - 10) * (y - 10) <= 100)) return 1;
  else  return 0;}

int main() {
  int i, j, l, i1, j1, l1, n=50, k=0;
  scanf("%d%d%d", &i, &j, &l);

  for (k = 0; k < n && check(i,j)==0; k++) {
    printf("%d %d %d %d\n", k, i, j, l);
    i1 = (max(j-k, l-k))%30 + (max(i+l, j+k))%20;
    j1 = (abs(i-l) * sign(j+k) + abs(i-k) * (j+k))%20;
    l1 = ((i+k) * (j-k) * (l+k))%25;
    i = i1; j = j1; l = l1;}

    if (check(i, j)) { printf("Попадание в заданную область не более чем за 50 шагов\n");
      printf("Время попадания = %d\n", k);
      printf("Конечные координаты точки = %d %d\n", i, j);
      printf("Значение динамического параметра движения = %d\n", l);} 
    else { printf("Промах\n");
      printf("Время окончания движения = %d\n", n);
      printf("Конечные координаты точки = %d %d\n", i, j);
      printf("Значение динамического параметра движения = %d\n", l);}}
