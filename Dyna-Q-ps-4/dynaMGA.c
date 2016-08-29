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

int bestAction(double* phi, double* theta, double** b, double*** F) {

	double best = -DBL_MAX, result, op1, op2;
	int a, action = 0, size = 0;
	double* tmp = (double*) malloc(PHI_SIZE * sizeof(double));
	ListMaxAction listMaxAction = NULL;

	for (a = 0; a < NB_ACTIONS; a++) {
		op1 = multVectorOneValue2(b, phi, a);
		multMatrixLCarre(tmp, theta, F, a);
		op2 = GAMMA * multVectorOneValue(tmp, phi);
		result = op1 + op2;

		if (result >= best) {
			best = result;
			listMaxAction = addElementListMaxAction(listMaxAction, a, best, &size);

		}
	}

	action = listMaxActionRandom(listMaxAction, size);
	suppListMaxAction(listMaxAction);
	free(tmp);

	return action;
}

ListMaxAction bestActionForVerifPolicy(double* phi, double *theta, double** b, double*** F) {
	double best = -DBL_MAX, result, op1, op2;
	int a, action = 0, size = 0;
	double* tmp = (double*) malloc(PHI_SIZE * sizeof(double));
	ListMaxAction listMaxAction = NULL;

	for (a = 0; a < NB_ACTIONS; a++) {
		op1 = multVectorOneValue2(b, phi, a);
		multMatrixLCarre(tmp, theta, F, a);
		op2 = GAMMA * multVectorOneValue(tmp, phi);
		result = op1 + op2;

		if (result >= best) {
			best = result;
			action = a;
			listMaxAction = addElementListMaxAction(listMaxAction, a, best,
					&size);

		}
	}

	return listMaxAction;
}

int softmax(int x, int y, double* phi, double* theta, double** b, double*** F){

 ListIndAction list = NULL, subList1 = NULL, subList2 = NULL, subList3 = NULL, subList4 = NULL;
 int a = 2, i, size1 = 0, size2 = 0, size3 = 0, size4 = 0;
 double op1, op2, result, sum = 0, r;
 double* tmp = (double*) calloc(PHI_SIZE, sizeof(double));
 double* tabValue = (double*) malloc(NB_ACTIONS * sizeof(double));

 if(x==RWX && y==RWY)
	 a = rand()%NB_ACTIONS;

 else{

	 //Compute e(Q(a)*BETA)
	 for(i = 0; i < NB_ACTIONS; i++){
		 op1 = multVectorOneValue2(b, phi, i);
		 multMatrixLCarre(tmp, theta, F, i);
		 op2 = GAMMA*multVectorOneValue(tmp, phi);
		 tabValue[i] = exp((op1 + op2)*BETA);
	 }

	 for(i = 0; i<NB_ACTIONS; i++){
		 sum += tabValue[i];
	 }

	 for(i = 0; i < NB_ACTIONS; i++){
		 result = tabValue[i]/sum;
		 list = addElementLIA(list, i, result);
	 }

	 r = (double)rand()/RAND_MAX;

	 double sum1 = list->prob+list->next->prob;
	 double sum2 = sum1+list->next->next->prob;
	 double sum3 = sum2+list->next->next->next->prob;

	 if(0 <= r && r <= list->prob){

		 subList1 = findSameProbLIA(list , list->prob, &size1);
		 if(size1 != 0){
			 a = listActionRandomLIA(subList1, size1);
		 }
		 else
			 a = list->action;
	 }
	 else if(list->prob < r && r <= sum1){

		 subList2 = findSameProbLIA(list , list->next->prob, &size2);
		 if(size2 != 0){
			 a = listActionRandomLIA(subList2, size2);
		 }
		 else
			 a = list->next->action;
	 }
	 else if(sum1 < r && r <= sum2){

		 subList3 = findSameProbLIA(list, list->next->next->prob, &size3);
		 if(size3 != 0){
			 a = listActionRandomLIA(subList3, size3);
		 }
		 else
			 a = list->next->next->action;
	 }
	 else if(sum2 < r && r <= sum3){
		subList4 = findSameProbLIA(list , list->next->next->next->prob, &size4);
		 if(size4 != 0){
			 a = listActionRandomLIA(subList4, size4);
		 }
		 else
			 a = list->next->next->next->action;
	 }

	/* printf("State = (%d, %d)\n", x, y);
	 printf("prob0 = %f action = %d\n", list->prob, list->action);
	 printf("prob1 = %f action = %d\n", list->next->prob, list->next->action);
	 printf("prob3 = %f action = %d\n", list->next->next->prob, list->next->next->action);
	 printf("prob4 = %f action = %d\n", list->next->next->next->prob, list->next->next->next->action);

	 printf("r = %f\n", r);
	 printf("a = %d\n\n", a);*/

 }

 //printf("recompense a = %d\n\n", a);

 suppLIA(subList1);
 suppLIA(subList2);
 suppLIA(subList3);
 suppLIA(subList4);
 suppLIA(list);
 free(tmp);
 free(tabValue);

 return a;
 }

int e_greedy(int x, int y, double* phi, double* theta, float e, double** b,
		double*** F) {

	int a = 0;
	double r = (double) rand() / RAND_MAX;

	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action

	if ((x == RWX && y == RWY) || (r < e))
		a = rand() % NB_ACTIONS;
	else
		a = bestAction(phi, theta, b, F);

	return a;
}

void multMatrixCarreCol(double* result, double*** mat1, double* mat2,
		int action) {

	int i, j;
	for(i = 0; i<PHI_SIZE; i++){ result[i] = 0;}
	for (i = 0; i < PHI_SIZE; i++)
		for (j = 0; j < PHI_SIZE; j++)
			result[i] += mat1[action][i][j] * mat2[j];

}

void multMatrixLCarre(double* result, double* mat1, double*** mat2, int action) {

	int i, j, z;
	for (i = 0; i < PHI_SIZE; i++)
		result[i] = 0;
	for (i = 0; i < PHI_SIZE; i++) {
		for (j = 0; j < PHI_SIZE; j++) {
			result[i] += mat1[j] * mat2[action][j][i];
		}
	}

}

void multMatrixColL(double** result, double* mat1, double* mat2) {
	int i, j;
	for (i = 0; i < PHI_SIZE; i++) {
		for (j = 0; j < PHI_SIZE; j++) {
			result[i][j] = mat1[i] * mat2[j];
		}
	}
}

double multVectorOneValue(double* mat1, double* mat2) {

	int i;
	double result = 0;
	for (i = 0; i < PHI_SIZE; i++) {
		result += mat1[i] * mat2[i];
	}

	return result;
}

double multVectorOneValue2(double** mat1, double* mat2, int action) {

	int i;
	double result = 0;
	for (i = 0; i < PHI_SIZE; i++) {
		result += mat1[action][i] * mat2[i];
	}

	return result;
}

void multiplicationVectorScalar(double* result, double* mat, double lambda) {

	int i;
	for (i = 0; i < PHI_SIZE; i++) {
		result[i] = mat[i] * lambda;
	}
}

void additionVector(double* mat1, double* mat2) {

	int i;
	for (i = 0; i < PHI_SIZE; i++) {
		mat1[i] += mat2[i];
	}
}

void additionVector2(double** mat1, double* mat2, int action) {

	int i;
	for (i = 0; i < PHI_SIZE; i++) {
		mat1[action][i] += mat2[i];
	}
}

void soustractionVector(double* result, double* mat1, double* mat2) {
	int i;
	for (i = 0; i < PHI_SIZE; i++) {
		result[i] = mat1[i] - mat2[i];
	}
}

void additionMatrix(double*** mat1, double** mat2, int action) {

	int i, j;
	for (i = 0; i < PHI_SIZE; i++) {
		for (j = 0; j < PHI_SIZE; j++) {
			mat1[action][i][j] += mat2[i][j];
		}
	}
}

double** initMatrix(int dim1, int dim2) {
	double** p = (double**) malloc(dim1 * sizeof(double*));
	int i;
	for (i = 0; i < dim1; i++) {
		p[i] = (double*) calloc(dim2, sizeof(double));
	}

	return p;
}

void freeMatrix(double** mat, int dim1, int dim2) {
	int i, j;
	for (i = 0; i < dim1; i++) {
		free(mat[i]);
	}
	free(mat);
}

double*** initCube(int dim1, int dim2, int dim3) {
	double*** p = (double***) malloc(dim1 * sizeof(double**));
	int i;
	for (i = 0; i < dim1; i++) {
		p[i] = initMatrix(dim2, dim3);
	}
	return p;
}

void freeCube(double*** mat, int dim1, int dim2, int dim3) {
	int i, j;
	for (i = 0; i < dim1; i++) {
		freeMatrix(mat[i], dim2, dim3);
	}
	free(mat);
}

double generateGaussian(int var, int ectype, double d) {

	double fact1 = 1 / (sqrt((M_PI * 2) * ectype));
	double div1, div2;
	div1 = -(pow((d - VAR), 2));
	div2 = 2 * pow(ectype, 2);
	double fact2 = div1 / div2;
	double fact3 = exp(fact2);
	double result = fact1 * fact3;

	return result;
}

void generateVect(double* phi, int X, int Y) {

	int ind = 0, i, j;
	double distance;
	for (i = 0; i < PHI_SIZE; i++)
		phi[i] = 0;
	for (i = 0; i < GRID_SIZE; i++) {
		for (j = 0; j < GRID_SIZE; j++) {
			if ((X + i < 1 && Y + j < 1)
					|| (abs(X - i) < 1 && abs(Y - j) < 1)) {
				ind = i * GRID_SIZE + j;
				distance = sqrt(
						pow((X - i) * DISTANCE, 2)
								+ pow((Y - j) * DISTANCE, 2));

				//if(generateGaussian(VAR, ECTYPE, distance) > 0.4){
				phi[ind] = generateGaussian(VAR, ECTYPE, distance);
			} else
				phi[ind] = 0;
		}
	}
	ind = X * GRID_SIZE + Y;
	phi[ind] = 1;
}

void dyna_MG(double* theta, double** b, double*** F, int* episode_to_converge, int* step_to_converge, int testSeed) {

	PQueue pQueue;
	PQueueE pQueueE2, pQueueE, head;
	int i, j, e, a, pas, cpt, cptStop = 0, test = 0, step_to_converge_per_episode = 0;
	int X, Y, Xnext, Ynext, A, Anext, A2;
	float d = 0;
	double delta, r, priority, R = 0, alpdelta;

	double* phi1 = (double*) malloc(PHI_SIZE * sizeof(double));
	double* phi2 = (double*) malloc(PHI_SIZE * sizeof(double));
	double* AlphaDeltaPhi1 = (double*) malloc(PHI_SIZE * sizeof(double));
	double* oldTheta = (double*) malloc(PHI_SIZE * sizeof(double));

	pQueue = createPQueue();

	double* resultTmp1 = (double*) calloc(PHI_SIZE, sizeof(double));
	double* resultTmp2 = (double*) calloc(PHI_SIZE, sizeof(double));
	double** resultTmp4 = initMatrix(PHI_SIZE, PHI_SIZE);

#ifdef DEBUG
	struct timeval start, end;
#endif

	for (e = 0; e < NB_EPISODES; e++) {

		pQueue = createPQueue();
		printf("Episode n°%d\n", e);

		cpt = 0;

		for (i = 0; i < PHI_SIZE; i++)
			oldTheta[i] = theta[i];

		//Select a random real state
		do{
			X = rand() % GRID_SIZE;
			Y = rand() % GRID_SIZE;
		}
		while((X == RWX) && (Y == RWY));

		//Generate a feature vector
		generateVect(phi1, X, Y);

		R = 0;

#ifdef DEBUG
		gettimeofday(&start, NULL);
#endif

		while (cpt != 1){

			(*step_to_converge)++;

			//printf("Step to converge = %d\n", *step_to_converge);
			step_to_converge_per_episode++;

			//next real state
			Xnext = X;
			Ynext = Y;

			//Select an action
#ifdef EGREEDY
			A = e_greedy(X, Y, phi1, theta, EPSILON, b, F);
#endif
#ifdef SOFTMAX
			A = softmax(X, Y, phi1, theta, b, F);
#endif
#ifdef RAND
			A = rand()%4;
#endif

			//reward
			r = 0;

			switch (A) {
			case NORTH:
				//We cannot move
				if (X == 0)
					r = 0;
				else
					Xnext = X - 1;
				break;
			case EAST:
				if (Y == GRID_SIZE - 1)
					r = 0;
				else
					Ynext = Y + 1;
				break;
			case SOUTH:
				if (X == GRID_SIZE - 1)
					r = 0;
				else
					Xnext = X + 1;
				break;
			case WEST:
				if (Y == 0)
					r = 0;
				else
					Ynext = Y - 1;
				break;
			}

			if ((Xnext == RWX) && (Ynext == RWY)) {
				r = REWARD_VALUE;
				cpt++;
			}

			R += r;
			generateVect(phi2, Xnext, Ynext);

			//delta updating
			delta = r + (GAMMA * multVectorOneValue(theta, phi2)) - multVectorOneValue(theta, phi1);

			//theta updating
			alpdelta = ALPHA * delta;
			multiplicationVectorScalar(AlphaDeltaPhi1, phi1, alpdelta);
			additionVector(theta, AlphaDeltaPhi1);

			//F updating
			multMatrixCarreCol(resultTmp1, F, phi1, A);			        //F*phi
			soustractionVector(resultTmp2, phi2, resultTmp1);			//phi'-F*phi
			multiplicationVectorScalar(resultTmp1, resultTmp2, ALPHA);	//alpha*(phi'-F*phi)
			multMatrixColL(resultTmp4, resultTmp1, phi1);				//alpha*(phi'-F*phi)*(phi)T
			additionMatrix(F, resultTmp4, A);							//F <- F + alpha*(phi'-F*phi)*(phi)T

			//b updating
			double tmp = multVectorOneValue2(b, phi1, A); 				//(b)T*phi
			double lambda = ALPHA * (r - tmp);							//alpha*(r - (b)T*phi)
			double vectTmp[PHI_SIZE];
			multiplicationVectorScalar(vectTmp, phi1, lambda);			//alpha*(r - (b)T*phi)*phi
			additionVector2(b, vectTmp, A);								//b <- b + alpha*(r - (b)T*phi)

#ifdef REPLAY
			int m;
			for(m=0; m<PHI_SIZE; m++) {
				if(phi1[m] != 0) { //or threshold
					priority = fabs(delta*phi1[m]);

					pQueueE.priority = priority;
					pQueueE.i = m;

					if(priority != 0 && priority < DBL_MAX)
						pQueue = addElement(pQueue, pQueueE);
				}
			}

			int p = 0;

			while(pQueue != NULL) {

				if(p == NB_STEPS) break;

				head = headP(pQueue);
				int indice = head.i;
				pQueue = deleteHead(pQueue);

				double resultTmp5[PHI_SIZE], tmp2;
				int exist = 0;

				for(j=0; j<PHI_SIZE; j++) {
					for(a = 0; a<NB_ACTIONS; a++) {
						if(F[a][indice][j] != 0) exist = 1;
					}

					if(exist == 1) {
						exist = 0;

						double best = -DBL_MAX;
						double result;

						for(a=0; a<NB_ACTIONS; a++) {
							multMatrixLCarre(resultTmp5, theta, F, a);
							result = b[a][j] + (GAMMA*resultTmp5[j]);
							if(result > best) {
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

			X = Xnext;
			Y = Ynext;
			generateVect(phi1, X, Y);

		}

#ifdef POLICY_VERIF
		int verif = verifPolicy(theta, b, F);
		if (verif == 0) {
			(*episode_to_converge) = e;
			break;
		}
#endif

#ifdef THETA_CONV_VERIF
		double diff, diffMax = 0;
		for (i = 0; i < PHI_SIZE; i++) {
			diff = fabsf(oldTheta[i] - theta[i]);

			if (diffMax < diff)
				diffMax = diff;

		}
		if(diffMax < THETA_CONV) {
			cptStop++;
		}
		else {
			cptStop = 0;
		}

		if(cptStop == HAVE_REALLY_CONV) break;
#endif

		(*episode_to_converge) = e;

#ifdef DEBUG
		gettimeofday(&end, NULL);
		fprintf(fileTimeReward, "%ld;%f\n", (end.tv_sec*1000000+end.tv_usec - (start.tv_sec*1000000+start.tv_usec)), R);
#endif

#ifdef WFILE2
		fprintf(testSeed, "%d;%d\n", e, step_to_converge_per_episode);
#endif
		step_to_converge_per_episode = 0;

		//suppPQueue(pQueue);
	}

	freeMatrix(resultTmp4, PHI_SIZE, PHI_SIZE);
	free(resultTmp1);
	free(resultTmp2);
	free(phi1);
	free(phi2);
	free(AlphaDeltaPhi1);
	free(oldTheta);
	suppPQueue(pQueue);

}

int verifPolicy(double* theta, double** b, double*** F) {

	int i, j, action, wrong = 0;
	double* phi = (double*)malloc(sizeof(double) * PHI_SIZE);

	for (i = 0; i < GRID_SIZE; i++) {
		for (j = 0; j < GRID_SIZE; j++) {

			if (i == RWX && j == RWY) {

			}

			else {
				generateVect(phi, i, j);
				ListMaxAction listMaxAction = NULL;
				listMaxAction = bestActionForVerifPolicy(phi, theta, b, F);

				while (listMaxAction != NULL) {

					action = listMaxAction->action;

					if (i == RWX && j != RWY) {
						if (j < RWY) {
							if (action != EAST)
								wrong++;
						} else if (j > RWY) {
							if (action != WEST)
								wrong++;
						}
					}

					else if (j == RWY && i != RWX) {
						if (i < RWX) {
							if (action != SOUTH)
								wrong++;
						} else if (i > RWX) {
							if (action != NORTH)
								wrong++;
						}
					}

					else if (i != RWX && j != RWY) {
						if (i < RWX) {
							if (j < RWY) {
								if (action != SOUTH && action != EAST)
									wrong++;
							} else {
								if (action != SOUTH && action != WEST)
									wrong++;
							}
						} else {
							if (j < RWY) {
								if (action != NORTH && action != EAST)
									wrong++;
							} else {
								if (action != NORTH && action != WEST)
									wrong++;
							}
						}

					}
					if (listMaxAction->next != NULL){

						listMaxAction = deleteHeadListMaxAction(listMaxAction);
					}
					else{
						break;
					}

				}
				suppListMaxAction(listMaxAction);
			}
		}
	}
	free(phi);

	return wrong;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISPLAY
//////////////////////////////////////////////////////////////////////////////////////////////////////////

void displayGridDirections(double* theta, double** b, double*** F) {
	int i, j, action, wrong = 0;
	double* phi = (double*) malloc(PHI_SIZE * sizeof(double));
	for (i = 0; i < GRID_SIZE; i++) {
		for (j = 0; j < GRID_SIZE; j++) {

			if (i == RWX && j == RWY) {
				printf(" +%0.1f", REWARD_VALUE);
			}

			else {
				generateVect(phi, i, j);
				action = bestAction(phi, theta, b, F);

				if (i == RWX && j != RWY) {
					if (j < RWY) {
						if (action != EAST)
							wrong++;
					} else if (j > RWY) {
						if (action != WEST)
							wrong++;
					}
				}

				else if (j == RWY && i != RWX) {
					if (i < RWX) {
						if (action != SOUTH)
							wrong++;
					} else if (i > RWX) {
						if (action != NORTH)
							wrong++;
					}
				}

				else if (i != RWX && j != RWY) {
					if (i < RWX) {
						if (j < RWY) {
							if (action != SOUTH && action != EAST)
								wrong++;
						} else {
							if (action != SOUTH && action != WEST)
								wrong++;
						}
					} else {
						if (j < RWY) {
							if (action != NORTH && action != EAST)
								wrong++;
						} else {
							if (action != NORTH && action != WEST)
								wrong++;
						}
					}

				}

				switch (action) {
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
	free(phi);
}
