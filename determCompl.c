#include "determCompl.h"
#include <math.h>
#include <string.h>

int isAsynchronous(FA* fa, int disp){
    int asynch = 0;
    for(int i = 1; i <= fa->nbStates; i++){
        if(fa->transTable[i][fa->nbAlpha+1][0]){
            if(!asynch){
                asynch = 1;
                if(disp)
                    printf("\nAsynchronous: ");
            }
            if(disp){
                for(int j = 1; j <= fa->transTable[i][fa->nbAlpha+1][0]; j++){
                    printf("%d*%d ", fa->transTable[i][0][1], fa->transTable[i][fa->nbAlpha+1][j]);
                }
            }
        }
    }
    if(disp)
        printf("\n");
    if(asynch){
        return 1;
    }
    if(disp)
        printf("Synchronous\n");
    return 0;
}

int isDeterministic(FA* fa, int disp){
    if(fa->init[0] == 1){
        int det = 1;
        for(int i = 1; i <= fa->nbStates; i++){
            for(int j = 1; j <= fa->nbAlpha; j++){
                if(!existingState(fa->transTable[i][j], fa)){ // State not yet created by determinization
                    if(det){
                        det = 0;
                        if(disp)
                            printf("\nNot deterministic: ");
                    }
                    if(disp){
                        for(int k = 1; k <= fa->transTable[i][j][0]; k++){
                            printf("%d%c%d ", fa->transTable[i][0][1], fa->transTable[0][j][0], fa->transTable[i][j][k]);
                        }
                    }
                }
            }
        }
        if(disp)
            printf("\n");
        if(det){
            if(disp)
                printf("Deterministic\n");
            return 1;
        }
        return 0;
    }
    if(disp)
        printf("\nNot deterministic: there must be ONE initial state\n");
    return 0;
}

int isComplete(FA* fa, int disp) {
    int compl = 1;
    for (int i = 1; i <= fa->nbStates; i++) {
        for (int j = 1; j <= fa->nbAlpha; j++) {
            if (!fa->transTable[i][j][0]) {
                if (compl) {
                    compl = 0;
                    if(disp)
                        printf("\nNot complete: ");
                }
                if(disp)
                    printf("%d%c- ", fa->transTable[i][0][1], fa->transTable[0][j][0]);
            }
        }
    }
    if(disp)
        printf("\n");
    if (compl) {
        if(disp)
            printf("Complete\n");
        return 1;
    }
    return 0;
}

FA* determCompl(FA* fa){
    FA* detFa = malloc(sizeof(FA));
    detFa->nbAlpha = fa->nbAlpha;
    detFa->nbStates = 1;

    // Initialize the table
    detFa->transTable = malloc((pow(2, fa->nbStates)+3)*sizeof(int**)); /* 2^n: max number of states for the determinized
                                                                           Spaces for possible 'P' and 'i' */
    for(int i = 0; i < pow(2, fa->nbStates)+3; i++){
        detFa->transTable[i] = malloc((detFa->nbAlpha+2)*sizeof(int*));
        for(int j = 0; j < detFa->nbAlpha+2; j++){
            detFa->transTable[i][j] = malloc((pow(2,fa->nbStates)+1)*sizeof(int));
            detFa->transTable[i][j][0] = 0;
        }
    }

    // Fill alphabet
    for(int j = 1; j <= detFa->nbAlpha; j++){
        detFa->transTable[0][j][0] = 96+j;
    }
    detFa->transTable[0][detFa->nbAlpha+1][0] = 42;

    // New initial state
    detFa->init = malloc(2*sizeof(int));
    detFa->init[0] = 1;
    detFa->init[1] = 1;
    // Combination of original initial states
    for(int i = 1; i <= fa->init[0]; i++){
        detFa->transTable[1][0][0]++;
        detFa->transTable[1][0][i] = fa->init[i]-1;
    }
    // Compute new states
    int i = 1;
    while(detFa->transTable[i][0][0]){
        for(int j = 1; j <= detFa->transTable[i][0][0]; j++){
            int state = searchLin(fa, detFa->transTable[i][0][j]);
            for(int k = 1; k <= fa->nbAlpha; k++){
                for(int l = 1; l <= fa->transTable[state][k][0]; l++){
                    // Add transitions of composing states if they are not already in it
                    if(!inArray(fa->transTable[state][k][l], detFa->transTable[i][k])){
                        int nb = ++detFa->transTable[i][k][0];
                        detFa->transTable[i][k][nb] = fa->transTable[state][k][l];
                    }
                }
                sortArray(detFa->transTable[i][k]); // So that 0.1.2 and 1.0.2 are the same for example
            }
        }
        // Add new states
        for(int j = 1; j <= detFa->nbAlpha; j++){
            if(!existingState(detFa->transTable[i][j], detFa)){
                detFa->nbStates++;
                copyArray(detFa->transTable[detFa->nbStates][0], detFa->transTable[i][j]);
            }
        }
        i++;
    }
    // Find final states
    detFa->term = malloc((detFa->nbStates+1)*sizeof(int));
    detFa->term[0] = 0;
    int final, j;
    for(int i = 1; i <= detFa->nbStates; i++){
        final = 0;
        j = 1;
        while(j <= fa->term[0] && !final){
            // Terminal if one original terminal state in a state
            if(inArray(fa->transTable[fa->term[j]][0][1], detFa->transTable[i][0])){
                final = 1;
                detFa->term[0]++;
                detFa->term[detFa->term[0]] = i;
            }
            j++;
        }
    }

    free(fa);
    // Complete
    if(!isComplete(detFa, 0)){
        complete(detFa);
    }
    return detFa;
}

FA* complete(FA* fa){
    fa->nbStates++;
    fa->transTable[fa->nbStates][0][0]++;
    fa->transTable[fa->nbStates][0][1] = -1; // -1 is P
    for (int i = 1; i <= fa->nbStates; i++) {
        for (int j = 1; j <= fa->nbAlpha; j++) {
            // Add -1 to empty transitions
            if (!fa->transTable[i][j][0]) {
                fa->transTable[i][j][0]++;
                fa->transTable[i][j][1] = -1;
            }
        }
    }
    return fa;
}

void sortArray(int* array){
    int sorted, temp;
    do{
        sorted = 1;
        for(int i = 1; i < array[0]; i++){
            if(array[i] > array[i+1]){
                temp = array[i];
                array[i] = array[i+1];
                array[i+1] = temp;
                sorted = 0;
            }
        }
    } while(!sorted);
}

int existingState(int* state, FA* fa){
    // Ignore empty state
    if(!state[0]){
        return 1;
    }

    int j;
    for(int i = 1; i <= fa->nbStates; i++){
        if(fa->transTable[i][0][0] == state[0]){
            j = 1;
            while(j <= state[0] && fa->transTable[i][0][j] == state[j]){
                j++;
            }
            if(j > state[0]){
                return 1;
            }
        }
    }
    return 0;
}

void copyArray(int* array2, int* array1){
    for(int i = 0; i <= array1[0]; i++){
        array2[i] = array1[i];
    }
}