#include "determComplAsynch.h"
#include "minimization.h"
#include "complemStand.h"

int main(){
    int numFa;
    printf("*** Finite automata ***\n(Enter 0 to exit the loop)\nWhich automaton do you want to use ? ");
    scanf("%d", &numFa);
    while(numFa != 0){
        char path[20];
        sprintf(path, "automata/#%d.txt", numFa);
        FA* fa = readAutomaton(path);
        
        if(fa){
            printf("\n*** Automaton ***\n");
            displayAutomaton(fa);

            FA* cdfa;
            if(isAsynchronous(fa, 1))
                cdfa = determComplAsynch(fa);
            else{
                if(isDeterministic(fa, 1)){
                    if(isComplete(fa))
                        cdfa = fa;
                    else
                        cdfa = complete(fa);
                }
                else
                    cdfa = determCompl(fa);
            }
            printf("\n*** Complete deterministic automaton ***\n");
            displayAutomaton(cdfa);

            FA* mcdfa = minimize(cdfa);
            printf("\n*** Complete deterministic minimal automaton ***\n");
            displayAutomaton(mcdfa);

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
            displayAutomaton(complfa);

            FA* complstdfa = standardize(complfa);
            printf("\n*** Complementary language standardized automaton ***\n");
            displayAutomaton(complstdfa);

            printf("\n*** Word recognition ***\n(Enter a digit to exit the loop)\n");
            printf("Word to test: ");
            scanf("%s", word);
            while(word[0] < '0' || word[0] > '9'){
                recognizeWord(word, mcdfa);
                printf("Word to test: ");
                scanf("%s", word);
            }

            freeFa(complstdfa);
        }
        else{
            printf("This automaton doesn't exist.\n");
        }

        printf("\nWhich automaton do you want to use ? ");
        scanf("%d", &numFa);
    }

	return 0;
}