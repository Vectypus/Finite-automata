#ifndef DETERMCOMPL_H
#define DETERMCOMPL_H

#include "readDisplay.h"

int isAsynchronous(FA* fa, int disp);
int isDeterministic(FA* fa, int disp);
int isComplete(FA* fa);
FA* determCompl(FA* fa);
FA* complete(FA* fa);
void sortArray(int* array);
int existingState(int* state, FA* fa);
void copyArray(int* array2, int* array1);

#endif
