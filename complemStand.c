#include "complemStand.h"

FA* complementary(FA* fa){
    int* nonTerm = malloc(fa->nbStates*sizeof(int));
    nonTerm[0] = 0;
    // Find non terminal states
    for(int i = 1; i <= fa->nbStates; i++){
        if(!inArray(i, fa->term)){
            nonTerm[0]++;
            nonTerm[nonTerm[0]] = i;
        }
    }
    // Set new terminal states to previous non terminal
    fa->term = nonTerm;

    return fa;
}

int isStandard(FA* fa){
    
}

FA* standardize(FA* fa){

}