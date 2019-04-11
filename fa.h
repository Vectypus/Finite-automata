#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa {
    int*** transTable;
    int nbStates;
    int nbAlpha;
    int nbInit;
    int nbTerm;
    int* init;
    int* term;
} FA;

#endif
