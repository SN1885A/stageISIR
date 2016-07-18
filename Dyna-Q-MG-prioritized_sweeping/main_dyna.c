#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dyna.h"

//Playground
int grid[GRID_SIZE][GRID_SIZE];
//Q-matrix
float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

PQueue pQueue;

//Actual state
int X = 0;
int Y = 0;

//Actual action
int A = 0;
int step_to_converge = 0;

int main() {

	pQueue = createPQueue();
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
	
	double theta[VECT_SIZE]; 	//Weight
	double b[VECT_SIZE]; 		//Rewards
	double F[VECT_SIZE][VECT_SIZE]; //Transition matrix
	double vect[VECT_SIZE];		//Feature vector

	for(i=0; i<VECT_SIZE; i++){ 
		theta[i] = 0;
		b[i] = 0;
		vect[i] = 0;
		for(j=0; j<VECT_SIZE; j++){
			F[i][j] = 0;
		}
	} 

	float delta = 0;
	DynaQReturn dynaQReturn;

	printf("START Dyna-Q\n");

	X = rand()%GRID_SIZE; 
	Y = rand()%GRID_SIZE;
	
	generateVect(vect, X, Y, 2);
	dyna_MG(theta, b, F, pQueue, Q, X, Y, A, step_to_converge);
	/*for(i=0; i<NB_TRY; i++) {
		dynaQReturn = dyna_Q(model, pQueue, Q, X, Y, A, &step_to_converge);
		delta = dynaQReturn.delta;
		model = dynaQReturn.model;
		pQueue = dynaQReturn.pQueue;
		//printf("delta = %5.3f ", delta);
		//printf("\n\n");
		if(delta<TETA_Q) break;

	}
	displayGridDirections(Q);
	printf("Nb step to converge = %d\n", step_to_converge);
	printf("END Dyna-Q\n\n\n");*/

return 0;
}

