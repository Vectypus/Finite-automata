#include "readDisplay.h"

int searchCol(FA* fa, int n){
	int i = 1;
	while(fa->transTable[0][i][0] != n){
		i++;
	}
	return i;
}

int searchLin(FA* fa, int n){
	int i = 1;
	while(fa->transTable[i][0][1] != n){
		i++;
	}
	return i;
}

FA* readAutomaton(char* filename){
	FA* fa = malloc(sizeof(FA));

	FILE* file = NULL;
	file = fopen(filename, "r");
	int nbTrans;
	int s1, s2, t;
	char c;

	if(file != NULL){
		// Read alphabet and states
		fscanf(file, "%d", &fa->nbAlpha);
		fscanf(file, "%d", &fa->nbStates);

		// Fill initial and terminal states
		fscanf(file, "%d", &fa->nbInit);
		fa->init = malloc(fa->nbInit*sizeof(int));
		for(int i = 0; i < fa->nbInit; i++){
			// Verify the initial state(s) from the text file
			fscanf(file, "%d", &fa->init[i]);
			if (fa->init[i] > fa->nbStates || fa->init[i] < 0) {
				printf("/!\\ Unknown initial state '%d'!\n", fa->init[i]);
				exit(1);
			}
		}
		fscanf(file, "%d", &fa->nbTerm);
		fa->term = malloc(fa->nbTerm*sizeof(int));
		for(int i = 0; i < fa->nbTerm; i++){
			// Verify the final state(s) from the text file
			fscanf(file, "%d", &fa->term[i]);
			if (fa->term[i] > fa->nbStates || fa->term[i] < 0) {
				printf("/!\\ Unknown terminal state '%d'!\n", fa->term[i]);
				exit(1);
			}
		}

		// Initialize the table
		fa->transTable = malloc((fa->nbStates+2)*sizeof(int**)); // Space for a possible 'P' state
		for(int i = 0; i <= fa->nbStates; i++){
			fa->transTable[i] = malloc((fa->nbAlpha+2)*sizeof(int*));
			for(int j = 0; j <= fa->nbAlpha+1; j++){
				fa->transTable[i][j] = malloc((fa->nbStates+1)*sizeof(int));
				fa->transTable[i][j][0] = 0;
			}
		}

		// Fill alphabet
		for(int i = 1; i <= fa->nbAlpha; i++){
			fa->transTable[0][i][0] = 96+i;
		}
		fa->transTable[0][fa->nbAlpha+1][0] = 42; // '*' char

		// Fill states
		for(int i = 1; i <= fa->nbStates; i++){
			fa->transTable[i][0][0]++;
			fa->transTable[i][0][1] = i-1;
		}

		// Fill transitions
		int j;
		fscanf(file, "%d", &nbTrans);
		for(int i = 0; i < nbTrans; i++){
			// Verify the source state(s) from the text file
			fscanf(file, "%d", &s1);
			if (s1 > fa->nbStates || s1 < 0) {
				printf("/!\\ Unknown source state '%d'!\n", s1);
				exit(1);
			}
			// Verify the transition from the text file
			fscanf(file, "%c", &c);
			if (c > 96+fa->nbAlpha || c < 97) {
				printf("/!\\ Transition '%c' is not in the alphabet!\n", c);
				exit(1);
			}
			// Verify the target state(s) from the text file
			fscanf(file, "%d", &s2);
			if (s2 > fa->nbStates || s2 < 0) {
				printf("/!\\ Unknown target state '%d'!\n", s2);
				exit(1);
			}
			t = c;
			j = fa->transTable[searchLin(fa, s1)][searchCol(fa, t)][0] + 1;
			fa->transTable[searchLin(fa, s1)][searchCol(fa, t)][j] = s2;
			fa->transTable[searchLin(fa, s1)][searchCol(fa, t)][0]++;
		}

		fclose(file);
	}
	else{
		fa = NULL;
	}

	return fa;
}

void displayAutomaton(FA* fa, int det){
	if(fa){
		printf("Initial states: ");
		for(int i = 0; i < fa->nbInit; i++){
			for(int j = 1; j <= fa->transTable[fa->init[i]+1][0][0]; j++){
				printf("%d", fa->transTable[fa->init[i]+1][0][j]);
			}
			printf(" ");
		}

		printf("\nTerminal states: ");
		for(int i = 0; i < fa->nbTerm; i++){
			for(int j = 1; j <= fa->transTable[fa->term[i]+1][0][0]; j++){
				printf("%d", fa->transTable[fa->term[i]+1][0][j]);
			}
			printf(" ");
		}

		printf("\n\nTransition table:\n");
		for(int i = 0; i <= fa->nbStates; i++){
			for(int j = 0; j <= fa->nbAlpha+1; j++){
				if(i != 0 || j != 0){
					if(i == 0){
						printf("%c ", fa->transTable[i][j][0]);
					}
					else{
						if(fa->transTable[i][j][0] == 0){
							printf("-");
						}
						for(int k = 1; k <= fa->transTable[i][j][0]; k++){
							if(fa->transTable[i][j][k] == -1){
								printf("P");
							}
							else if(det){
								printf("%d", fa->transTable[i][j][k]);
							}
							else{
								printf("%d ", fa->transTable[i][j][k]);
							}
						}
					}
				}
				printf("\t");
				if(j == fa->nbAlpha+1){
					printf("\n");
				}
			}
		}
	}
}