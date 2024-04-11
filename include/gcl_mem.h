#ifndef MEM_H
#define MEM_H

#include <stddef.h>

#define checked_malloc(size)        (internal_checked_malloc ((size), __LINE__, __FILE__))
#define checked_realloc(ptr, size)  (internal_checked_realloc((ptr), (size), __LINE__, __FILE__))
#define checked_calloc(nmemb, size) (internal_checked_calloc ((nmemb), (size), __LINE__, __FILE__))
#define checked_free(ptr)           (internal_checked_free   ((void**)(&ptr), __LINE__, __FILE__))


void *internal_checked_malloc (size_t size, int line, const char *file);
void *internal_checked_calloc (size_t nmemb, size_t size, int line, const char *file);
void *internal_checked_realloc(void *ptr, size_t size, int line, const char *file);
void  internal_checked_free   (void **ptr, int line, const char *file);
void  check_allocations();


#endif