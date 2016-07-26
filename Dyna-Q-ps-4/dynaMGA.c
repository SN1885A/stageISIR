#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "dynaMGA.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int bestAction(double phi[PHI_SIZE], double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){

	double best = -DBL_MAX, result, op1, op2;
	int a, action = 0;
	double tmp[PHI_SIZE];
	
	for(a=0; a<NB_ACTIONS; a++){
		op1 = multVectorOneValue2(b, phi, a);
		multMatrixCarreCol(tmp, F, phi, a);
		op2 = GAMMA*multVectorOneValue(theta, tmp);
		result = op1 + op2;
		if(result>best){
			best = result;
			action = a;
		}	
	}

return action;
}

int e_greedy(int x, int y, double phi[PHI_SIZE], double theta[PHI_SIZE], float e, double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){

	int a = 0;
	double r = (double)rand()/RAND_MAX;
	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action
	//if( (x==RWX && y==RWY) || (x==RW2X && y==RW2Y) || (r < e) ) a = rand()%NB_ACTIONS;
	if( (x==RWX && y==RWY) || (r < e) ) a = rand()%NB_ACTIONS;
	else a = bestAction(phi,theta, b, F);
	
return a;
}

void multMatrixCarreCol(double result[PHI_SIZE], double mat1[NB_ACTIONS][PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE], int action){

	int i, j;
	for(i = 0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<PHI_SIZE; i++)
			for(j = 0; j<PHI_SIZE; j++)
				result[i] += mat1[action][i][j]*mat2[j];
}

void multMatrixLCarre(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int action){

	int i, j, z;
	for(i = 0; i<PHI_SIZE; i++) result[i] = 0;
	for(i = 0; i<PHI_SIZE; i++){
		for(j = 0; j<PHI_SIZE; j++){
			result[i] += mat1[j]*mat2[action][j][i];
		}
	}
	
}

void multMatrixColL(double result[PHI_SIZE][PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]){
	int i, j;
	for(i = 0; i<PHI_SIZE; i++){
		for(j = 0; j<PHI_SIZE; j++){
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
	double result2[PHI_SIZE];
	for(i = 0; i<PHI_SIZE; i++){
		mat1[i] += mat2[i];
	}
}

void additionVector2(double mat1[NB_ACTIONS][PHI_SIZE], double mat2[PHI_SIZE], int action){

	int i;
	for(i = 0; i<PHI_SIZE; i++){
		mat1[action][i] += mat2[i];
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
			mat1[action][i][j]  += mat2[i][j];
		}
	}
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

void generateVect(double phi[PHI_SIZE], int X, int Y){

	int ind = 0, i, j;
	double distance;
	for(i = 0; i<PHI_SIZE; i++) phi[i] = 0;
	for(i = 0; i<GRID_SIZE; i++){
		for(j = 0; j<GRID_SIZE; j++){
			if( (X+i<2 && Y+j<2) || (abs(X-i)<2 && abs(Y-j)<2)){
				ind = i*GRID_SIZE+j;
				distance = sqrt( pow((X-i)*DISTANCE, 2) + pow((Y-j)*DISTANCE , 2) );
				phi[ind] = generateGaussian(VAR, ECTYPE, distance);
			}
		}
	}
	ind = X*GRID_SIZE+Y;
	phi[ind] = 1;
}

void normalize(double phi[PHI_SIZE]){

	int i;
	double max = -DBL_MAX;
	for(i = 0; i<PHI_SIZE; i++){
		if(fabs(phi[i])>max)
			max = fabs(phi[i]);
	}
	for(i = 0; i<PHI_SIZE; i++){
		phi[i]/=max;
	}
}

void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int *step_to_converge){
	
	//Declarations
	PQueue pQueue;
	int i, j, e, a, pas, cpt, episode_cpt = 0;
	int X, Y, Xnext, Ynext, A, Anext, A2;
	float d = 0, diff = 0;
	double delta, r, priority, R = 0;
	double phi1[PHI_SIZE];
	double phi2[PHI_SIZE];
	double AlphaDeltaPhi1[PHI_SIZE];

	pQueue = createPQueue();
	double oldTheta[PHI_SIZE];

	FILE *file = NULL;
	file = fopen("reward.txt" ,"w+");

	FILE *file2 = NULL;
	file2 = fopen("delta.txt" ,"w+");

	for(e=0; e<NB_EPISODES; e++){
		
		cpt = 0;
		episode_cpt++;

		for(i=0; i<PHI_SIZE; i++)  oldTheta[i] = theta[i];

		//Select a random real state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;
		
		//Generate a feature vector
		generateVect(phi1, X, Y);
		
		int it;
		R = 0;
		//do{
		for(it = 0; it<30; it++){
			if(cpt == 1) cpt++;
	
			//next real state
			Xnext = X;
			Ynext = Y; 
			
			//Select an action
			A = e_greedy(X, Y, phi1, theta, EPSILON, b, F);
			
			//reward
			r = 0;

			if ((X==RWX) && (Y==RWY)) {  
				r = REWARD_VALUE;
				Xnext = RX; 
				Ynext = RY;
				cpt++;
			}
 			/*else if ((X==RW2X) && (Y==RW2Y)) {
				r = REWARD_VALUE2;
				Xnext = R2X; 
				Ynext = R2Y;
			}*/
			else {
				switch (A) {
					case NORTH: 
						//We cannot move
						if(X==0)  r = -0.1;
						else Xnext = X - 1;
					break;
					case EAST: 
						if(Y==GRID_SIZE-1)r = -0.1;
						else Ynext = Y + 1;
					break;
					case SOUTH:
						if(X==GRID_SIZE-1) r = -0.1;
						else Xnext = X + 1;
					break;
					case WEST: 
						if(Y==0) r = -0.1;
						else Ynext = Y - 1;
					break;
					
				}
			}
			
			R += r;
			generateVect(phi2, Xnext, Ynext);

			//delta updating
			delta = r + GAMMA*multVectorOneValue(theta, phi2) - multVectorOneValue(theta, phi1);

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
			double unitBasisVect[PHI_SIZE];

			for(i=0; i<PHI_SIZE; i++){
				if(phi1[i] != 0){ //ou seuiller
					priority = abs(delta*phi1[i]);	
					PQueueE pQueueE;
					pQueueE.priority = priority;
					pQueueE.i = i;
					pQueue = addElement(pQueue, pQueueE);
				}
				unitBasisVect[i] = 0;
				
			}
			
			int p = 0;
			while(pQueue != NULL){

				p++;
				if(p == NB_STEPS) break;

				PQueueE head = headP(pQueue);
				pQueue = deleteHead(pQueue);
				
				int indice = head.i;
				double resultTmp5[PHI_SIZE];
				double tmp2;
				for(a = 0; a<NB_ACTIONS; a++){
					for(j=0; j<PHI_SIZE; j++){
						if(F[a][indice][j] != 0){
							int m;
							double best = -DBL_MAX;
							double result;
							for(m=0; m<NB_ACTIONS; m++){
								//Initialization of the unit basis vetor ej
								unitBasisVect[j] = 1;
								multMatrixLCarre(resultTmp5, theta, F, m);
								tmp2 = multVectorOneValue(resultTmp5, unitBasisVect);
								
								unitBasisVect[j] = 0; 
								result = (b[m][j] + GAMMA*tmp2) - theta[j];
			
								if(result>best) best = result;
							}
							
							delta = best;
							
							//Updating theta
							theta[j] += ALPHA*delta;
							
							//Put j on the PQueue with priority |delta|
							PQueueE pQueueE2;
							pQueueE2.priority = abs(delta);
							pQueueE2.i = j;
							pQueue = addElement(pQueue, pQueueE2);
							
						}
					}
				}
			}
			
			X = Xnext; Y = Ynext;
			generateVect(phi1, X, Y);

			(*step_to_converge)++;
		}
		//while(cpt != 2);
		
		double diff, diffMax = 0;
		for(i=0; i<PHI_SIZE; i++){
			diff = fabsf(oldTheta[i] - theta[i]);
			if (diffMax < diff) diffMax = diff;
			
		}
		fprintf(file, "%f\n", R);
		printf("episode %d\n", e);
		//printf("diffmax %f\n", diffMax);
		if(diffMax < THETA_CONV){
			printf("break : %d\n", episode_cpt);
			break;
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISPLAY
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayGridDirections(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){
	int i, j, action;
	double phi[PHI_SIZE];
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){

			if( i == RWX && j == RWY  ){
				printf(" +%0.1f",  REWARD_VALUE);
			}
			/*else if(i == RW2X && j == RW2Y){
				printf(" +%d  ",  REWARD_VALUE2);
			}*/
			else{
				generateVect(phi, i, j);
				action = bestAction(phi, theta, b, F);
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
	printf("\n");
} 
