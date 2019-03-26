#include <string.h>
#include "readDisplay.h"

int searchCol(FA* fa, char c){
    int i = 1;
    while(fa->transTable[0][i][0] != c){
        i++;
    }
    return i;
}

int searchLin(FA* fa, char c){
    int i = 1;
    while(fa->transTable[i][0][0] != c){
        i++;
    }
    return i;
}

FA* readAutomaton(char* filename){
    FA* fa = malloc(sizeof(FA));
    fa->init = malloc(50*sizeof(char));
    fa->term = malloc(50*sizeof(char));

    FILE* file = NULL;
    char* alpha = malloc(100*sizeof(char));
    char* states = malloc(100*sizeof(char));
    char* transitions = malloc(1000*sizeof(char));
    char* init = malloc(50*sizeof(char));
    char* term = malloc(50*sizeof(char));
    file = fopen(filename, "r");

    if(file != NULL){
        // Read file
        fgets(alpha, 100, file);
        fa->nbCol = strlen(alpha)/2 + 1;
        fgets(states, 100, file);
        fa->nbLin = strlen(states)/2 + 1;
        fgets(transitions, 1000, file);
        fgets(init, 50, file);
        int nbInit = strlen(init)/2;
        fgets(term, 50, file);
        int nbTerm = strlen(term)/2;
        fclose(file);

        // Initialize the table
        fa->transTable = malloc(fa->nbLin*sizeof(char**));
        for(int i = 0; i < fa->nbLin; i++){
            fa->transTable[i] = malloc(fa->nbCol*sizeof(char*));
            for(int j = 0; j < fa->nbCol; j++){
                fa->transTable[i][j] = malloc(10*sizeof(char));
                fa->transTable[i][j][0] = '\0';
            }
        }

        // Fill alphabet
        for(int i = 1; i < fa->nbCol; i++){
            fa->transTable[0][i][0] = alpha[(i-1)*2];
            fa->transTable[0][i][1] = '\0';
        }
        // Fill states
        for(int i = 1; i < fa->nbLin; i++){
            fa->transTable[i][0][0] = states[(i-1)*2];
            fa->transTable[i][0][1] = '\0';
        }
        // Fill transitions
        int i = 0, j = 0;
        char state, trans;
        while(transitions[i] != '\n'){
            if(i != 0)
                i++;
            j = 0;
            state = transitions[i++];
            trans = transitions[i++];
            while(fa->transTable[searchLin(fa, state)][searchCol(fa, trans)][j] != '\0'){
                j++;
            }
            fa->transTable[searchLin(fa, state)][searchCol(fa, trans)][j] = transitions[i++];
            fa->transTable[searchLin(fa, state)][searchCol(fa, trans)][j+1] = '\0';
        }
        // Fill initial and terminal states
        for(int i = 0; i < nbInit; i++){
            fa->init[i] = init[i*2];
        }
        fa->init[nbInit] = '\0';
        for(int i = 0; i < nbTerm; i++){
            fa->term[i] = term[i*2];
        }
        fa->term[nbTerm] = '\0';
    }
    else{
        fa = NULL;
    }

    return fa;
}

void displayAutomaton(FA* fa){
    if(fa){
        int i = 0;

        printf("Initial states: ");
        while(fa->init[i] != '\0'){
            printf("%c ", fa->init[i++]);
        }

        printf("\nTerminal states: ");
        i = 0;
        while(fa->term[i] != '\0'){
            printf("%c ", fa->term[i++]);
        }

        int k;
        printf("\n\nTransition table:\n");
        for(int i = 0; i < fa->nbLin; i++){
            for(int j = 0; j < fa->nbCol; j++){
                k = 0;
                while(fa->transTable[i][j][k] != '\0'){
                    printf("%c ", fa->transTable[i][j][k]);
                    k++;
                }
                printf("\t");
                if(j == fa->nbCol-1){
                    printf("\n");
                }
            }
        }
    }
}
