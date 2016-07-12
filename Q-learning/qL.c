#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "qL.h"

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

float qLearning(int num_it, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS], int X, int Y, int A, int* step_to_converge){

	int a, i, j, p;
	float delta = 0;
	float diff = 0;
	float Qold[GRID_SIZE][GRID_SIZE][NB_ACTIONS];

	//Save the old Q-table
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
			for(a=0; a<NB_ACTIONS; a++){ 
				Qold[i][j][a] = Q[i][j][a];
			}
		}
	}

	for(i=0; i<NB_EPISODES; i++){
		//Select a random state
		X = rand()%GRID_SIZE; 
		Y = rand()%GRID_SIZE;
		
		//Select an action with e-greedy policy
		A = e_greedy(X, Y, EPSILON, Q);

		for (p=0; p<NB_STEPS; p++){

		  	int Xnext = X; 
			int Ynext = Y;
			int r = 0;
			if ((X==RWX) && (Y==RWY)) {  
				//R+= 10*gk; 
				r = REWARD_VALUE;
				Xnext = RX; 
				Ynext = RY;
			}
 			else if ((X==RW2X) && (Y==RW2Y)) {  
				//R+= 10*gk; 
				r = REWARD_VALUE2;
				Xnext = R2X; 
				Ynext = R2Y;
			}
			else {
				switch (A) {
					case NORTH: 
						//We cannot move
						if(X==0)  r = -1; //R -= gk;}
						else Xnext = X - 1;
					break;
					case EAST: 
						if(Y==GRID_SIZE-1)r = -1; //R -= gk;}
						else Ynext = Y + 1;
					break;
					case SOUTH:
						if(X==GRID_SIZE-1) r = -1; //R -= gk;}
						else Xnext = X + 1;
					break;
					case WEST: 
						if(Y==0) r = -1; //R -= gk;}
						else Ynext = Y - 1;
					break;
				}
			}

			
			int aNext = e_greedy(Xnext, Ynext, EPSILON, Q);

			Q[X][Y][A] += ALPHA * (r + GAMMA * bestQ(Xnext, Ynext, Q) - Q[X][Y][A]);

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

void displayGridDirections(float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]){
	int i, j, action;
	for(i=0; i<GRID_SIZE; i++){
		for(j=0; j<GRID_SIZE; j++){
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
