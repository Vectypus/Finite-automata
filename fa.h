#ifndef FA_H
#define FA_H

#include <stdio.h>
#include <stdlib.h>

typedef struct fa {
    int*** transTable;
    int nbStates;
    int nbAlpha;
    int* init; /* init[0] contains nbInit
                init contains lines of initial states */
    int* term; /* term[0] contains nbTerm
                term contains lines of final states */
} FA;

#endif
