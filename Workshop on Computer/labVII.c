#include <stdio.h>

void umn_vstr_mtr(float*, float**, int, int);
void prn_mtr(float**, int, int);
void prn_vstr(float*, int);
void prn_vec(float*);

int main() {
    float **mtr, *vstr, *vec, y = 1; 
    int m, x = 1, n, i = 1, k = -1;
    printf("Input m = "); 
    scanf("%d", &m);
    printf("Input n = "); 
    scanf("%d", &n);
    vstr = new float[m];
    vec = new float[1000];
    printf("Enter the vector string elements: "); 
	
    for(i = 0; i < m; i++) {
	    scanf("%f",&vstr[i]);
    }
	
    mtr = new float*[m];
    printf("Enter the matrix: ");
	
    for(i = 0; i < m; i++) {
	    mtr[i] = new float[n];
    }
	
    while(x != 0 || y != 0) {
        k += 1; 
	scanf("%d", &x); 
	vec[k] = x; 
	k += 1; 
	scanf("%f", &y); 
	vec[k] = y;
	    
        if(x == 0 && y == 0) {
		break;
	}
	    
        else if (x == 0 && y != 0) {
		i = y - 1;
	}
	    
        else {
		mtr[i][x - 1] = y;
	}
    }
	
    prn_vstr(vstr, m);
    prn_vec(vec);
    prn_mtr(mtr, n, m);
    umn_vstr_mtr(vstr, mtr, n, m);
}

void prn_vec(float *vec) {
    int i = 0;
    printf("\nShort array:\n");
	
    while((vec[i] + vec[i + 1]) != 0) {
        printf("  %3.2f", vec[i]); i++;
    }
	
    printf("  %3.2f", vec[i]); 
    printf(" %3.2f", vec[i + 1]); 
    printf("\n");
}
        
void prn_vstr(float *vstr, int m) {
    int i; 
    printf("\nVector string:\n");
	
    for(i = 0; i < m; i++) {
        printf("  %3.2f", vstr[i]);
    }
	
    printf("\n");
}

void prn_mtr(float **mtr, int n, int m) {
    int x, y;  
    printf("\nFull matrix:");
	
    for(x = 0; x < m; x++) {
        printf("\n");
	    
        for(y = 0; y < n; y++) {
		printf("  %3.2f", mtr[x][y]);
	}
    }
}
    
void umn_vstr_mtr(float *vstr, float **mtr, int n, int m) {
    float *ummtr; 
    int i, j, x, k = 0;
    ummtr = new float[n];
	
    for(j = 0; j < n; j++) {
        ummtr[j] = 0;
	    
        for(i = 0; i < m; i++) {
            ummtr[j] += vstr[i] * mtr[i][j];
	}
    }
	
    printf("\n\nShort resulting array:\n");
	
    for(x = 0; x < m; x++) {
        if(ummtr[x] != 0) {
		printf("%3d%3d", k, x + 1); 
		printf("%3d  %3.2f", k + 1, ummtr[x]);
	}
    }
	
    printf("%3d%3d", k, k);   
    printf("\n\nFull resulting matrix:"); 
	
    for(x = 0; x < n; x++) {
        printf("\n");
	printf("  %3.2f ", ummtr[x]);
        if(ummtr[x] != 0) {
		k += 1;
	} 
    }
	
    printf("\n\nNumber of non-zero elements: "); 
    printf("%d", k); 
    printf("\n");
}
