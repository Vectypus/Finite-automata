#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa {
    char*** transTable;
    int nbLin;
    int nbCol;
    char* init;
    char* term;
} FA;

#endif
