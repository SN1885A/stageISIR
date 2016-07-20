#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dynaMG.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////

float bestQ(int i, int j, double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
  float qm = -1000;
  int a;
  for (a=0; a<NB_ACTIONS; a++) 
    if (qm<Q[i][j][a]) {
      qm = Q[i][j][a];
    }
  return qm;
}

int bestAction(int i, int j, double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	float qm = -1000;
	int a, action = 0;
  	for (a=0; a<NB_ACTIONS; a++) 
    		if (qm<Q[i][j][a]){
      			qm = Q[i][j][a];
      			action = a;
   		}
return action;
}

int bestActionForTest(int i, int j, double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	float qm = -1;
	int a, action = 0;
	if( (i==RWX && j==RWY) || (i==RW2X && j==RW2Y) ) return WIN;
	for (a=0; a<NB_ACTIONS; a++)
		if (qm<Q[i][j][a]){
			qm = Q[i][j][a];
			action = a;
		}
return action;
}

int e_greedy(int x, int y, float e, double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

	int a = 0;
	double r = (double)rand()/RAND_MAX;
	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action
	if( (x==RWX && y==RWY) || (x==RW2X && y==RW2Y) || (r < e) ) a = rand()%NB_ACTIONS;
	else a = bestAction(x, y, Q);
	
return a;
}

double generateGaussian(int var, int ectype, double d){
	
	double fact1 = 1/(sqrt( (M_PI*2)*ectype));
	double div1, div2;
	div1 = -( pow( (d-VAR), 2) );
	div2 = 2*pow(ectype, 2);
	double fact2 = div1/div2;
	double fact3 = exp(fact2);
	double result = fact1*fact3;

return result;
}

void multMatrixCarreCol(double result[PHI_SIZE], double mat1[PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE]){

	int i, j, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<PHI_SIZE; i++)
		for(j = 0; j<1; j++)
			for(z = 0; z<PHI_SIZE; z++)
				result[i] += mat1[i][z]*mat2[z];
}

void multMatrixLCarre(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE][PHI_SIZE]){
	int i, j, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<1; i++){
		for(j = 0; j<PHI_SIZE; j++){
			for(z = 0; z<PHI_SIZE; z++){
				result[j] += mat1[z]*mat2[z][j];
				printf("%f\n", result[j]);
			}
		}
	}
}

void multMatrixColL(double result[PHI_SIZE][PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]){
	int i, j;
	for(i = 0; i<PHI_SIZE; i++){
		for(j = 0; j<PHI_SIZE; j++){
			result[i][j] = 0;
			result[i][j] = mat1[i]*mat2[j];
		}
	}
}

double multMatrixOneValue(double mat1[PHI_SIZE], double mat2[PHI_SIZE]){

	int i;
	double result = 0;
	for(i = 0; i<PHI_SIZE; i++){
		result += mat1[i]*mat2[i];
	}

return result;
}

void multiplicationVectorScalar(double result[PHI_SIZE], double mat[PHI_SIZE], double lambda){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		result[i] = mat[i]*lambda;
	}
}

void additionVector(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		result[i] = mat1[i]+mat2[i];
	}
}

void soustractionVector(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		result[i] = mat1[i]-mat2[i];
	}
}

void additionMatrix(double result[PHI_SIZE][PHI_SIZE], double mat1[PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE][PHI_SIZE]){

	int i, j;
	for(i = 0; i<PHI_SIZE; i++){
		for(j = 0; j<PHI_SIZE; j++){
			result[i][j] = mat1[i][j]+mat2[i][j];
		}
	}
}

void generateVect(double phi[PHI_SIZE], int X, int Y, int action){

	int ind = 0, i, j;
	double distance;
	for(i = 0; i<GRID_SIZE; i++){
		for(j = 0; j<GRID_SIZE; j++){
			ind = j*GRID_SIZE+i;
			distance = sqrt( pow((X-i)*10, 2) + pow((Y-j)*10 , 2) );
			phi[ind] = generateGaussian(VAR, ECTYPE, distance);
		}
	}
	
	switch(action){
		case NORTH: 
			phi[GRID_SIZE*GRID_SIZE] = 1;
		break;
		case EAST: 
			phi[GRID_SIZE*GRID_SIZE+1] = 1;
		break;
		case SOUTH:
			phi[GRID_SIZE*GRID_SIZE+2] = 1;
		break;
		case WEST: 
			phi[GRID_SIZE*GRID_SIZE+3] = 1;
		break;
	}
}

double dyna_MG(double theta[PHI_SIZE], double b[PHI_SIZE], double F[PHI_SIZE][PHI_SIZE], PQueue pQueue, double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS], int X, int Y, int A,  int* step_to_converge){
	
	//Declarations
	double Qold[GRID_SIZE][GRID_SIZE][NB_ACTIONS];
	int i, j, e, a, pas, diff;
	double delta, r, priority;
	double phi1[PHI_SIZE];
	double phi2[PHI_SIZE];
	double AlphaDeltaPhi1[PHI_SIZE];

	//Save the old Q-table
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){ 
				Qold[i][j][a] = Q[i][j][a];
			}
		}
	}

	for(e=0; e<NB_EPISODES; e++){
		
		//Select a random real state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;
		
		//Select an action with e-greedy policy
		A = e_greedy(X, Y, EPSILON, Q);
		
		//Generate a feature vector
		generateVect(phi1, X, Y, A);

		//Or until I find a reward
		for (pas=0; pas<NB_STEPS; pas++){

			//next real state
			int Xnext, Ynext; 
			//reward
			int r = 0;

			if ((X==RWX) && (Y==RWY)) {  
				r = REWARD_VALUE;
				Xnext = RX; 
				Ynext = RY;
			}
 			else if ((X==RW2X) && (Y==RW2Y)) {
				r = REWARD_VALUE2;
				Xnext = R2X; 
				Ynext = R2Y;
			}
			else {
				switch (A) {
					case NORTH: 
						//We cannot move
						if(X==0)  r = -1;
						else Xnext = X - 1;
					break;
					case EAST: 
						if(Y==GRID_SIZE-1)r = -1;
						else Ynext = Y + 1;
					break;
					case SOUTH:
						if(X==GRID_SIZE-1) r = -1;
						else Xnext = X + 1;
					break;
					case WEST: 
						if(Y==0) r = -1; 
						else Ynext = Y - 1;
					break;
					
				}
			}
			generateVect(phi2, Xnext, Ynext, A);

			//delta updating
			delta = r + GAMMA*multMatrixOneValue(theta, phi2)-multMatrixOneValue(theta, phi1);
		
			//theta updating
			multiplicationVectorScalar(AlphaDeltaPhi1, phi1, ALPHA*delta);
			additionVector(theta, theta, AlphaDeltaPhi1);

			//F updating
			double resultTmp[PHI_SIZE];
			double resultTmp2[PHI_SIZE];
			double resultTmp3[PHI_SIZE];
			double resultTmp4[PHI_SIZE][PHI_SIZE];
			multMatrixCarreCol(resultTmp, F, phi1);				//F*phi
			soustractionVector(resultTmp2, phi2, resultTmp);		//phi'-F*phi
			multiplicationVectorScalar(resultTmp3, resultTmp2, ALPHA);	//alpha*(phi'-F*phi)
			multMatrixColL(resultTmp4, resultTmp3, phi1);			//alpha*(phi'-F*phi)*(phi)T
			additionMatrix(F, F, resultTmp4);				//F <- F + alpha*(phi'-F*phi)*(phi)T

			//b updating
			double tmp = multMatrixOneValue(b, phi1); 			//(b)T*phi
			double lambda = ALPHA*(r-tmp);					//alpha*(r - (b)T*phi)
			double vectTmp[PHI_SIZE];
			multiplicationVectorScalar(vectTmp, phi1, lambda);		//alpha*(r - (b)T*phi)*phi
			additionVector(b, b, vectTmp);					//b <- b + alpha*(r - (b)T*phi)
		
			//Replay
			double unitBasisVect[PHI_SIZE];
			for(i=0; i<PHI_SIZE; i++){
				if(phi1[i] != 0){ //ou seuiller
					priority = abs(delta*phi1[i]);	
					PQueueE pQueueE;
					pQueueE.priority = priority;
					pQueueE.i = i;
					addElement(pQueue, pQueueE);
				}
				unitBasisVect[i] = 0;
			}

			while(pQueue != NULL){

				PQueueE head = headP(pQueue);
				int indice = head.i;
				double tmp[PHI_SIZE];
				double tmp2;
			
				//Q[][][] += ALPHA * (reward + GAMMA * bestQ(s2.X, s2.Y, Q) - Q[s.X][s.Y][action]);

				for(j=0; j<PHI_SIZE; j++){
					if(F[indice][j] != 0){
						//Initialization of the unit basis vetor ej
						unitBasisVect[j] = 1;

						multMatrixLCarre(tmp, theta, F);		//(theta)T*F
						tmp2 = multMatrixOneValue(tmp, unitBasisVect); 

						delta = b[j] + GAMMA*tmp2 - theta[j];
						
						//Updating theta
						theta[j] += ALPHA*delta;
						
						//Put j on the PQueue with priority |delta|
						PQueueE pQueueE2;
						pQueueE2.priority = abs(delta);
						pQueueE2.i = j;
						addElement(pQueue, pQueueE2);
					}
				}
			}

			int aNext = e_greedy(Xnext, Ynext, EPSILON, Q);
			X = Xnext; Y = Ynext; A = aNext;

			(*step_to_converge)++;
		}
			
	}

	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){
				diff = fabsf(Qold[i][j][a] - Q[i][j][a]);
				if (delta < diff) delta = diff;
			}
		}
	}

return delta;


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISPLAY
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayConfig(int stateX, int stateY, int grid[GRID_SIZE][GRID_SIZE]){
	int i, j;
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			
			if( (i == RWX && j == RWY) || ( (i == stateX && j == stateY) && (i == RWX && j == RWY) )  ){
				if(i == stateX && j == stateY) printf(" X   ");
				else printf(" +10 ");
			}
			else if( (i == RW2X && j == RW2Y) || ( (i == stateX && j == stateY) && (i == RW2X && j == RW2Y) )  ){
				if(i == stateX && j == stateY) printf(" X   ");
				else printf(" +6  ");
			}
			else if( (i == RX && j == RY) || ( (i == stateX && j == stateY) && (i == RX && j == RY) )  ){
				if(i == stateX && j == stateY) printf(" X   ");
				else printf(" R1  ");
			}
			else if( (i == R2X && j == R2Y) || ( (i == stateX && j == stateY) && (i == R2X && j == R2Y) )  ){
				if(i == stateX && j == stateY) printf(" X   ");
				else printf(" R2  ");
			}
			else if(i == stateX && j == stateY) printf(" X   ");
			else printf(" .   ");
		}
		printf("\n");
	}
	printf("\n\n");
}


void displayQ(double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

	int i, j, a;
	
  	for (i=0; i<GRID_SIZE; i++){
    		for (j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++) printf("%2.0f ", Q[i][j][a]);
     			printf("|");
    		}
    		printf("\n");
	}
}

void test(int X, int Y, int grid[GRID_SIZE][GRID_SIZE], double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

	int bestA = 0;

	printf("START TEST\n\n");
	printf("First state (%d, %d)\n\n", X, Y);

	while(bestA != WIN){
		bestA = bestActionForTest(X, Y, Q);
		displayConfig(X, Y, grid);
		switch (bestA) {
			case NORTH: 
				//We cannot move
				if(X==0)  printf("cannot move");
				else X = X - 1; 
			break;
			case EAST: 
				if(Y==GRID_SIZE-1)  printf("cannot move");
				else Y = Y + 1;  
			break;
			case SOUTH:
				if(X==GRID_SIZE-1)  printf("cannot move");
				else X = X + 1; 
			break;
			case WEST: 
				if(Y==0)  printf("cannot move");
				else Y = Y - 1;
				break;
				
		}
	}

	printf("Final state (%d, %d)\n", X, Y);
	printf("END TEST\n\n");
}

void displayGridDirections(double Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	int i, j, action;
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){

			if( i == RWX && j == RWY  ){
				printf(" +10 ");
			}
			else if(i == RW2X && j == RW2Y){
				printf(" +6  ");
			}
			else{
				action = bestAction(i, j, Q);
				switch(action){
					case NORTH: 
						printf(" ^   ");	
					break;
					case EAST: 
						printf(" >   ");
					break;
					case SOUTH:
						printf(" v   ");
					break;
					case WEST: 
						printf(" <   ");
					break;
				}
			}
		}
		printf("\n");
	}
	printf("\n\n");
} 
