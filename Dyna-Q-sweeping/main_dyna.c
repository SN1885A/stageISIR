#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dyna.h"

int grid[GRID_SIZE][GRID_SIZE];
float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

Model model;
PQueue pQueue;

//Actual state
int X = 0;
int Y = 0;

//Actual action
int A = 0;
int step_to_converge = 0;

int main() {

	model.list = NULL;
	model.size = 0;

	pQueue = createPQueue();

	printf("START Dyna-Q\n");
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
	DynaQReturn dynaQReturn;

	for(i=0; i<NB_TRY; i++) {
		dynaQReturn = dyna_Q(model, pQueue, Q, X, Y, A, &step_to_converge);
		delta = dynaQReturn.delta;
		model = dynaQReturn.model;
		pQueue = dynaQReturn.pQueue;
		if(delta<TETA_Q) break;
	}

	displayGridDirections(Q);
	printf("Nb step to converge = %d\n", step_to_converge);
	printf("END Dyna-Q\n\n\n");

return 0;
}

