#include "determComplAsynch.h"
#include "minimization.h"
#include "complemStand.h"

int main(){

    FA* fa = readAutomaton("FA 2.txt");
    printf("*** Automaton ***\n");
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
    printf("\n*** Complete deterministic automaton ***\n");
    displayAutomaton(cdfa, 1);

    FA* mcdfa = minimize(cdfa);
    printf("\n*** Complete deterministic minimal automaton ***\n");
    displayAutomaton(mcdfa, 1);

    char word[1000];
    printf("\n*** Word recognition ***\n(Enter a digit to exit the loop)\n");
    printf("Word to test: ");
    scanf("%s", word);
    while(word[0] < '0' || word[0] > '9'){
        recognizeWord(word, mcdfa);
        printf("Word to test: ");
        scanf("%s", word);
    }

    FA* complfa = complementary(mcdfa);
    printf("\n*** Complementary language automaton ***\n");
    displayAutomaton(complfa, 1);

    FA* complstdfa = standardize(complfa);
    printf("\n*** Complementary language standardized automaton ***\n");
    displayAutomaton(complstdfa, 1);

    printf("\n*** Word recognition ***\n(Enter a digit to exit the loop)\n");
    printf("Word to test: ");
    scanf("%s", word);
    while(word[0] < '0' || word[0] > '9'){
        recognizeWord(word, mcdfa);
        printf("Word to test: ");
        scanf("%s", word);
    }

	return 0;
}