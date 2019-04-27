#include "determComplAsynch.h"

FA* determComplAsynch(FA* fa){
    // Find '*'-elimination table states
    int** synchStates = malloc(fa->nbStates*sizeof(int*));
    for(int i = 0; i < fa->nbStates; i++){
        synchStates[i] = malloc((fa->nbStates+1)*sizeof(int));
        synchStates[i][0] = 0;
    }
    for(int i = 0; i < fa->nbInit; i++){
        synchStates[0][0]++;
        synchStates[0][i+1] = fa->init[i];
    }
    int nb = 1;
    for(int i = 1; i <= fa->nbStates; i++){
        if(!inArray(fa->transTable[i][0][1], synchStates[0]) && target(fa->transTable[i][0][1], fa)){
            synchStates[nb][0]++;
            synchStates[nb][1] = fa->transTable[i][0][1];
            nb++;
        }
    }
    // Make '*'-closures
    for(int i = 0; i < nb; i++){
        closures(synchStates[i], fa);
    }

    // Construct synchronous fa
    FA* synchFa = malloc(sizeof(FA));
    synchFa->nbAlpha = fa->nbAlpha;
    synchFa->nbStates = nb;

    // Initialize the table
    synchFa->transTable = malloc((synchFa->nbStates+1)*sizeof(int**));
    for(int i = 0; i <= synchFa->nbStates; i++){
        synchFa->transTable[i] = malloc((synchFa->nbAlpha+2)*sizeof(int*));
        for(int j = 0; j <= synchFa->nbAlpha+1; j++){
            synchFa->transTable[i][j] = malloc((fa->nbStates+1)*sizeof(int));
            synchFa->transTable[i][j][0] = 0;
        }
    }

    // Fill alphabet
    for(int i = 1; i <= synchFa->nbAlpha; i++){
        synchFa->transTable[0][i][0] = 96+i;
    }
    synchFa->transTable[0][synchFa->nbAlpha+1][0] = 42;

    // Construction of the table
    synchFa->nbInit = 1;
    synchFa->init = malloc(sizeof(int));
    synchFa->init[0] = 0;

    // Import states and transitions
    for(int i = 1; i <= synchFa->nbStates; i++){
        synchFa->transTable[i][0][0]++;
        synchFa->transTable[i][0][1] = synchStates[i-1][1];
        for(int j = 1; j <= synchFa->nbAlpha; j++){
            for(int k = 1; k <= synchStates[i-1][0]; k++){
                for(int l = 1; l <= fa->transTable[searchLin(fa, synchStates[i-1][k])][j][0]; l++){
                    if(!inArray(fa->transTable[searchLin(fa, synchStates[i-1][k])][j][l], synchFa->transTable[i][j])){
                        int nb = ++synchFa->transTable[i][j][0];
                        synchFa->transTable[i][j][nb] = fa->transTable[searchLin(fa, synchStates[i-1][k])][j][l];
                    }
                }
            }
        }
    }

    // Find final states
    synchFa->nbTerm = 0;
    synchFa->term = malloc(synchFa->nbStates*sizeof(int));
    int final, j;
    for(int i = 1; i <= synchFa->nbStates; i++){
        final = j = 0;
        while(j < fa->nbTerm && !final){
            if(inArray(fa->term[j], synchStates[i-1])){
                final = 1;
                synchFa->term[synchFa->nbTerm] = i-1;
                synchFa->nbTerm++;
            }
            j++;
        }
    }

    displayAutomaton(synchFa, 0);
    FA* detFa = determCompl(synchFa);

    return detFa;
}

int target(int x, FA* fa){
    int i = 1, j;
    while(i <= fa->nbStates){
        j = 1;
        while(j <= fa->nbAlpha){
            if(inArray(x, fa->transTable[i][j])){
                return 1;
            }
            j++;
        }
        i++;
    }
    return 0;
}

void closures(int* array, FA* fa){
    int nb = 0;
    while(array[0] != nb){
        nb = array[0];
        for(int i = 1; i <= nb; i++){
            for(int j = 1; j <= fa->transTable[searchLin(fa, array[i])][fa->nbAlpha+1][0]; j++){
                if(!inArray(fa->transTable[searchLin(fa, array[i])][fa->nbAlpha+1][j], array)){
                    array[0]++;
                    array[array[0]] = fa->transTable[searchLin(fa, array[i])][fa->nbAlpha+1][j];
                }
            }
        }
    }
}