#include "determComplAsynch.h"

int main(){

    FA* fa = readAutomaton("FA 2.txt");
    displayAutomaton(fa, 0);

    FA* cdfa;
    if(isAsynchronous(fa))
        cdfa = determComplAsynch(fa);
    else{
        if(isDeterministic(fa)){
            if(isComplete(fa))
                cdfa = fa;
            else
                cdfa = complete(fa);
        }
        else
            cdfa = determCompl(fa);
    }
    displayAutomaton(cdfa, 1);

	return 0;
}