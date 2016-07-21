#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dynaMG.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int bestAction(int i, int j, double theta[PHI_SIZE]){

	double qm = -1000.0, result;
	int a, z, action = 0;
	double phi[PHI_SIZE];
	
  	for (a=0; a<NB_ACTIONS; a++){
		generateVect(phi, i, j, a);
		result = multMatrixOneValue(phi, theta); 
    		if (qm<result){
      			qm = result;
      			action = a;
   		}
	}

return action;
}

int e_greedy(int x, int y, float e, double theta[PHI_SIZE]){

	int a = 0;
	double r = (double)rand()/RAND_MAX;
	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action
	if( (x==RWX && y==RWY) || (x==RW2X && y==RW2Y) || (r < e) ) a = rand()%NB_ACTIONS;
	else a = bestAction(x, y, theta);
	
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

	int i, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<PHI_SIZE; i++)
			for(z = 0; z<PHI_SIZE; z++)
				result[i] += mat1[i][z]*mat2[z];
}

void multMatrixLCarre(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE][PHI_SIZE]){
	int i, j, z;
	for(i=0; i<PHI_SIZE; i++) result[i] = 0;
	for(j = 0; j<PHI_SIZE; j++){
		for(z = 0; z<PHI_SIZE; z++){
			result[j] += mat1[z]*mat2[z][j];
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
	for(i = 0; i<PHI_SIZE; i++) phi[i] = 0;
	for(i = 0; i<GRID_SIZE; i++){
		for(j = 0; j<GRID_SIZE; j++){
			ind = j*GRID_SIZE+i;
			distance = sqrt( pow((X-i)*DISTANCE, 2) + pow((Y-j)*DISTANCE , 2) );
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

void dyna_MG(double theta[PHI_SIZE], double b[PHI_SIZE], double F[PHI_SIZE][PHI_SIZE], int* step_to_converge){
	
	//Declarations
	PQueue pQueue;
	int i, j, e, a, pas, X, Y, A, Xnext, Ynext, A2;
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
		
		//Select an action with e-greedy policy
		A = e_greedy(X, Y, EPSILON, theta);
		
		//Generate a feature vector
		generateVect(phi1, X, Y, A);
		
		//Or until I find a reward
		for (pas=0; pas<NB_STEPS; pas++){

			//printf("Etat = (%d, %d)\n", X, Y);
			//printf("Action = %d\n", A);

			//next real state
			Xnext = X;
			Ynext = Y; 

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
			//printf("Etat suivant = (%d, %d)\n", Xnext, Ynext);
			A2 = e_greedy(Xnext, Ynext, 0, theta);
			generateVect(phi2, Xnext, Ynext, A);

			//delta updating
			delta = r + ( GAMMA*multMatrixOneValue(theta, phi2) ) - multMatrixOneValue(theta, phi1);
			//printf("Delta = %f\n", delta);

			//theta updating
			multiplicationVectorScalar(AlphaDeltaPhi1, phi1, (ALPHA*delta));
			additionVector(theta, theta, AlphaDeltaPhi1);
			//printf("Theta has been updated\n");

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
			//printf("F has been updated\n");

			/*for(i=0; i<PHI_SIZE; i++){
				for(j=0; j<PHI_SIZE; j++){
					printf("%f    ", F[i][j]);
				}
				printf("\n");
			}*/
		
			//b updating
			double tmp = multMatrixOneValue(b, phi1); 			//(b)T*phi
			double lambda = ALPHA*(r-tmp);					//alpha*(r - (b)T*phi)
			double vectTmp[PHI_SIZE];
			multiplicationVectorScalar(vectTmp, phi1, lambda);		//alpha*(r - (b)T*phi)*phi
			additionVector(b, b, vectTmp);					//b <- b + alpha*(r - (b)T*phi)
			//printf("b has been updated\n");

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
				//printf("Pqueue actual size is %d\n", cpt);
				//usleep(50000);
				int indice = head.i;
				double tmp[PHI_SIZE];
				double tmp2;

				for(j=0; j<PHI_SIZE; j++){
					if(F[indice][j] != 0){
						//Initialization of the unit basis vetor ej
						unitBasisVect[j] = 1;

						multMatrixLCarre(tmp, theta, F);		//(theta)T*F
						tmp2 = multMatrixOneValue(tmp, unitBasisVect);
						unitBasisVect[j] = 0; 
						delta = b[j] + GAMMA*tmp2 - theta[j];
						
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
			
			int aNext = e_greedy(Xnext, Ynext, EPSILON, theta);
			X = Xnext; Y = Ynext; A = aNext;
			generateVect(phi1, X, Y, A);

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


void displayGridDirections(double theta[PHI_SIZE]){
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
				action = bestAction(i, j, theta);
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