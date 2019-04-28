#include "determComplAsynch.h"
#include "minimization.h"

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

    FA* mcdfa = minimize(cdfa);
    displayAutomaton(mcdfa, 1);

    char word[1000];
    printf("Word to test: ");
    scanf("%s", word);
    while(word[0] < '0' || word[0] > '9'){
        recognizeWord(word, cdfa);
        printf("Word to test: ");
        scanf("%s", word);
    }

	return 0;
}