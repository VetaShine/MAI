#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct ls;
typedef ls* link;

struct ls {
    float body;
    link next;
} 

*list, *t, *f,*tail;

int i, n, k, c;
float m, m1;

void printlist() {
    if(!list) {
	    printf("\nList is empty!\n");
    }
	
    else { 
	    printf("\nList:\n[ "); 
	    t = list;
    	    do { 
		    printf("%2.2f ", t->body); 
		    t = t->next;
	    } 
	    
            while(t != NULL); 
	    printf("]\n");
    }
}
        
void add(float m) { 
    if(!list) { 
	    list = new ls; 
	    list->body = m; 
	    list->next = NULL; 
	    tail = list;
    }
	
    else { 
	    t = new ls; 
	    t->body = m; 
	    t->next = NULL; 
	    tail->next = t; 
	    tail = t;
    }
}
    
void addfirst(float m) { 
    if (!list) { 
	    list = new ls; 
	    list->body = m; 
	    list->next = NULL; 
	    tail = list;
    }
	
    else { 
	    t = new ls; 
	    t->body = m; 
	    tail->next = NULL; 
	    t->next = list; 
	    list = t;
    }
}

void genlist(int n) {
    for(i = 0; i < n; i++) {
        m = (float)(rand()%10000)/100; 
	add(m); 
    }
}
        
void deleting(float m) { 
    if(list->body == m) {
        f = list;
        if (list->next == NULL) { 
            list = NULL;
	}
	    
        else {
            list = list->next;
	}
	    
        delete f;
    }
	
    else {
        t = list; 
	c = 0; 
	    
        do {
            if (t->next->body == m) { 
                f = t->next; 
		t->next = f->next; 
		delete f; 
		c = 1; 
		break;
	    }
		
            t = t->next;
	}
	    
        while(t->next != NULL); 
	    
	if(!c) {
            printf("\nItem with this value not exists!\n");
	}
    }
}

void insert(float m, float m1) { 
    t = list; 
    c = 0; 
	
    if(t->body == m) { 
	    f = new ls; 
	    f->body = m1; 
	    f->next = t->next; 
	    t->next = f; 
	    return;
    }
	
    do {
        if(t->next->body == m) {
            f = new ls; 
	    f->body = m1;
            f->next = t->next->next; 
	    t->next->next = f; 
	    c = 1; 
	    break;
	}
	    
        t = t->next;
    }
	
    while(t != tail);
	
    if(!c) {
        printf("\nItem with this value not exists!\n"); 
	return;
    }
}

int size() {
    t = list; 
    n = 0; 
    if(t) 
	    do {
		    t = t->next; 
		    n++;
	    } 
	
	    while(t!=NULL); 
    return n;
}

void action(int n) {
    float a;
	
    for(int j = 0; j < n/2; j++) { 
	    t = list; 
	    
    	    for(int i = 0; i < j; i++) {
		    t = t->next;
	    } //printf(" %d",i);}
	    
    	    a = t->body; 
	    ls* t1 = t;
	    
    	    if(n%2 == 0) { 
		    for(int k = j; k < n/2 + j; k++) {
			    t = t->next; 
		    }
	    }
	    
    	    else { 
		    for(int k = j; k < n/2 + j + 1; k++) {
			    t = t->next; 
		    }
	    }
	    
    	    t1->body = t->body; 
            t->body = a; 
    }
}

int main() {
    list = 0; 
    int n, k = 9; 
    float m, m1;
	
    for(;;) {
        if(k == 9) {
		printf("\nInput from 1 to 10 or 0 for actions:\n"
            "1. Generation list.\n2. Addition new items in end of list.\n"
            "3. Addition new items in begin of list.\n"
            "4. Deleting from list.\n5. Print list.\n"
		    "6. Inserting in list.\n7. Erase list.\n"
		    "8. Lenght of list.\n9. Print this MENU\n"
		    "10. Main Action\n"
            "0. Exit out program.\n"); 
	}
	    
	printf("\nInput number of MENU: k = "); 
	scanf("%d",&k);
	    
        if(!k) {
		break;
	}
	    
        if(k == 1) {
		printf("\nInput number of new items of list: n = "); 
		scanf("%d", &n);
                genlist(n);
	}
	    
        else if(k == 2) { 
		printf("\nInput value of adding item: m = "); 
		scanf("%f", &m);
                add(m);
	}  
	    
        else if(k == 3) { 
		printf("\nInput value of adding item: m = "); 
		scanf("%f", &m);
                addfirst(m);
	}
	    
        else if(k == 4) { 
		if(!list) { 
			printf("\nList is empty!\n"); 
			continue;
		}
		
                printf("\nInput value of deleting item: m = "); 
		scanf("%f", &m);
                deleting(m);
	}  
	    
        else if(k == 5) {
		if(!list) {
			printf("\nList is empty!\n");
		}
		else {
			printlist();
		}
	}
	    
        else if(k == 6) {
		if(!list) { 
			printf("\nList is empty!\n"); 
			continue;
		}
		
                printf("\nInput value inserting item: m1 = "); 
		scanf("%f", &m1);
		printf("\nInput value item after which to insert: m = "); 
		scanf("%f", &m);
                insert(m, m1);
	}   
	    
        else if(k == 7) { 
		t = list; 
		if(t)
			do {
				f = t; 
				t = t->next; 
				delete f;
			} 
		        
		        while(t != NULL); 
		list=0;
	}
	    
        else if(k == 8) { 
		int f = size(); 
		printf("\nLenght of list = %d\n", f);
	} 
	    
        else if(k == 10) { 
		if(!list) { 
			printf("\nList is empty!\n"); 
			continue;
		} 
		
                int f = size(); 
		action(f);
	}
	    
        else if(k > 10) {
		printf("\nHave no such number of MENU\n");
	}
    }
}
