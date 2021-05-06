#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

#define SBRK_ERR ((void*) -1)

typedef struct __attribute__((packed)) mem_header {
    bool free;
    size_t size;
    struct mem_header* next;
    struct mem_header* prev;
} Mem_header;

Mem_header* head;
Mem_header* tail;

Mem_header* get_block_w_size(size_t size) {
    Mem_header* cursor;
    for(cursor = head; cursor; cursor = cursor->next) {
        if(cursor->size >= size && cursor->free){
            return cursor;
        }
    }
    return NULL;
}

Mem_header* expand_pb(size_t size) {
    size_t expansion_size = tail ? size - tail->size : size;
    void* prev_break;
    if(SBRK_ERR == (prev_break = sbrk(expansion_size + sizeof(Mem_header))))
        return NULL;

    Mem_header* prev_break_base = (Mem_header*) prev_break;
    prev_break_base->free = false;
    prev_break_base->size = size;

    if(!head) {
        head = tail = prev_break_base;
    }
    else {
        prev_break_base->prev = tail;
        prev_break_base->next = NULL;
        tail->next = prev_break_base;
        tail = prev_break_base;
    }

    return prev_break_base;
}

void* mymalloc(size_t size) {
    Mem_header* mem_header; 
    if(NULL != (mem_header = get_block_w_size(size))) {
        mem_header->free = false;
        return mem_header+1;
    }
    else {
        if(NULL == (mem_header = expand_pb(size)))
            return NULL
            ;
        return mem_header+1;
    }
}

int main() {
    char* s = mymalloc(5);
    s[0] = 'a';
    s[1] = '\n';
    s[2] = '\0';
    printf("%s", s);
}