#include "readDisplay.h"
#include "determCompl.h"

int main(){

    FA* fa = readAutomaton("FA example.txt");
    displayAutomaton(fa, isDeterministic(fa));
    isAsynchronous(fa);
	isComplete(fa);
    FA* detFa = determCompl(fa);
    displayAutomaton(detFa, isDeterministic(detFa));

	return 0;
}