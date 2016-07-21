#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dynaMGA.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int bestAction(int i, int j, double b[NB_ACTIONS][PHI_SIZE]){

	double qm = -1000.0, result;
	int a, z, action = 0;
	double phi[PHI_SIZE];
  	for (a=0; a<NB_ACTIONS; a++){
		generateVect(phi, i, j);
		result = multVectorOneValue2(b, phi, a); 
    		if (qm<result){
      			qm = result;
      			action = a;
   		}
	}

return action;
}

int e_greedy(int x, int y, float e,double b[NB_ACTIONS][PHI_SIZE]){

	int a = 0;
	double r = (double)rand()/RAND_MAX;
	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action
	if( (x==RWX && y==RWY) || (x==RW2X && y==RW2Y) || (r < e) ) a = rand()%NB_ACTIONS;
	else a = bestAction(x, y, b);
	
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

void multMatrixCarreCol(double result[PHI_SIZE], double mat1[NB_ACTIONS][PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE], int action){

	int i, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<PHI_SIZE; i++)
			for(z = 0; z<PHI_SIZE; z++)
				result[i] += mat1[action][i][z]*mat2[z];
}

void multMatrixLCarre(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int action){
	int i, j, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(j = 0; j<PHI_SIZE; j++){
		for(z = 0; z<PHI_SIZE; z++){
			result[j] += mat1[z]*mat2[action][z][j];
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

double multVectorOneValue(double mat1[PHI_SIZE], double mat2[PHI_SIZE]){

	int i;
	double result = 0;
	for(i = 0; i<PHI_SIZE; i++){
		result += mat1[i]*mat2[i];
	}

return result;
}

double multVectorOneValue2(double mat1[NB_ACTIONS][PHI_SIZE], double mat2[PHI_SIZE], int action){

	int i;
	double result = 0;
	for(i = 0; i<PHI_SIZE; i++){
		result += mat1[action][i]*mat2[i];
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

void additionVector2(double mat1[NB_ACTIONS][PHI_SIZE], double mat2[PHI_SIZE], int action){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		mat1[action][i] = mat1[action][i]+mat2[i];
	}
}

void soustractionVector(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		result[i] = mat1[i]-mat2[i];
	}
}

void additionMatrix(double mat1[NB_ACTIONS][PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE][PHI_SIZE], int action){

	int i, j;
	for(i = 0; i<PHI_SIZE; i++){
		for(j = 0; j<PHI_SIZE; j++){
			mat1[action][i][j]= mat1[action][i][j]+mat2[i][j];
		}
	}
}

void generateVect(double phi[PHI_SIZE], int X, int Y){

	int ind = 0, i, j;
	double distance;
	for(i = 0; i<PHI_SIZE; i++) phi[i] = 0;
	for(i = 0; i<GRID_SIZE; i++){
		for(j = 0; j<GRID_SIZE; j++){
			ind = j*GRID_SIZE+i;
			distance = sqrt( pow((X-i)*DISTANCE, 2) + pow((Y-j)*DISTANCE , 2) );
			phi[ind] = generateGaussian(VAR, ECTYPE, distance);
		}
	}
}

void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int *step_to_converge){
	
	//Declarations
	PQueue pQueue;
	int i, j, e, a, pas, X, Y, A, Xnext, Ynext, A2, Anext;
	float d = 0, diff = 0;
	double delta, r, priority;
	double phi1[PHI_SIZE];
	double phi2[PHI_SIZE];
	double AlphaDeltaPhi1[PHI_SIZE];

	pQueue = createPQueue();

	for(e=0; e<NB_EPISODES; e++){
		
		//Select a random real state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;
		
		//Generate a feature vector
		generateVect(phi1, X, Y);

		//Select an action 
		double best = -1000;
		double result;
		double tmp[PHI_SIZE];
		double op1, op2;

		//Or until I find a reward
		for (pas=0; pas<NB_STEPS; pas++){

			//next real state
			Xnext = X;
			Ynext = Y; 

			for(a=0; a<NB_ACTIONS; a++){
				op1 = multVectorOneValue2(b, phi1, a);
				multMatrixCarreCol(tmp, F, phi1, a);
				op2 = GAMMA*multVectorOneValue(theta, tmp);
				result = op1 + op2;
				if(result>best){
					best = result;
					A = a;
				}	
			}

			//reward
			r = 0;

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
			
			generateVect(phi2, Xnext, Ynext);

			//delta updating
			delta = r + ( GAMMA*multVectorOneValue(theta, phi2) ) - multVectorOneValue(theta, phi1);

			//theta updating
			multiplicationVectorScalar(AlphaDeltaPhi1, phi1, (ALPHA*delta));
			additionVector(theta, theta, AlphaDeltaPhi1);

			//F updating
			double resultTmp1[PHI_SIZE];
			double resultTmp2[PHI_SIZE];
			double resultTmp3[PHI_SIZE];
			double resultTmp4[PHI_SIZE][PHI_SIZE];

			multMatrixCarreCol(resultTmp1, F, phi1, A);			//F*phi
			soustractionVector(resultTmp2, phi2, resultTmp1);		//phi'-F*phi
			multiplicationVectorScalar(resultTmp3, resultTmp2, ALPHA);	//alpha*(phi'-F*phi)
			multMatrixColL(resultTmp4, resultTmp3, phi1);			//alpha*(phi'-F*phi)*(phi)T
			additionMatrix(F, resultTmp4, A);				//F <- F + alpha*(phi'-F*phi)*(phi)T
			
			//b updating
			double tmp = multVectorOneValue2(b, phi1, A); 			//(b)T*phi
			double lambda = ALPHA*(r-tmp);					//alpha*(r - (b)T*phi)
			double vectTmp[PHI_SIZE];

			multiplicationVectorScalar(vectTmp, phi1, lambda);		//alpha*(r - (b)T*phi)*phi
			additionVector2(b, vectTmp, A);					//b <- b + alpha*(r - (b)T*phi)

			//Replay
			int cpt = 0;
			double unitBasisVect[PHI_SIZE];

			for(i=0; i<PHI_SIZE; i++){
				if(phi1[i] > 0.03){ //ou seuiller
					priority = abs(delta*phi1[i]);	
					PQueueE pQueueE;
					pQueueE.priority = priority;
					pQueueE.i = i;
					pQueue = addElement(pQueue, pQueueE);
					cpt++;
				}
				unitBasisVect[i] = 0;
				
			}
			//printf("Pqueue has been fill at %d\n", cpt);
			int p = 0;
			while(pQueue != NULL){
				p++;
				if(p == 20) break;

				PQueueE head = headP(pQueue);
				pQueue = deleteHead(pQueue);
				cpt--;
				
				int indice = head.i;
				double resultTmp5[PHI_SIZE];
				double tmp2;
				for(a = 0; a<NB_ACTIONS; a++){
					for(j=0; j<PHI_SIZE; j++){
						if(F[a][indice][j] != 0){
							int m;
							double best2 = -1000;
							for(m=0; m<NB_ACTIONS; m++){
								//Initialization of the unit basis vetor ej
								unitBasisVect[j] = 1;
								multMatrixLCarre(resultTmp5, theta, F, m);
								tmp2 = multVectorOneValue(resultTmp5, unitBasisVect);
								unitBasisVect[j] = 0; 
								result = b[m][j] + GAMMA*tmp2 - theta[j];

								if(result>best) best2 = result;
							}

							delta = best2;
						
							//Updating theta
							theta[j] += ALPHA*delta;
						
							//Put j on the PQueue with priority |delta|
							PQueueE pQueueE2;
							pQueueE2.priority = abs(delta);
							pQueueE2.i = j;
							pQueue = addElement(pQueue, pQueueE2);
							cpt++;
						}
					}
				}
			}

			X = Xnext; Y = Ynext;
			generateVect(phi1, X, Y);

			(*step_to_converge)++;
		}
			
	}
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


void displayGridDirections(double b[NB_ACTIONS][PHI_SIZE]){
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
				action = bestAction(i, j, b);
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
