#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

#define error_exit(...) \
            fprintf(stderr, "ERROR AT LINE:%d in FILE: %s: ", __LINE__, __FILE__);\
            fprintf(stderr, __VA_ARGS__);\
            exit(1)

#endif