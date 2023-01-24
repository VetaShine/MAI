#include <stdio.h>

int main() {
   int a[9][9],n,i,j,k;
   printf ("Input n (<=9) = "); scanf ("%d",&n);
   for (i=0; i<n; i++) for (j=0; j<n; j++) a[i][j]=(i + 1)*10 + j + 1;
   printf ("Sourse matrix:\n");
   for (i=0; i<n; i++){ for (j=0; j<n; j++) printf ("%3d",a[i][j]); printf ("\n");}
   printf ("Result Vector:\n");
   if (n==1) { printf ("%3d",a[0][0]); printf ("\n");}
   i=-1; j=-1;
   for (k=0; k<2*n-3; k++) { 
        if ((k%4==0)||(k%4==3)) { j+=1; for (j; j<n; j++) {
            if (i<n) i+=1;
            else break;
            if (i<n && i>=0) { printf ("%4d",a[i][j]);}}}
        else  for (j=j-1; j>=0; j--) { 
            if (i==0) {j+=1; break;}
            i-=1;
            if (i<n && i>=0) { printf ("%4d",a[i][j]);}}
        if (j==-1) j=0;
        if (i==0 && j!=n-1) { j=n-j-1; i=n-1; printf ("%4d",a[i][j]);}
        else if (i==n) { i=0; j=(k/2-k/4)*2; printf("%4d",a[i][j]);}
        else if (j==0 && i!=n-1) { i+=1; printf ("%4d",a[i][j]);}
        else if (j==n) { i-=1; j-=1; printf ("%4d",a[i][j]);}
        if (i==n-1 && j==0) { i=0; j=n-1; printf ("%4d",a[i][j]); printf ("\n");}
        else if (i==0 && j==n-1) { i=n-1; j=0; printf ("%4d",a[i][j]); printf ("\n");}}}
