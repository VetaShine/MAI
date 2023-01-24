#define N 26

char A='A'; char Z='Z'; char a='a'; char z='z'; char Alph[2*N];  

int Letter(char c) { return c>=a&&c<=z||c>=A&&c<=Z;}

void CreateSet(char W[], char S[]) { 
  char c; int i;
  for(i=0;i<N;i++)S[i]=S[N+i]=0;
  for(i=0;c=W[i];i++)
  if(c>=a&&c<=z) S[c-a]=1; else
  if(c>=A&&c<=Z) S[N+c-A]=1;}

void DisplaySet(char S[]) {
  int i;
  Alph[0]=a; Alph[N]=A; Alph[2*N]=0; 
  for(i=1;i<N;i++)
  { Alph[i]=Alph[i-1]+1;
  Alph[i+N]=Alph[i+N-1]+1; }       
  printf("\n%s\n",Alph);           
  for(i=0;i<2*N;i++)
  printf("%1d",S[i]); printf("\n");}  

void IntersSet(char S1[], char S2[], char S3[]) {
  for(int i=0;i<2*N;i++) S3[i]=S1[i]&&S2[i]; }

int NoIncludeSet(char S1[], char S2[]) { 
  for(int i=0;i<2*N;i++)
    if(S1[i]==1&&S2[i]==1) return 0; return 1;}
    
int KonSet(char S1[], char S2[]) {
    for(int i=0;i<2*N;i++)
      if (S1[i]==1&&S2[i]==0) return 0; return 1;}

int IncludeSet(char S1[], char S2[]) { 
  int k=0, s=0,n=1000;
  for(int i=0;i<2*N;i++){
    if(S1[i]==1&&S2[i]==1){ k+=1; s+=n; n/=10;}
    else if(S1[i]==0&&S2[i]==1){ n/=10;}
    if(n==0) n=1;}
  if(k==4||k==3) return 1;
  else if(k==1) return 0;                                           
  else { 
         if (s==1010||s==101) return 0;
         else return 1;}}

int EmptySet(char S[]) { 
  for(int i=0;i<2*N;i++)
    if(S[i]==1) return 0; return 1;}
