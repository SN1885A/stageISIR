#include <stdio.h>
#include <time.h>
#include <math.h>
#include "qL.h"

int grid[GRID_SIZE][GRID_SIZE];
float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

//Actual state
int X = 0;
int Y = 0;

//Actual action
int A = 0;
int step_to_converge = 0;

int delta; 

int main() {

	printf("START Q-learning\n");
	srand(time(NULL));
	//Grid initialization
	int i, j;
	for(i=0; i<GRID_SIZE; i++) 
		for(j=0; j<GRID_SIZE; j++) 
			grid[i][j] = 0;

	//Q-table initialization
	int a;
	for(i=0; i<GRID_SIZE; i++) 
		for(j=0; j<GRID_SIZE; j++) 
			for(a=0; a<NB_ACTIONS; a++) 
				Q[i][j][a]  = Q_INITIAL;

	//Begining
	float delta = 0;
	for(i=0; i<NB_TRY; i++) {
		delta = qLearning(i, Q, X, Y, A, step_to_converge);
		printf("delta = %5.3f ", delta);
		printf("\n\n");
		displayQ(Q);
		if(delta<TETA_Q) break;
	}

	printf("END Q-learning\n\n\n");
	
	test(4, 4, grid, Q);

return 0;
}

