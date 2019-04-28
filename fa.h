#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa {
    int*** transTable;
    int nbStates;
    int nbAlpha;
    int* init; // init[0] contains nbInit
    int* term; // term[0] contains nbTerm
} FA;

#endif
