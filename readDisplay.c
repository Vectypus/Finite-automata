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
	int s1, s2;
	char t;

	if(file != NULL){
		// Read alphabet and states
		fscanf(file, "%d", &fa->nbAlpha);
		fscanf(file, "%d", &fa->nbStates);

		// Fill initial and terminal states
		fa->init = malloc(fa->nbStates*sizeof(int));
		fscanf(file, "%d", &fa->init[0]);
		for(int i = 1; i <= fa->init[0]; i++){
			// Verify the initial state(s) from the text file
			fscanf(file, "%d", &fa->init[i]);
			fa->init[i]++; // line number in transTable
			if (fa->init[i] > fa->nbStates || fa->init[i] <= 0) {
				printf("/!\\ Unknown initial state '%d'!\n", fa->init[i]);
				exit(1);
			}
		}
		fa->term = malloc(fa->nbStates*sizeof(int));
		fscanf(file, "%d", &fa->term[0]);
		for(int i = 1; i <= fa->term[0]; i++){
			// Verify the final state(s) from the text file
			fscanf(file, "%d", &fa->term[i]);
			fa->term[i]++;
			if (fa->term[i] > fa->nbStates || fa->term[i] <= 0) {
				printf("/!\\ Unknown terminal state '%d'!\n", fa->term[i]);
				exit(1);
			}
		}

		// Initialize the table
		fa->transTable = malloc((fa->nbStates+3)*sizeof(int**)); // Spaces for possible 'P' and 'i' states
		for(int i = 0; i < fa->nbStates+3; i++){
			fa->transTable[i] = malloc((fa->nbAlpha+2)*sizeof(int*)); // Space for '*' trans
			for(int j = 0; j < fa->nbAlpha+2; j++){
				fa->transTable[i][j] = malloc((fa->nbStates+1)*sizeof(int));
				fa->transTable[i][j][0] = 0;
			}
		}

		// Fill alphabet
		for(int j = 1; j <= fa->nbAlpha; j++){
			fa->transTable[0][j][0] = 96+j;
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
			fscanf(file, "%c", &t);
			if (t > 96+fa->nbAlpha || (t < 97 && t != 42)) {
				printf("/!\\ Transition '%c' is not in the alphabet!\n", t);
				exit(1);
			}
			// Verify the target state(s) from the text file
			fscanf(file, "%d", &s2);
			if (s2 > fa->nbStates || s2 < 0) {
				printf("/!\\ Unknown target state '%d'!\n", s2);
				exit(1);
			}
			j = ++fa->transTable[s1+1][searchCol(fa, t)][0];
			fa->transTable[s1+1][searchCol(fa, t)][j] = s2;
		}

		fclose(file);
	}
	else{
		fa = NULL;
	}

	return fa;
}

void displayAutomaton(FA* fa){
	printf("Initial states: ");
	for(int i = 1; i <= fa->init[0]; i++){
		for(int j = 1; j <= fa->transTable[fa->init[i]][0][0]; j++){
			if(j == fa->transTable[fa->init[i]][0][0]){
				if(fa->transTable[fa->init[i]][0][j] == -2)
					printf("i");
				else
					printf("%d", fa->transTable[fa->init[i]][0][j]);
			}
			else
				printf("%d.", fa->transTable[fa->init[i]][0][j]);
		}
		printf(" ");
	}

	printf("\nTerminal states: ");
	for(int i = 1; i <= fa->term[0]; i++){
		for(int j = 1; j <= fa->transTable[fa->term[i]][0][0]; j++){
			if(j == fa->transTable[fa->term[i]][0][0]){
				if(fa->transTable[fa->term[i]][0][j] == -1)
					printf("P");
				else if(fa->transTable[fa->term[i]][0][j] == -2)
					printf("i");
				else
					printf("%d", fa->transTable[fa->term[i]][0][j]);
			}
			else
				printf("%d.", fa->transTable[fa->term[i]][0][j]);
		}
		printf(" ");
	}

	printf("\n\nTransition table:\n");
	for(int i = 0; i <= fa->nbStates; i++){
		for(int j = 0; j <= fa->nbAlpha+1; j++){
			if(!(!isAsynchronous(fa, 0) && j == fa->nbAlpha+1)){
				if(i == 0){
					printf("%c ", fa->transTable[0][j][0]);
				}
				else{
					if(fa->transTable[i][j][0] == 0){
						printf("-");
					}
					for(int k = 1; k <= fa->transTable[i][j][0]; k++){
						if(fa->transTable[i][j][k] == -1){
							printf("P");
						}
						else if(fa->transTable[i][j][k] == -2){
							printf("i");
						}
						else if(!isAsynchronous(fa, 0) && isDeterministic(fa, 0)){
							if(k == fa->transTable[i][j][0])
								printf("%d", fa->transTable[i][j][k]);
							else
								printf("%d.", fa->transTable[i][j][k]);
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

int searchState(int* state, FA* fa){
    int j;
    for(int i = 1; i <= fa->nbStates; i++){
        if(fa->transTable[i][0][0] == state[0]){
            j = 1;
            while(j <= state[0] && fa->transTable[i][0][j] == state[j]){
                j++;
            }
            if(j > state[0]){
                return i; // line number
            }
        }
    }
    return 0;
}

int inArray(int x, int* array){
    for(int i = 1; i <= array[0]; i++){
        if(array[i] == x){
            return 1;
        }
    }
    return 0;
}

int recognizeWord(char* word, FA* fa){
	int curr = fa->init[1], i = 0, t = 0;
	if(word[i] != '*'){
		while(word[i] != '\0'){
			t = word[i];
			curr = searchState(fa->transTable[curr][searchCol(fa, t)], fa);
			i++;
		}
	}

	if(inArray(curr, fa->term)){
		printf("Yes\n");
		return 1;
	}
	printf("No\n");
	return 0;
}