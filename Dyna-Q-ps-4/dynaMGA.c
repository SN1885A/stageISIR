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
	int a, action = 0, size = 0;
	double tmp[PHI_SIZE];
	ListMaxAction listMaxAction = NULL;
	
	for(a=0; a<NB_ACTIONS; a++){
		op1 = multVectorOneValue2(b, phi, a);
		multMatrixLCarre(tmp, theta, F, a);
		op2 = GAMMA*multVectorOneValue(tmp, phi);
		result = op1 + op2;

		if(result>=best){
			best = result;
			action = a;
			listMaxAction = addElementListMaxAction(listMaxAction, a, best, &size);

		}
	}

	action = listMaxActionRandom(listMaxAction, size);

return action;
}

ListMaxAction bestActionForVerifPolicy(double phi[PHI_SIZE], double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){

	double best = -DBL_MAX, result, op1, op2;
	int a, action = 0, size = 0;
	double tmp[PHI_SIZE];
	ListMaxAction listMaxAction = NULL;

	for(a=0; a<NB_ACTIONS; a++){
		op1 = multVectorOneValue2(b, phi, a);
		multMatrixLCarre(tmp, theta, F, a);
		op2 = GAMMA*multVectorOneValue(tmp, phi);
		result = op1 + op2;

		if(result>=best){
			best = result;
			action = a;
			listMaxAction = addElementListMaxAction(listMaxAction, a, best, &size);

		}
	}

return listMaxAction;
}

int softmax(int x, int y, double phi[PHI_SIZE], double theta[PHI_SIZE], float e, double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){

	ListIndAction list = NULL;
	int a = 2, i;
	double tabValue[NB_ACTIONS], op1, op2, result, sum = 0, r;
	double tmp[PHI_SIZE];

	if(x==RWX && y==RWY){
		a = rand()%NB_ACTIONS;
	}

	else{

		for(i = 0; i < NB_ACTIONS; i++){
			op1 = multVectorOneValue2(b, phi, a);
			multMatrixLCarre(tmp, theta, F, a);
			op2 = GAMMA*multVectorOneValue(tmp, phi);
			tabValue[i] = op1 + op2;
			sum += tabValue[i];

		}

		for(i = 0; i < NB_ACTIONS; i++){
			result = exp(tabValue[i]/TAU)/exp(sum/TAU);
			list = addElementLIA(list, i, result);
		}

		r = (double)rand()/RAND_MAX;

		if(0 <= r && r < list->prob){
			a = list->action;
		}
		else if(list->prob <= r && r < list->next->prob){
			a = list->next->action;
		}
		else if(list->next->prob <= r && r < list->next->next->prob){
			a = list->next->next->action;
		}
		else{
			a = list->next->next->next->action;
		}

	}

printf("a = %d\n", a);
return a;
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
		//if( (X+i<1 && Y+j<1) || (abs(X-i)<1 && abs(Y-j)<1)){
			ind = i*GRID_SIZE+j;
			distance = sqrt( pow((X-i)*DISTANCE, 2) + pow((Y-j)*DISTANCE , 2) );

			if(generateGaussian(VAR, ECTYPE, distance) > 0.4){
				phi[ind] = generateGaussian(VAR, ECTYPE, distance);
			}
			else
				phi[ind] = 0;
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

void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int *episode_to_converge, int* step_to_converge){
	
	//Declarations
	PQueue pQueue;
	int i, j, e, a, pas, cpt, cptStop = 0, test = 0, step_to_converge_per_episode = 0;
	int X, Y, Xnext, Ynext, A, Anext, A2;
	float d = 0;
	double delta, r, priority, R = 0;
	double phi1[PHI_SIZE];
	double phi2[PHI_SIZE];
	double AlphaDeltaPhi1[PHI_SIZE];
	PQueueE pQueueE2;

	pQueue = createPQueue();
	double oldTheta[PHI_SIZE];

	#ifdef DEBUG
	FILE *fileStep2ConvPerEpisode = NULL;
	fileStep2ConvPerEpisode = fopen("fileStep2ConvPerEpisode.txt" ,"w+");

	FILE *fileTimeReward = NULL;
	fileTimeReward = fopen("rewardTime.txt" ,"w+");

	struct timeval start, end;
	#endif

	for(e=0; e<NB_EPISODES; e++){
		
		printf("Episode n°%d\n", e);
		cpt = 0;

		for(i=0; i<PHI_SIZE; i++)  oldTheta[i] = theta[i];

		//Select a random real state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;

		//Generate a feature vector
		generateVect(phi1, X, Y);
		
		R = 0;
		
		#ifdef DEBUG
		gettimeofday(&start, NULL);
		#endif
		
		do{

		(*step_to_converge)++;

		printf("Step to converge = %d\n", *step_to_converge);
		step_to_converge_per_episode++;

			if(cpt == 1) cpt++;
	
			//next real state
			Xnext = X;
			Ynext = Y; 

			//Select an action
			A = e_greedy(X, Y, phi1, theta, EPSILON, b, F);
			//A = softmax(X, Y, phi1, theta, EPSILON, b, F);

			//reward
			r = 0;

			if ((X==RWX) && (Y==RWY)) {  
				r = REWARD_VALUE;
				Xnext = RX; 
				Ynext = RY;
				cpt++;
			}

			else{
				switch (A) {
					case NORTH: 
						//We cannot move
						if(X==0)   r = 0;
						else Xnext = X - 1;
					break;
					case EAST: 
						if(Y==GRID_SIZE-1)r = 0;
						else Ynext = Y + 1;
					break;
					case SOUTH:
						if(X==GRID_SIZE-1) r = 0;
						else Xnext = X + 1;
					break;
					case WEST: 
						if(Y==0) r = 0;
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

			multMatrixCarreCol(resultTmp1, F, phi1, A);			        //F*phi
			soustractionVector(resultTmp2, phi2, resultTmp1);		    //phi'-F*phi
			multiplicationVectorScalar(resultTmp3, resultTmp2, ALPHA);	//alpha*(phi'-F*phi)
			multMatrixColL(resultTmp4, resultTmp3, phi1);			    //alpha*(phi'-F*phi)*(phi)T
			additionMatrix(F, resultTmp4, A);				            //F <- F + alpha*(phi'-F*phi)*(phi)T
			
			//b updating
			double tmp = multVectorOneValue2(b, phi1, A); 				//(b)T*phi
			double lambda = ALPHA*(r-tmp);					        	//alpha*(r - (b)T*phi)
			double vectTmp[PHI_SIZE];
			multiplicationVectorScalar(vectTmp, phi1, lambda);			//alpha*(r - (b)T*phi)*phi
			additionVector2(b, vectTmp, A);					        	//b <- b + alpha*(r - (b)T*phi)

			#ifdef REPLAY

			for(i=0; i<PHI_SIZE; i++){
				if(phi1[i] != 0){ //or threshold
					priority = fabs(delta*phi1[i]);
					PQueueE pQueueE;
					pQueueE.priority = priority;
					pQueueE.i = i;

					if(priority != 0 && priority < DBL_MAX)
						pQueue = addElement(pQueue, pQueueE);
				}
			}

			int p = 0;

			while(pQueue != NULL){

				if(p == NB_STEPS) break;

				PQueueE head = headP(pQueue);
				pQueue = deleteHead(pQueue);
				
				int indice = head.i;
				double resultTmp5[PHI_SIZE], tmp2;
				int exist = 0;
				
				for(j=0; j<PHI_SIZE; j++){
					for(a = 0; a<NB_ACTIONS; a++){
							if(F[a][indice][j] != 0) exist = 1;
					}
				
					if(exist == 1){
						exist = 0;

						double best = -DBL_MAX;
						double result;

						for(a=0; a<NB_ACTIONS; a++){
							multMatrixLCarre(resultTmp5, theta, F, a);
							result = b[a][j] + (GAMMA*resultTmp5[j]);
							if(result > best){
								best = result;
							}
						}

						delta = best - theta[j];

						//Updating theta

						theta[j] = theta[j] + (ALPHA*delta);

						//Put j on the PQueue with priority |delta|

						pQueueE2.priority = fabs(delta);
	
						pQueueE2.i = j;
						if(pQueueE2.priority != 0 && pQueueE2.priority < DBL_MAX)
							pQueue = addElement(pQueue, pQueueE2);
					}
				}
				p++;
			}
			#endif
			
			X = Xnext; Y = Ynext;
			generateVect(phi1, X, Y);

		}
		while(cpt != 2);


		double diff, diffMax = 0;
		for(i=0; i<PHI_SIZE; i++){
			diff = fabsf(oldTheta[i] - theta[i]);
			if (diffMax < diff) diffMax = diff;
		}

		#ifdef POLICY_VERIF
		int verif = verifPolicy(theta, b, F);
		if(verif == 0){
			(*episode_to_converge) = e;
			break;
		}
		#endif

		#ifdef THETA_CONV_VERIF
		if(diffMax < THETA_CONV){
			cptStop++;
		}
		else{
			cptStop = 0;
		}

		if(cptStop == HAVE_REALLY_CONV) break;
		#endif


		(*episode_to_converge) = e;

		#ifdef DEBUG
			fprintf(fileStep2ConvPerEpisode, "%d;%d\n", e, step_to_converge_per_episode);
			#ifdef WHILE
				gettimeofday(&end, NULL);
				fprintf(fileTimeReward, "%ld;%f\n", (end.tv_sec*1000000+end.tv_usec - (start.tv_sec*1000000+start.tv_usec)), R);
			#endif
		#endif

		step_to_converge_per_episode = 0;

	}
}

int verifPolicy(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){

	int i, j, action, wrong = 0;
	double phi[PHI_SIZE];

	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){

			if( i == RWX && j == RWY  ){

			}

			else{
				generateVect(phi, i, j);
				ListMaxAction listMaxAction = NULL;
				listMaxAction = bestActionForVerifPolicy(phi, theta, b, F);

				while(listMaxAction != NULL){

					action = listMaxAction->action;

					//Policy verif
					if(i == RWX && j != RWY){
						if(j < RWY){
							if(action != EAST) wrong++;
						}
						else if(j > RWY){
							if(action != WEST) wrong++;
						}
					}

					else if(j == RWY && i != RWX){
						if(i < RWX){
							if(action != SOUTH) wrong++;
						}
						else if(i > RWX){
							if(action != NORTH) wrong++;
						}
					}

					else if(i != RWX && j != RWY){
						if(i<RWX){
							if(j<RWY){
								if(action != SOUTH && action != EAST) wrong++;
							}
							else{
								if(action != SOUTH && action != WEST) wrong++;
							}
						}
						else{
							if(j<RWY){
								if(action != NORTH && action != EAST) wrong++;
							}
							else{
								if(action != NORTH && action != WEST) wrong++;
							}
						}

					}
					if(listMaxAction->next != NULL) listMaxAction = listMaxAction->next;
					else break;
				}
			}
		}
	}

	return wrong;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISPLAY
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void displayGridDirections(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]){
	int i, j, action, wrong = 0;
	double phi[PHI_SIZE];
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){

			if( i == RWX && j == RWY  ){
				printf(" +%0.1f",  REWARD_VALUE);
			}
			
			else{
				generateVect(phi, i, j);
				action = bestAction(phi, theta, b, F);

				//Policy verif
				if(i == RWX && j != RWY){
					if(j < RWY){
						if(action != EAST) wrong++;
					}
					else if(j > RWY){
						if(action != WEST) wrong++;
					}	
				}

				else if(j == RWY && i != RWX){
					if(i < RWX){
						if(action != SOUTH) wrong++;
					}
					else if(i > RWX){
						if(action != NORTH) wrong++;
					}	
				}

				else if(i != RWX && j != RWY){
					if(i<RWX){
						if(j<RWY){
							if(action != SOUTH && action != EAST) wrong++;
						}
						else{
							if(action != SOUTH && action != WEST) wrong++;
						}
					}
					else{
						if(j<RWY){
							if(action != NORTH && action != EAST) wrong++;
						}
						else{
							if(action != NORTH && action != WEST) wrong++;
						}
					}

				} 

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
	printf("Policy error percent = %d\n", wrong);
} 
