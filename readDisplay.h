#ifndef READDISPLAY_H
#define READDISPLAY_H

#include "fa.h"

int searchCol(FA* fa, int n);
int searchLin(FA* fa, int n);
FA* readAutomaton(char* filename);
void displayAutomaton(FA* fa, int det);
int searchState(int* state, FA* fa);
int inArray(int x, int* array);
int recognizeWord(char* word, FA* fa);

#endif
