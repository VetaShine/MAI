#include <stdlib.h>
#include <stdio.h>  
#include <time.h>
int main() {	
    int a[9][9],n,k,i,j,min=10;
    printf ("Input size of matrix <10: n=");
    scanf ("%d",&n);
    srand(time(0));
    for(j=0;j<n;j++) {
             k=rand()%7+15; srand(k);
               for(i=0;i<n;i++) a[i][j]=rand()%10;}
    printf ("Started matrix:\n");
    for(i=0;i<n;i++) {
              for(j=0;j<n;j++) printf (" %3d ",a[i][j]);
              printf ("\n");}
    for(j=0;j<n;j++) { for(i=j;i<n;i++) if (a[i][j]<min) min=a[i][j];}
    for(j=0;j<n;j++){ for(i=0;i<j+1;i++) a[i][j]=a[i][j]*min;}
    printf ("Finish matrix:\n");
    for(i=0;i<n;i++) {
	    for(j=0;j<n;j++) printf (" %3d ",a[i][j]);
	    printf ("\n");}
    return 0;}
