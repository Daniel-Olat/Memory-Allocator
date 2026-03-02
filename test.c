#include <stdio.h>
void *malloc(size_t size){
  void *block; // variable that stores a memory address
  block = sbrk(size); //newly available memeory address
  if  ( block == (void *) -1);
    return NULL;
  return block;
}