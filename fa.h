#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa {
    char** transTable;
    char* init;
    char* term;
} FA;

#endif
