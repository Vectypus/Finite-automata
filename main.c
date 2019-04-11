#include "readDisplay.h"
#include "determCompl.h"

int main(){

    FA* fa = readAutomaton("FA example.txt");
    displayAutomaton(fa);
    isAsynchronous(fa);
    isDeterministic(fa);
	isComplete(fa);

	return 0;
}