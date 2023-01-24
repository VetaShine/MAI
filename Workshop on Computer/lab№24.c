#include<stdio.h>
#include<stdlib.h>

typedef char tdata;

int i; char ch; 

struct node;

typedef node * link;

struct node{
    tdata data;
    link left,right;} 
*tree;

void printtree(link t){
    static int l=0;
    l++;
    if(t){
        printtree(t->right);
        for(i=0;i<l;i++)printf("    ");
        printf("\\__%c\n",t->data);
        printtree(t->left);}
    l--;} 

int isAN(){
    return (ch>='a')&&(ch<='z')||(ch>='0')&&(ch<='9');}

int isN(char c){
    return ((c>='a')&&(c<='z'));}

link mknode(char c, link l, link r){
    link t=new node;
    t->data=c; t->left=l; t->right=r;
    return t;}

link expr();

link fact(){
    link t;
    scanf("%c",&ch);
    if(ch=='('){ t=expr();
        if(ch!=')') printf("ERROR: not )\n");}
    else if(isAN()) t=mknode(ch,0,0);
	    else printf("ERROR: not AN\n");
   return t;}

link term(){
    link tm; int done; char ch1;
    tm=fact(); done=0;
    while((ch!='\n')&&(!done)){
       scanf("%c",&ch);
       if((ch=='*')||(ch=='/')){ch1=ch; tm=mknode(ch1,tm,fact());}
       else done=1;}
    return tm;}

link expr(){
    link ex; int done; char ch1;
    ex=term(); done=0;
    while((ch!='\n')&&(!done)){
        if((ch=='+')||(ch=='-')) {ch1=ch; ex=mknode(ch1,ex,term()); }
        else done=1;}
    return ex;}

void tree2expr(link tree){
    if(tree){
        if((tree->data=='+')||(tree->data=='-')) printf("(");
        tree2expr(tree->left);
        printf("%c",tree->data);
        tree2expr(tree->right);
        if((tree->data=='+')||(tree->data=='-')) printf(")");}}

int numvar(link tree, int *k, char *a, int n){
    if(tree){
        if(isN(tree->data)==1){ int c=0;
            for(int j=0;j<n;j++) if(a[j]==tree->data){c=1;break;}
            if(c==0){a[*k]=tree->data;*k+=1;}}
        numvar(tree->left,k,a,n);
        numvar(tree->right,k,a,n);}}

int main(){
    printf("Input expression:\n");
    tree=expr();
    printf("\n\n-----------------------\n\n");
    int k=0; int n=26; 
    char arr[26]; 
    numvar(tree,&k,arr,n); printf("Number of variables = %d",k);
    printf("\n\n-----------------------\n\n");
    printtree(tree);
    printf("\n\n-----------------------\n\n");
    tree2expr(tree);
    printf("\n\n-----------------------\n\n");
    return 0;}
