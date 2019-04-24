#ifndef DETERMCOMPL_H
#define DETERMCOMPL_H

#include "fa.h"

int isAsynchronous(FA* fa);
int isDeterministic(FA* fa);
int isComplete(FA* fa);
FA* determCompl(FA* fa);
int inArray(int x, int* array);
void sortArray(int* array);
int existingState(int* state, FA* fa);

#endif
