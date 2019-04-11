#ifndef READDISPLAY_H
#define READDISPLAY_H

#include "fa.h"

int searchCol(FA* fa, int n);
int searchLin(FA* fa, int n);
FA* readAutomaton(char* filename);
void displayAutomaton(FA* fa);
int isComplete(FA* fa);

#endif
