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
	float qm = -FLT_MAX;
	int a, action = 0, size = 0;
	ListMaxAction listMaxAction = NULL;

  	for (a=0; a<NB_ACTIONS; a++){ 
    		if (qm<Q[i][j][a]){
      			qm = Q[i][j][a];
      			listMaxAction = addElementListMaxAction(listMaxAction, a, qm, &size);
   		}
	}

  	action = listMaxActionRandom(listMaxAction, size);
  	suppListMaxAction(listMaxAction);

return action;
}

int bestActionForTest(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	float qm = -FLT_MAX;
	int a, action = 0, size = 0;
	ListMaxAction listMaxAction = NULL;

	if( i==RWX && j==RWY ) return WIN;

	for (a=0; a<NB_ACTIONS; a++)
		if (qm<Q[i][j][a]){
			qm = Q[i][j][a];
			listMaxAction = addElementListMaxAction(listMaxAction, a, qm, &size);
		}

  	action = listMaxActionRandom(listMaxAction, size);
  	suppListMaxAction(listMaxAction);

return action;
}

int e_greedy(int x, int y, float e, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){

	int a = 0;
	double r = (double)rand()/RAND_MAX;
	//e-greegy:
	//With a proba e we randomly select a action
	//With a proba (1-e) we take the best action
	if( (x==RWX && y==RWY) || (r < e) ) a = rand()%NB_ACTIONS;
	else a = bestAction(x, y, Q);
	
return a;
}

DynaQReturn dyna_Q(Model model, PQueue pQueue, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS], int X, int Y, int A,  int* step_to_converge){

	DynaQReturn dynaQReturn;
	int a, i, j, pas, foundReward = 0;
	float delta = 0;
	float diff = 0;
	float p;
	float Qold[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

	model.list = NULL;
	model.size = 0;

	//Save the old Q-table
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){ 
				Qold[i][j][a] = Q[i][j][a];
			}
		}
	}
	
	for(i=0; i<NB_EPISODES; i++){
		
		foundReward = 0;

		//Select a random state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;
		
		//Select an action with e-greedy policy
		A = e_greedy(X, Y, EPSILON, Q);

		//Or until I find a reward
		//for (pas=0; pas<NB_STEPS; pas++){
		while(foundReward != 1){
			//printf("Pas n°%d\n", pas);

			State firstState;
			firstState.X = X;
			firstState.Y = Y;
		  	int Xnext = X; 
			int Ynext = Y;
			int r = 0;

			if ((X==RWX) && (Y==RWY)) {
				foundReward++;
				r = REWARD_VALUE;
				Xnext = RX; 
				Ynext = RY;
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
			
			State nextState;
			nextState.X = Xnext;
			nextState.Y = Ynext;

			Sasr sasr;
			sasr.firstState = firstState;
			sasr.secondState = nextState;
			sasr.action = A;
			sasr.reward = r;

#ifndef SWEEPING

			Q[firstState.X][firstState.Y][sasr.action] += ALPHA * (sasr.reward + GAMMA * bestQ(nextState.X, nextState.Y, Q) - Q[firstState.X][firstState.Y][sasr.action]);
#endif

			int e = alreadyExist(model.list, sasr);
			if(e == 0){
				model.list = addHead(model.list, sasr);
				model.size++;
			}

#ifndef SWEEPING

			for(j=0; j<10 ; j++){
				if(model.size != 0){
					List listTmp = model.list;
					int r = rand()%model.size;
					int cpt = 0;
					while(r != cpt){
						listTmp = listTmp->next;
						cpt++;
					}

					Sasr sasrR = model.list->value;
					State s1 = sasrR.firstState;
					State s2 = sasrR.secondState;

					int action = sasrR.action;
					int reward = sasrR.reward;

					Q[s1.X][s1.Y][action] += ALPHA * ((float)reward + GAMMA * bestQ(s2.X, s2.Y, Q) - Q[s1.X][s1.Y][action]);
				}
			}

#endif



#ifdef SWEEPING
			p = fabs(r + GAMMA * bestQ(Xnext, Ynext, Q) - Q[X][Y][A]);
			
			if(p>TETA_P){
				PQueueE element;
				element.state = firstState;
				element.action = A;
				element.priority = p;
				pQueue = addElement(pQueue, element);
			}		
			
			int action, reward;
			while(pQueue != NULL){

				PQueueE pQueueE = headP(pQueue);
				pQueue = deleteHead(pQueue);
				Sasr sasr2 = findSrWithSa(model.list, pQueueE.state, pQueueE.action); 
				State s, s2;
				s.X = sasr2.firstState.X;
				s.Y = sasr2.firstState.Y;
				s2.X = sasr2.secondState.X;
				s2.Y = sasr2.secondState.Y;
				action = pQueueE.action;
				reward = sasr2.reward;
				Q[s.X][s.Y][action] += ALPHA * (reward + GAMMA * bestQ(s2.X, s2.Y, Q) - Q[s.X][s.Y][action]);
				List l = findAllStateLeadsToS(model.list, s);
				if(l != NULL){
					Sasr sasr3 = head(l);
					l = deleteHeadL(l);
					p = fabs((float)sasr3.reward + GAMMA * bestQ(s.X, s.Y, Q) - Q[sasr3.firstState.X][sasr3.firstState.Y][sasr3.action]);
					if(p > TETA_P){
						PQueueE element2;
						element2.state = sasr3.firstState;
						element2.action = sasr3.action;
						element2.priority = p;
						pQueue = addElement(pQueue, element2);
						
					}
				}
			}
#endif

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

#ifdef SWEEPING
dynaQReturn.pQueue = pQueue;
#endif

dynaQReturn.delta = delta;
dynaQReturn.model = model;

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
			else if( (i == RX && j == RY) || ( (i == stateX && j == stateY) && (i == RX && j == RY) )  ){
				if(i == stateX && j == stateY) printf(" X   ");
				else printf(" R1  ");
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
