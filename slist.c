#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define NUM_NUMS 20

typedef struct node {
    int val;
    struct node* next;
} Node;

typedef struct arg {
    pthread_t tid;
    int beginning_index;
    int ending_index;
} Arg;

Node* head = NULL;
int nums[NUM_NUMS];
pthread_mutex_t lock;

bool in_between(int val, Node* n1, Node* n2) {
    return !n2 || (n1->val <= val && n2->val >= val);
}

Node* get_prev(int val) {
    Node* cursor;
    for(cursor = head; cursor && !in_between(val, cursor, cursor->next); 
        cursor = cursor->next);

    return cursor;
}

Node* make_node(int val) {
    Node* node = malloc(sizeof(Node));
    node->val = val;
    return node;
}

void add_node(int val) {
    Node* nn = make_node(val);
    Node* prev_node;
   
    if(!head) {
        head = nn;
    }
    else if(head->val > val) {
        nn->next = head;
        head = nn;
    }
    else {
        prev_node = get_prev(val);
        nn->next = prev_node->next;
        prev_node->next = nn;
    }
        
}


void print_list() {
    Node* cursor;
    for(cursor = head; cursor; cursor = cursor->next) 
        printf("%d\n", cursor->val);
}

void* print_hi(void* args) {
    pthread_mutex_lock(&lock);
    printf("Hi from thread %d\n", *((int*) args));
    pthread_mutex_unlock(&lock);
}

int main(int num_args, char** args) {
    int num_threads = strtol(args[3], NULL, 10);
    int low = strtol(args[1], NULL, 10);
    int high = strtol(args[2], NULL, 10);
    int i;
    srand(time(0));
    Arg a;
     pthread_t tids[5];
     int blah[5];
     pthread_mutex_init(&lock, NULL);
     for(i = 0; i < 5; i++) {
        blah[i] = i;
        pthread_create(&(tids[i]), NULL, print_hi, (void*) &(blah[i]));
     }
     for(i = 0; i < 5; i++) {
         pthread_join(tids[i], NULL);
     }
// add_node((rand() % (high - low + 1)) + low);

//     for(i = 0; i < NUM_NUMS; i++) 
//         nums[i] = ((rand() % (high - low + 1)) + low);
    
//     pthread_t tids[5];
//     pthread_mutex_init(&lock, NULL);

//     int entries_per_thread = NUM_NUMS / num_threads;
//     for(i = 0; i < NUM_NUMS; i += entries_per_thread)
//         pthread_create(&tids[i/entries_per_thread]; )

//     print_list();
}
