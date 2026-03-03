#include <stdio.h>
#include <pthread.h> //Pthread library for thread management and synchronization
#include <unistd.h> //Unix standard library for sbrk function
// Minimal definitions to satisfy the custom allocator example.  The
// original snippet referenced types and symbols that weren't defined in
// this file, which caused red-line (intellisense) errors in the editor.

// simple header structure used by the allocator.
typedef struct header {
  struct {
    int is_free;
  } s;
  size_t size;
  struct header *next;
} header_t;

// global lock used by malloc implementation
static pthread_mutex_t global_malloc_lock = PTHREAD_MUTEX_INITIALIZER;

// stub for helper that searches free list; returns NULL for now
static header_t *get_free_block(size_t size)
{
  (void)size; // unused in stub
  return NULL;
}
void *malloc(size_t size){
  size_t total_size;
  void *block;
  header_t *header;
  if (!size)
      return NULL;
  pthread_mutex_lock(&global_malloc_lock);
  header = get_free_block(size);
  if (header){
      header  ->s.is_free = 0;
      pthread_mutex_unlock(&global_malloc_lock);
      return (void*)(header + 1);
  }
  // allocator not implemented yet; unlock and return NULL to avoid
  // deadlock/uninitialized return path
  pthread_mutex_unlock(&global_malloc_lock);
  return NULL;
}

// provide a minimal main to exercise the custom allocator and avoid
// linker complaints when building as a standalone program.
int main(void)
{
  void *p = malloc(16);
  (void)p;
  return 0;
}