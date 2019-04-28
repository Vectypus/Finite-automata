#ifndef MINIMIZATION_H
#define MINIMIZATION_H

#include "readDisplay.h"

FA* minimize(FA* fa);
int findGroup(FA* fa, int* state, int** groups, int nbGroups);
int sameTrans(FA* fa, int s1, int s2, int** groups, int nbGroups);
void initGroups(int** groups0, int* nbGroups0, int** groups1, int* nbGroups1);
void displayGroups(int** groups, int nbGroups);

#endif
