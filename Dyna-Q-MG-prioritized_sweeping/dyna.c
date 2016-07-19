#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dyna.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////

float bestQ(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
  float qm = -1000;
  int a;
  for (a=0; a<NB_ACTIONS; a++) 
    if (qm<Q[i][j][a]) {
      qm = Q[i][j][a];
    }
  return qm;
}

int bestAction(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	float qm = -1000;
	int a, action = 0;
  	for (a=0; a<NB_ACTIONS; a++) 
    		if (qm<Q[i][j][a]){
      			qm = Q[i][j][a];
      			action = a;
   		}
return action;
}

int bestActionForTest(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
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

int e_greedy(int x, int y, float e, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

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

void multiplicationMatrix(double result[VECT_SIZE], double mat1[VECT_SIZE][VECT_SIZE], double mat2[VECT_SIZE][1], int mat1L, int mat1C, int mat2C){

	int i, j, z;
	for(i=0; i<VECT_SIZE; i++) result[i] = 0;
	for(i = 0; i<mat1L; i++)
		for(j = 0; j<mat2C; j++)
			for(z = 0; z<mat1C; z++)
				result[i] += mat1[i][z]*mat2[z][j];
}

void multiplicationMatrix2(double result[VECT_SIZE], double mat1[1][VECT_SIZE], double mat2[VECT_SIZE][VECT_SIZE], int mat1L, int mat1C, int mat2C){
	int i, j, z;
	for(i=0; i<VECT_SIZE; i++) result[i] = 0;
	for(i = 0; i<mat1L; i++){
		for(j = 0; j<mat2C; j++){
			for(z = 0; z<mat1C; z++){
				result[j] += mat1[i][z]*mat2[z][j];
				
			}
		}
	}
}

double multiplicationMatrixOneValue(double mat1[VECT_SIZE], double mat2[VECT_SIZE]){

	int i;
	double result = 0;
	for(i = 0; i<VECT_SIZE; i++){
		result += mat1[i]*mat2[i];
	}

return result;
}

void multiplicationMatrixScalar(double result[VECT_SIZE], double mat[VECT_SIZE], double lambda){

	int i;
	for(i = 0; i<VECT_SIZE; i++){
		result[i] = mat[i]*lambda;
	}
}

void additionMatrix(double result[VECT_SIZE], double mat1[VECT_SIZE], double mat2[VECT_SIZE]){

	int i;
	for(i = 0; i<VECT_SIZE; i++){
		result[i] = mat1[i]+mat2[i];
	}
}

void generateVect(double vect[VECT_SIZE], int X, int Y, int action){

	int ind = 0, i, j;
	double distance;
	for(i = 0; i<GRID_SIZE; i++){
		for(j = 0; j<GRID_SIZE; j++){
			ind = j*GRID_SIZE+i;
			distance = sqrt( pow((X-i)*10, 2) + pow((Y-j)*10 , 2) );
			vect[ind] = generateGaussian(VAR, ECTYPE, distance);
		}
	}
	
	switch(action){
		case NORTH: 
			vect[GRID_SIZE*GRID_SIZE] = 1;
		break;
		case EAST: 
			vect[GRID_SIZE*GRID_SIZE+1] = 1;
		break;
		case SOUTH:
			vect[GRID_SIZE*GRID_SIZE+2] = 1;
		break;
		case WEST: 
			vect[GRID_SIZE*GRID_SIZE+3] = 1;
		break;
	}
}

DynaQReturn dyna_MG(double theta[VECT_SIZE], double b[VECT_SIZE], double F[VECT_SIZE][VECT_SIZE], PQueue pQueue, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS], int X, int Y, int A,  int* step_to_converge){

	//float Qold[GRID_SIZE][GRID_SIZE][NB_ACTIONS];
	int i, j, a, diff;
	double delta, r, priority;
	DynaQReturn dynaQReturn;
	//Save the old Q-table
	/*for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){ 
				Qold[i][j][a] = Q[i][j][a];
			}
		}
	}*/

	for(i=0; i<NB_EPISODES; i++){
		
		//Select a random state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;

		double featureState1[VECT_SIZE];
		double featureState1AlphaDelta[VECT_SIZE];
		double featureState2[VECT_SIZE];

		//Select an action with e-greedy policy
		A = e_greedy(X, Y, EPSILON, Q);
		
		//Generate a feature vector;
		generateVect(featureState1, X, Y, A);

		//Or until I find a reward
		for (pas=0; pas<NB_STEPS; pas++){
			

			//delta updating
			multiplicationMatrix(featureState2, F, featureState1, 2, 2, 1);
			r = multiplicationMatrixOneValue(featureState1, b); 
			delta = r + GAMMA*multiplicationMatrixOneValue(theta, featureState2)-multiplicationMatrixOneValue(theta, featureState1);
		
			//theta updating
			multiplicationMatrixScalar(featureState1AlphaDelta, featureState1, ALPHA*delta);
			additionMatrix(theta, theta, featureState1AlphaDelta);

			//b updating
			double tmp = multiplicationMatrixOneValue(b, featureState1); 
			double lambda = ALPHA*(r-tmp);
			double vectTmp[VECT_SIZE];
			double unitBasisVect[VECT_SIZE];
			multiplicationMatrixScalar(vectTmp, featureState1, lambda);
			additionMatrix(b, b, vectTmp);

			for(i=0; i<VECT_SIZE; i++){
				if(featureState1 != 0){ //ou seuiller
					priority = abs(delta*featureState1[i]);	
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
				double tmp[VECT_SIZE];
				double tmp2;
			
				//Q[][][] += ALPHA * (reward + GAMMA * bestQ(s2.X, s2.Y, Q) - Q[s.X][s.Y][action]);

				for(j=0; j<VECT_SIZE; j++){
					if(F[indice][j] != 0){
						unitBasisVect[j] = 1;
						multiplicationMatrix2(tmp, theta, F, 1, VECT_SIZE, VECT_SIZE);
						tmp2 = multiplicationMatrixOneValue(tmp, unitBasisVect); 
						delta = b[j] + GAMMA*tmp2 - theta[j];
						theta[j] += ALPHA*delta;
						PQueueE pQueueE2;
						pQueueE2.priority = abs(delta);
						pQueueE2.i = j;
						addElement(pQueue, pQueueE2);
					}
				}
			}
		}
			
	}

	/*for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){
				diff = fabsf(Qold[i][j][a] - Q[i][j][a]);
				if (delta < diff) delta = diff;
			}
		}
	}*/

//dynaQReturn.delta = delta;
//dynaQReturn.pQueue = pQueue;

return dynaQReturn;


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


void displayQ(float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

	int i, j, a;
	
  	for (i=0; i<GRID_SIZE; i++){
    		for (j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++) printf("%2.0f ", Q[i][j][a]);
     			printf("|");
    		}
    		printf("\n");
	}
}

void test(int X, int Y, int grid[GRID_SIZE][GRID_SIZE], float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

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

void displayGridDirections(float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
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
