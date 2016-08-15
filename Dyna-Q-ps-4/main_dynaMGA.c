#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mcheck.h>
#include <malloc.h>
#include "dynaMGA.h"

//Nb episode/step to converge
int episode_to_converge = 0;
int step_to_converge = 0;
double* theta ;
double** b;
double*** F;


int main() {

	int it;
	FILE *testSeed = NULL;
	testSeed = fopen("SeedWithReplay_policyverif", "w+");

	for(it = 0; it < 100; it++){

		srand(it);

		printf("Seed = %d\n", it);

		int i, j, a;

		//All vectors initialization
//		double theta[PHI_SIZE]; 					//Weights
//		double b[NB_ACTIONS][PHI_SIZE]; 			//Rewards for every action a
//		double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE];	//Transition matrix for every action a

#ifdef DEBUG
		mtrace();
#endif


		 theta = (double*) calloc(PHI_SIZE , sizeof(double));
		 b = initMatrix(NB_ACTIONS , PHI_SIZE);
		 F = initCube( NB_ACTIONS , PHI_SIZE, PHI_SIZE);



		printf("\nSTART Dyna-Q-MG\n\n");
dyna_MG(theta, b, F, &episode_to_converge, &step_to_converge);
	
		//displayGridDirections(theta, b, F);
		//printf("Convergence speed (episode) : %d  -------------  steps : %d\n", episode_to_converge, step_to_converge);
free(theta);
freeMatrix(b , NB_ACTIONS , PHI_SIZE);
freeCube(F  , NB_ACTIONS , PHI_SIZE, PHI_SIZE);
#ifdef DEBUG
		muntrace();
#endif
		printf("\nEND Dyna-Q-MG\n\n\n");


	    fprintf(testSeed, "%d;%d;%d;\n", it, step_to_converge,  episode_to_converge);
	    fflush(testSeed);
	    step_to_converge = 0;
	    episode_to_converge = 0;
	}
fclose(testSeed);

return 0;
}

