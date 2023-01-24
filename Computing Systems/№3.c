#include <stdio.h>
#include <math.h>
int main() {
  double x, f, d, S, eps=1., a=0.1, b=0.6, h=(b-a)/10.;
  int n;
  while(1.+eps>1.) eps/=2.; 
  printf("Machine epsilon = %.21f\n",eps);
  printf("------------------------------------------------------------------\n");
  printf("|   x   |           S             |   (2x-3)/((x-1)*(x-1))  | n  |\n");
  printf("|-------|-------------------------|-------------------------|----|\n");
  for(x=a;x<=b;x+=h) {
    S=0; n=0; d=-2/x;
    while(fabs(d)>eps) {
      if (n>=100) break;
	  d=d*x*(n+3)/(n+2);
	  S+=d; n++;}
	f=(2*x-3)/((x-1)*(x-1));
    if (f>=10||f<=-10) printf("| %.3f | %.19f | %.19f | %2d |\n", x, S, f, n);
    else printf("| %.3f | %.20f | %.20f | %2d |\n", x, S, f, n);}
   printf("------------------------------------------------------------------\n");
   return 0;}
