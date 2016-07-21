#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dynaMGA.h"

//Nb step to converge
int step_to_converge = 0;

int main() {

	srand(time(NULL));

	int i, j, a;

	//All vectors initialization
	double theta[PHI_SIZE]; 			//Weights
	double b[NB_ACTIONS][PHI_SIZE]; 		//Rewards for every action a
	double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE];	//Transition matrix for every action a

	for(i=0; i<PHI_SIZE; i++) theta[i] = 0;
	
	for(a=0; a<NB_ACTIONS; a++){
		for(i=0; i<PHI_SIZE; i++){
			b[a][i] = 0;
			for(j=0; j<PHI_SIZE; j++){ 
				F[a][i][j] = 0;
			}
		} 
	}

	printf("\nSTART Dyna-Q-MG\n\n");
	
	dyna_MG(theta, b, F, &step_to_converge);

	displayGridDirections(b);

	printf("\nEND Dyna-Q-MG\n\n\n");

return 0;
}
