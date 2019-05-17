#include "minimization.h"

FA* minimize(FA* fa){
    int** groups0 = malloc(fa->nbStates*sizeof(int*));
    int nbGroups0 = 0;
    for(int i = 0; i < fa->nbStates; i++){
        groups0[i] = malloc((fa->nbStates+1)*sizeof(int));
        groups0[i][0] = 0;
    }

    int** groups1 = malloc(fa->nbStates*sizeof(int*));
    int nbGroups1;
    if (fa->term[0] == fa->nbStates)
        nbGroups1 = 1;
    else
        nbGroups1 = 2; // T and NT
    
    for(int i = 0; i < fa->nbStates; i++){
        groups1[i] = malloc((fa->nbStates+1)*sizeof(int));
        groups1[i][0] = 0;
    }
    // Groups at t0
    for(int i = 1; i <= fa->nbStates; i++){
        if(inArray(i, fa->term)){
            // Terminal states
            groups1[0][0]++;
            groups1[0][groups1[0][0]] = i-1; /* Renaming states with line number
                                                for simplicity */
        }
        else{
            // Non terminal states
            groups1[1][0]++;
            groups1[1][groups1[1][0]] = i-1;
        }
    }

    printf("\n*** Minimization process ***\n");
    corresTable(fa);
    int step = 0;
    displayGroups(groups1, nbGroups1, &step);
    displayTrans(fa, groups1, nbGroups1);

    int l, found;
    while(nbGroups0 != nbGroups1){ // Stop condition for minimization
        initGroups(groups0, &nbGroups0, groups1, &nbGroups1);
        l = 0;
        // Create groups1 from groups0
        for(int i = 0; i < nbGroups0; i++){
            for(int j = 1; j <= groups0[i][0]; j++){
                found = 0;
                for(int k = l; k < nbGroups1; k++){
                    // Add to the group if same transitions
                    if(sameTrans(fa, groups0[i][j], groups1[k][1], groups0, nbGroups0)){
                        found = 1;
                        groups1[k][0]++;
                        groups1[k][groups1[k][0]] = groups0[i][j];
                    }
                }
                // Create new group if no group with same transitions found
                if(!found){
                    groups1[nbGroups1][0]++;
                    groups1[nbGroups1][1] = groups0[i][j];
                    nbGroups1++;
                }
            }
            l = nbGroups1; // not to mix with other groups
        }
        displayGroups(groups1, nbGroups1, &step);
        displayTrans(fa, groups1, nbGroups1);
    }
    // If same number of states as the original fa
    if(nbGroups1 == fa->nbStates){
        printf("Already minimal\n");
        return fa;
    }
    else{
        FA* minFa = malloc(sizeof(FA));
        minFa->nbStates = nbGroups1;
        minFa->nbAlpha = fa->nbAlpha;

        // Initialize the table
        minFa->transTable = malloc((minFa->nbStates+2)*sizeof(int**)); // Space for possible 'i'
        for(int i = 0; i < minFa->nbStates+2; i++){
            minFa->transTable[i] = malloc((minFa->nbAlpha+2)*sizeof(int*));
            for(int j = 0; j < minFa->nbAlpha+2; j++){
                minFa->transTable[i][j] = malloc((fa->nbStates+1)*sizeof(int));
                minFa->transTable[i][j][0] = 0;
            }
        }

        // Fill alphabet
        for(int j = 1; j <= minFa->nbAlpha; j++){
            minFa->transTable[0][j][0] = 96+j;
        }
        minFa->transTable[0][minFa->nbAlpha+1][0] = 42;

        // Fill the table
        for(int i = 1; i <= minFa->nbStates; i++){
            // Add state (one group)
            copyArray(minFa->transTable[i][0], groups1[i-1]);
            for(int j = 1; j <= minFa->nbAlpha; j++){
                // Add group corresponding to the transition in the original fa
                int group = findGroup(fa, fa->transTable[minFa->transTable[i][0][1]+1][j], groups1, minFa->nbStates);
                copyArray(minFa->transTable[i][j], groups1[group]);
            }
        }

        // Find initial and final states
        minFa->init = malloc(2*sizeof(int));
        minFa->init[0] = 1;
        for(int i = 1; i <= minFa->nbStates; i++){
            // Initial if original initial state in the state
            if(inArray(fa->init[1]-1, minFa->transTable[i][0]))
                minFa->init[1] = i;
        }

        minFa->term = malloc((fa->term[0]+1)*sizeof(int));
        minFa->term[0] = 0;
        for(int i = 1; i <= minFa->nbStates; i++){
            // Final if state composed of original final state(s)
            if(inArray(minFa->transTable[i][0][1]+1, fa->term)){
                minFa->term[0]++;
                minFa->term[minFa->term[0]] = i;
            }
        }

        freeFa(fa);
        return minFa;
    }
}

int findGroup(FA* fa, int* state, int** groups, int nbGroups){
    int num = searchState(state, fa)-1;
    for(int i = 0; i < nbGroups; i++){
        for(int j = 1; j <= groups[i][0]; j++){
            if(num == groups[i][j])
                return i;
        }
    }
    return -1;
}

int sameTrans(FA* fa, int s1, int s2, int** groups, int nbGroups){
    for(int i = 1; i <= fa->nbAlpha; i++){
        // Compare transitions in terms of groups for each char
        if(findGroup(fa, fa->transTable[s1+1][i], groups, nbGroups) != 
           findGroup(fa, fa->transTable[s2+1][i], groups, nbGroups))
            return 0;
    }
    return 1;
}

void initGroups(int** groups0, int* nbGroups0, int** groups1, int* nbGroups1){
    *nbGroups0 = *nbGroups1;
    *nbGroups1 = 0;
    for(int i = 0; i < *nbGroups0; i++){
        for(int j = 0; j <= groups1[i][0]; j++){
            groups0[i][j] = groups1[i][j];
        }
        groups1[i][0] = 0;
    }
}

void displayGroups(int** groups, int nbGroups, int* step){
    printf("t%d = {", (*step)++);
    for(int i = 0; i < nbGroups; i++){
        printf("(");
        for(int j = 1; j <= groups[i][0]; j++){
            printf("%d ", groups[i][j]);
        }
        printf("), ");
    }
    printf("}\n");
}

void displayTrans(FA* fa, int** groups, int nbGroups){
    for(int j = 1; j <= fa->nbAlpha; j++){
        printf("\t%c", 96+j);
    }
    printf("\n");

    for(int i = 1; i <= fa->nbStates; i++){
        printf("%d\t", i-1);
        for(int j = 1; j <= fa->nbAlpha; j++){
            printf("G%d\t", findGroup(fa, fa->transTable[i][j], groups, nbGroups));
        }
        printf("\n");
    }
}

void corresTable(FA* fa){
    printf("Table of correspondence:\n");
    for(int i = 1; i <= fa->nbStates; i++){
        printf("%d -> ", i-1);
        for(int k = 1; k <= fa->transTable[i][0][0]; k++){
            if(fa->transTable[i][0][k] == -1)
                printf("P");
            else{
                printf("%d", fa->transTable[i][0][k]);
                if(k < fa->transTable[i][0][0])
                    printf(".");
                else
                    printf("\n");
            }
        }
    }
    printf("\n");
}