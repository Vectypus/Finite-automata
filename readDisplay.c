#include <string.h>
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
	while(fa->transTable[i][0][0] != n){
		i++;
	}
	return i;
}

FA* readAutomaton(char* filename){
	FA* fa = malloc(sizeof(FA));

	FILE* file = NULL;
	file = fopen(filename, "r");
	int nbTrans;
	int s1, s2, t, test;
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
			fscanf(file, "%d", &test);
			if (test < fa->nbStates || test < 0) {
				fa->init[i] = test;
			}
			else {
				printf("/!\\ Unknown initial state !\n");
				exit(1);
			}
		}
		fscanf(file, "%d", &fa->nbTerm);
		fa->term = malloc(fa->nbTerm*sizeof(int));
		for(int i = 0; i < fa->nbTerm; i++){
			// Verify the final state(s) from the text file
			fscanf(file, "%d", &test);
			if (test < fa->nbStates || test < 0) {
				fa->term[i] = test;
			}
			else {
				printf("/!\\ Unknown terminal state !\n");
				exit(1);
			}
		}

		// Initialize the table
		fa->transTable = malloc((fa->nbStates+1)*sizeof(int**));
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
		fa->transTable[0][fa->nbAlpha+1][0] = 42;

		// Fill states
		for(int i = 1; i <= fa->nbStates; i++){
			fa->transTable[i][0][0] = -1+i;
		}

		// Fill transitions
		int j;
		fscanf(file, "%d", &nbTrans);
		for(int i = 0; i < nbTrans; i++){
			// Verify the source state(s) from the text file
			fscanf(file, "%d", &test);
			if (test < fa->nbStates || test < 0) {
				s1 = test;
			}
			else {
				printf("/!\\ Unknown source state !\n");
				exit(1);
			}
			fscanf(file, "%c", &c);
			// Verify the target state(s) from the text file
			fscanf(file, "%d", &test);
			if (test < fa->nbStates || test < 0) {
				s2 = test;
			}
			else {
				printf("/!\\ Unknown target state !\n");
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

void displayAutomaton(FA* fa){
	if(fa){
		printf("Initial states: ");
		for(int i = 0; i < fa->nbInit; i++){
			printf("%d ", fa->init[i]);
		}

		printf("\nTerminal states: ");
		for(int i = 0; i < fa->nbTerm; i++){
			printf("%d ", fa->term[i]);
		}

		printf("\n\nTransition table:\n");
		for(int i = 0; i <= fa->nbStates; i++){
			for(int j = 0; j <= fa->nbAlpha+1; j++){
				if(i != 0 || j != 0){
					if(i == 0){
						printf("%c ", fa->transTable[i][j][0]);
					}
					else if(j == 0){
						printf("%d ", fa->transTable[i][j][0]);
					}
					else{
						if(fa->transTable[i][j][0] == 0){
							printf("-");
						}
						for(int k = 1; k <= fa->transTable[i][j][0]; k++){
							printf("%d ", fa->transTable[i][j][k]);
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