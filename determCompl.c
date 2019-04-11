#include "determCompl.h"
#include "readDisplay.h"

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
                printf("%d*%d ", fa->transTable[i][0][0], fa->transTable[i][epsCol][j]);
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
                if(fa->transTable[i][j][0] > 1){
                    if(det){
                        det = 0;
                        printf("\nNot deterministic: ");
                    }
                    for(int k = 1; k <= fa->transTable[i][j][0]; k++){
                        printf("%d%c%d ", fa->transTable[i][0][0], fa->transTable[0][j][0], fa->transTable[i][j][k]);
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
                printf("%d%c- ", fa->transTable[i][0][0], fa->transTable[0][j][0]);
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