#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dynaMG.h"

//Playground
int grid[GRID_SIZE][GRID_SIZE];

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

	printf("\nSTART Dyna-Q-MG\n\n");
	
	dyna_MG(theta, b, F, pQueue, X, Y, A, &step_to_converge);
	int z;
	double phitest[PHI_SIZE];	
	double phitest2[PHI_SIZE];

	generateVect(phitest, 0, 0, 2);
	generateVect(phitest2, 4, 4, 0);
	//for(z = 0; z<PHI_SIZE; z++) printf("| %f |  | %f |\n", phitest[z], phitest2[z]);
	double result1 = 0; 
	double result2 = 0;;

	/*for(z = 0; z<PHI_SIZE; z++){
		result1 += phitest[z]*theta[z];
		result2 += phitest2[z]*theta[z];
		printf("%f * %f = %f; %f * %f = %f\n", phitest[z], theta[z], result1, phitest2[z], theta[z],  result2); 
	}

	printf("result1 = %f   result2 = %f\n", result1, result2); */
	displayGridDirections(theta);

	//printf("Nb step to converge = %d\n", step_to_converge);

	printf("END Dyna-Q-MG\n\n\n");

return 0;
}

