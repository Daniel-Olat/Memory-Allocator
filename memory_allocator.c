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