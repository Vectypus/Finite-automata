#include "complemStand.h"

FA* complementary(FA* fa){
    int* nonTerm = malloc((fa->nbStates+1)*sizeof(int));
    nonTerm[0] = 0;
    // Find non terminal states
    for(int i = 1; i <= fa->nbStates; i++){
        if(!inArray(i, fa->term)){
            nonTerm[0]++;
            nonTerm[nonTerm[0]] = i;
        }
    }
    // Set new terminal states to previous non terminal
    free(fa->term);
    fa->term = nonTerm;

    return fa;
}

int isStandard(FA* fa){
    if(fa->init[0] == 1){
        int stan = 1;
        for(int i = 1; i <= fa->nbStates; i++){
            for(int j = 1; j <= fa->nbAlpha; j++){
                if(searchState(fa->transTable[i][j],fa) == fa->init[1]){
                    if(stan){
                        stan = 0;
                        printf("\nNot standard: ");
                    }
                    printf("%d%c%d ", fa->transTable[i][0][1], fa->transTable[0][j][0], fa->transTable[fa->init[1]][0][1]);
                }
            }
        }
        printf("\n");
        if(stan){
            printf("Standard\n");
            return 1;
        }
        return 0;
    }
    printf("\nNot standard: there must be ONE initial state\n");
    return 0;
}

FA* standardize(FA* fa){
    if(!isStandard(fa)){
        fa->nbStates++;
        fa->transTable[fa->nbStates][0][0]++;
        fa->transTable[fa->nbStates][0][1] = -2;

        if(recognizeWord("*", fa)){
            fa->term[0]++;
            fa->term[fa->term[0]] = fa->nbStates;
        }

        // Already deterministic so one initial state
        for(int j = 1; j <= fa->nbAlpha; j++){
            copyArray(fa->transTable[fa->nbStates][j], fa->transTable[fa->init[1]][j]);
        }

        // Set new initial state
        fa->init[1] = fa->nbStates;
    }
    return fa;
}