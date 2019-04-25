#include "determCompl.h"
#include <math.h>

int isAsynchronous(FA* fa){
    int epsCol = searchCol(fa, 42);
    int asynch = 0;
    for(int i = 1; i <= fa->nbStates; i++){
        if(fa->transTable[i][epsCol][0] != 0){
            if(!asynch){
                asynch = 1;
                printf("\nAsynchronous: ");
            }
            for(int j = 1; j <= fa->transTable[i][epsCol][0]; j++){
                printf("%d*%d ", fa->transTable[i][0][1], fa->transTable[i][epsCol][j]);
            }
        }
    }
    printf("\n");
    if(asynch){
        return 1;
    }
    printf("Synchronous\n");
    return 0;
}

int isDeterministic(FA* fa){
    if(fa->nbInit == 1){
        int det = 1;
        for(int i = 1; i <= fa->nbStates; i++){
            for(int j = 1; j <= fa->nbAlpha+1; j++){
                if(!existingState(fa->transTable[i][j], fa)){
                    if(det){
                        det = 0;
                        printf("\nNot deterministic: ");
                    }
                    for(int k = 1; k <= fa->transTable[i][j][0]; k++){
                        printf("%d%c%d ", fa->transTable[i][0][1], fa->transTable[0][j][0], fa->transTable[i][j][k]);
                    }
                }
            }
        }
        printf("\n");
        if(det){
            printf("Deterministic\n");
            return 1;
        }
        return 0;
    }
    printf("\nNot deterministic: there must be ONE initial state\n");
    return 0;
}

int isComplete(FA* fa) {
    int compl = 1;
    for (int i = 1; i <= fa->nbStates; i++) {
        for (int j = 1; j <= fa->nbAlpha; j++) {
            if (fa->transTable[i][j][0] == 0) {
                if (compl) {
                    compl = 0;
                    printf("\nNot complete: ");
                }
                printf("%d%c- ", fa->transTable[i][0][1], fa->transTable[0][j][0]);
            }
        }
    }
    printf("\n");
    if (compl) {
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
    detFa->transTable = malloc((pow(2,fa->nbStates)+2)*sizeof(int**)); // 2^n: max number of states for the determinized
    for(int i = 0; i <= pow(2, fa->nbStates)+1; i++){
        detFa->transTable[i] = malloc((detFa->nbAlpha+2)*sizeof(int*));
        for(int j = 0; j <= detFa->nbAlpha+1; j++){
            detFa->transTable[i][j] = malloc((fa->nbStates+1)*sizeof(int));
            detFa->transTable[i][j][0] = 0;
        }
    }

    // Fill alphabet
    for(int i = 1; i <= detFa->nbAlpha; i++){
        detFa->transTable[0][i][0] = 96+i;
    }
    detFa->transTable[0][detFa->nbAlpha+1][0] = 42;

    // Construction of the table
    detFa->nbInit = 1;
    detFa->init = malloc(sizeof(int));
    detFa->init[0] = 0;
    // New initial state
    for(int i = 0; i < fa->nbInit; i++){
        detFa->transTable[1][0][0]++;
        detFa->transTable[1][0][i+1] = fa->init[i];
    }
    // Compute new states
    int i = 1;
    while(detFa->transTable[i][0][0] != 0){
        for(int j = 1; j <= detFa->transTable[i][0][0]; j++){
            int state = searchLin(fa, detFa->transTable[i][0][j]);
            for(int k = 1; k <= fa->nbAlpha; k++){
                for(int l = 1; l <= fa->transTable[state][k][0]; l++){
                    if(!inArray(fa->transTable[state][k][l], detFa->transTable[i][k])){
                        int nb = ++detFa->transTable[i][k][0];
                        detFa->transTable[i][k][nb] = fa->transTable[state][k][l];
                    }
                }
                sortArray(detFa->transTable[i][k]);
            }
        }
        // Add new states
        for(int j = 1; j <= detFa->nbAlpha; j++){
            if(!existingState(detFa->transTable[i][j], detFa)){
                detFa->nbStates++;
                detFa->transTable[detFa->nbStates][0] = detFa->transTable[i][j];
            }
        }
        i++;
    }
    // Find final states
    detFa->nbTerm = 0;
    detFa->term = malloc(detFa->nbStates*sizeof(int));
    int final, k;
    for(int j = 1; j <= detFa->nbStates; j++){
        final = k = 0;
        while(k < fa->nbTerm && !final){
            if(inArray(fa->transTable[fa->term[k]+1][0][1], detFa->transTable[j][0])){
                final = 1;
                detFa->term[detFa->nbTerm] = j-1;
                detFa->nbTerm++;
            }
            k++;
        }
    }
    // Complete
    if(!isComplete(detFa)){
        complete(detFa);
    }
    return detFa;
}

FA* complete(FA* fa){
    fa->nbStates++;
    fa->transTable[fa->nbStates][0][0]++;
    fa->transTable[fa->nbStates][0][1] = -1;
    for (int i = 1; i <= fa->nbStates; i++) {
        for (int j = 1; j <= fa->nbAlpha; j++) {
            if (fa->transTable[i][j][0] == 0) {
                fa->transTable[i][j][0]++;
                fa->transTable[i][j][1] = -1;
            }
        }
    }
    return fa;
}

int inArray(int x, int* array){
    int i = 1;
    while(i <= array[0]){
        if(array[i] == x){
            return 1;
        }
        i++;
    }
    return 0;
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
    if(state[0] == 0){
        return 1;
    }

    int i = 1, j;
    while(i <= fa->nbStates){
        if(fa->transTable[i][0][0] == state[0]){
            j = 1;
            while(j <= state[0] && fa->transTable[i][0][j] == state[j]){
                j++;
            }
            if(j > state[0]){
                return 1;
            }
        }
        i++;
    }
    return 0;
}