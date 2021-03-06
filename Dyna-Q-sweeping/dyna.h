#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <float.h>

#include "list.h"

#define NB_STEPS 10	 // 10
#define NB_EPISODES 1000 // 1 000
#define NB_TRY 20 	 // 20	
#define SWEEPING 1
#define NB_ACTIONS 4

#define GRID_SIZE 5

#define REWARD_VALUE 10
#define REWARD_VALUE2 3

//Rewards' position
#define RWX 0
#define RWY 1

//When you get a reward you get back to...
#define RX 4
#define RY 1

#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

#define ALPHA 	0.1
#define GAMMA 	0.9
#define TETA_Q  0.4
#define EPSILON 0.5

#define TETA_P 0.4

#define Q_INITIAL  0.0

#define WIN 7

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Find the best Q-value from a given state (i, j)
float bestQ(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);

//Choose the best action from a given state (i, j)
int bestAction(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);

//Same but return WIN value when you're on a reward state
int bestActionForTest(int i, int j, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);

//Select an action with a e-greedy policy
int e_greedy(int x, int y, float e, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);

//Dyna-Q function 
DynaQReturn dyna_Q(Model model, PQueue pQueue, float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS], int X, int Y, int A, int* step_to_converge);

//Test function with (X, Y) the starter state
void test(int X, int Y, int grid[GRID_SIZE][GRID_SIZE], float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Display
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Display the grid configuration (rewards, our position etc..)
void displayConfig(int stateX, int stateY, int grid[GRID_SIZE][GRID_SIZE]);

//Display Q-table
void displayQ(float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);

//Display the result grid 
void displayGridDirections(float Q[GRID_SIZE][GRID_SIZE][NB_ACTIONS]);


