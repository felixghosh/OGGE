#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include "gcl_colors.h"

#define error_exit(...) \
            fprintf(stderr, "%sERROR AT LINE: %d in FILE: %s: %s", RED, __LINE__, __FILE__, END);\
            fprintf(stderr, __VA_ARGS__);\
            exit(1)

#define warning(...) \
            fprintf(stderr, "%sWARNING AT LINE: %d in FILE: %s: %s", YELLOW, __LINE__, __FILE__, END);\
            fprintf(stderr, __VA_ARGS__)

#endif