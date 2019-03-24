#ifndef READDISPLAY_H
#define READDISPLAY_H

#include "fa.h"

int searchCol(FA* fa, char c);
int searchLin(FA* fa, char c);
FA* readAutomaton(char* filename);
void displayAutomaton(FA* fa);

#endif
