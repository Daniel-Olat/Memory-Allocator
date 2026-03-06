#include <stdio.h>
//Main header or metadata structure
typedef struct block{
    size_t size;
    int free;
    struct block *next;
} block_t; // Instead of declaring struct block everytime.

block_t *head = NULL; //Pointer for inital header of the memory block. I think BRK

//Requesting memory from the OS, expanding the heap for malloc

block_t *request_space(block_t *last , size_t size){
    block_t *block;

    block = sbrk(size + sizeof(block_t));
    if (block == (void*) -1)
        return NULL;
    block->size = size;
    block->free = 0;
    block->next = NULL;

    if(last)
        last->next = block;

    return block;
};

//Memory block reusage
block_t *find_free_block(block_t **last, size_t size) {

    block_t *current = head;

    while (current && !(current->free && current->size >= size)) {
        *last = current;
        current = current->next;
    }

    return current;
}

//Implementing malloc
void *malloc(size_t size){
    block_t *block;
    if (size <=0)
        return NULL;
    if (!head){
        block  = request_space(NULL , size);
        if (!block)
            return NULL;
        head = block;
    } else{
        block_t *last =  head;
        block = find_free_block(&last , size);
        if(!block){
            block = request_space(last , size);
            if (!block)
                return NULL;
        } else{
            block->free = 0;
        }
    }
    return(block + 1);
}

//Implementing free
void free(void *ptr){
    if (!ptr)
        return;
    block_t *block  = (block_t*)ptr - 1;
    block->free = 1;
}