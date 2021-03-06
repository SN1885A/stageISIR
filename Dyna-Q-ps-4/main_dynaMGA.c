#include <stdio.h>
#include <time.h>
#include <math.h>
#include <mcheck.h>
#include <malloc.h>
#include "dynaMGA.h"

int episode_to_converge = 0;
int step_to_converge = 0;
double* theta;
//Reward vector
double** b;
//Forward transition matrix
double*** F;

int main() {

#ifdef WFILE
	int it;
	FILE *testSeed = NULL;
	testSeed = fopen("WithoutReplay_diffMax", "w");

	for(it = 0; it < 50; it++) {

		srand(it);
		printf("Seed = %d\n", it);
#endif
	int i, j, a;
#ifndef WFILE
	srand(67);
#endif

	theta = (double*) calloc(PHI_SIZE, sizeof(double));
	b = initMatrix(NB_ACTIONS, PHI_SIZE);
	F = initCube( NB_ACTIONS, PHI_SIZE, PHI_SIZE);

	printf("\nSTART Dyna-Q-MG\n\n");

#ifndef WFILE2
	dyna_MG(theta, b, F, &episode_to_converge, &step_to_converge, 0);
#endif

#ifdef WFILE2
	dyna_MG(theta, b, F, &episode_to_converge, &step_to_converge, testSeed);
#endif

	displayGridDirections(theta, b, F);
	printf("Convergence speed (episode) : %d  -------------  steps : %d\n",
			episode_to_converge, step_to_converge);

	free(theta);
	freeMatrix(b, NB_ACTIONS, PHI_SIZE);
	freeCube(F, NB_ACTIONS, PHI_SIZE, PHI_SIZE);

	printf("\nEND Dyna-Q-MG\n\n\n");

#ifdef WFILE
#ifndef WFILE2
	fprintf(testSeed, "%d;%d;%d;\n", it, step_to_converge, episode_to_converge);
#endif
	fflush(testSeed);

	step_to_converge = 0;
	episode_to_converge = 0;
}

fclose(testSeed);
#endif

	return 0;
}

