#include <stdio.h>

int main() {
  long n,g,m,s=1,u=0;
  printf("Input number n = "); scanf("%ld",&n);
  g=n;m=n%10;n=n/10;
  while(n/10!=0) {
    if(m>=n%10) {u=u+(m-n%10)*s;}
    else {u=u+(n%10-m)*s;}
    m=n%10;s=s*10;n=n/10;}
  if(m>=n) {u=u+(m-n)*s;}
  else {u=u+(n-m)*s;}
  printf("New number = %ld\n",u);
  printf("Difference = %ld\n",g-u);}
