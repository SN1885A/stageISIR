#include <stdio.h>
#include <time.h>
#include <math.h>
#include "dynaMG.h"

//Nb step to converge
int step_to_converge = 0;

int main() {

	srand(time(NULL));

	int i, j;

	//All vectors initialization
	double theta[PHI_SIZE]; 	//Weight
	double b[PHI_SIZE]; 		//Rewards
	double F[PHI_SIZE][PHI_SIZE];   //Transition matrix

	for(i=0; i<PHI_SIZE; i++){ 
		theta[i] = 0;
		b[i] = 0;
		for(j=0; j<PHI_SIZE; j++){
			F[i][j] = 0;
		}
	} 

	printf("\nSTART Dyna-Q-MG\n\n");
	
	dyna_MG(theta, b, F, &step_to_converge);

	int z;
	/*double phitest1[PHI_SIZE];
	generateVect(phitest1, 0, 0, 0);
	for(z = 0; z<PHI_SIZE; z++) printf("| %f |\n", phitest1[z]);*/
	/*double phitest1[PHI_SIZE];	
	double phitest2[PHI_SIZE];
	double phitest3[PHI_SIZE];
	double phitest4[PHI_SIZE];

	double phitest5[PHI_SIZE];
	double phitest6[PHI_SIZE];
	double phitest7[PHI_SIZE];
	double phitest8[PHI_SIZE];

	generateVect(phitest1, 0, 0, 0);
	generateVect(phitest2, 0, 0, 1);
	generateVect(phitest3, 0, 0, 2);
	generateVect(phitest4, 0, 0, 3);

	double result1[4];
	result1[0] = multMatrixOneValue(phitest1, theta);
	result1[1] = multMatrixOneValue(phitest2, theta);
	result1[2] = multMatrixOneValue(phitest3, theta);
	result1[3] = multMatrixOneValue(phitest4, theta);

	generateVect(phitest5, 4, 4, 0);
	generateVect(phitest6, 4, 4, 1);
	generateVect(phitest7, 4, 4, 2);
	generateVect(phitest8, 4, 4, 3);

	double result2[4];
	result2[0] = multMatrixOneValue(phitest5, theta);
	result2[1] = multMatrixOneValue(phitest6, theta);
	result2[2] = multMatrixOneValue(phitest7, theta);
	result2[3] = multMatrixOneValue(phitest8, theta);

	//for(z = 0; z<PHI_SIZE; z++) printf("| %f |  | %f |\n", phitest[z], phitest2[z]);
	int action1, action2;
	double qm = result1[0];
	for(z=0; z<4; z++){
    		if (qm<result1[z]){
      			qm = result1[z];
      			action1 = z;
   		}
	}
	for(z=0; z<4; z++){
		printf("result1[%d] = %f\n", z, result1[z]);
	}

	printf("qm = %f\n", qm);

	qm = result2[0];
	for(z=0; z<4; z++){
    		if (qm<result2[z]){
      			qm = result2[z];
      			action2 = z;
   		}
	}
	for(z=0; z<4; z++){
		printf("result2[%d] = %f\n", z, result2[z]);
	}
	printf("qm = %f\n", qm);
	printf("action1 = %d - action2 = %d\n", action1, action2);*/

	displayGridDirections(theta);

	//printf("Nb step to converge = %d\n", step_to_converge);*/

	printf("\nEND Dyna-Q-MG\n\n\n");

return 0;
}
