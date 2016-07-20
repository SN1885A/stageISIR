#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dynaMG.h"

//Playground
int grid[GRID_SIZE][GRID_SIZE];

//Q-matrix
double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

PQueue pQueue;

//Actual state
int X = 0;
int Y = 0;

//Actual action
int A = 0;

//Nb step to converge
int step_to_converge = 0;

int main() {

	srand(time(NULL));
	pQueue = createPQueue();
	
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

	//All vectors initialization
	double theta[PHI_SIZE]; 	//Weight
	double b[PHI_SIZE]; 		//Rewards
	double F[PHI_SIZE][PHI_SIZE];   //Transition matrix
	double phi[PHI_SIZE];		//Feature vector

	for(i=0; i<PHI_SIZE; i++){ 
		theta[i] = 0;
		b[i] = 0;
		phi[i] = 0;
		for(j=0; j<PHI_SIZE; j++){
			F[i][j] = 0;
		}
	} 

	float delta = 0;

	printf("\nSTART Dyna-Q-MG\n\n");

	for(i=0; i<NB_TRY; i++) {
		delta = dyna_MG(theta, b, F, pQueue, Q, X, Y, A, &step_to_converge);
		if(delta<TETA_Q) break;
	}

	displayGridDirections(Q);
	printf("Nb step to converge = %d\n", step_to_converge);

	printf("END Dyna-Q-MG\n\n\n");

return 0;
}

