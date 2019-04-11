#include "readDisplay.h"

int main(){

	FA* fa = readAutomaton("FA example.txt");
	displayAutomaton(fa);
	isComplete(fa);

	return 0;
}