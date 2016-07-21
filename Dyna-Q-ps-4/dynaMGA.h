#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "listMGA.h"

//Experimentation definition
#define NB_STEPS 10	 // 10
#define NB_EPISODES 100 // 1 000

//Universe definition
#define NB_ACTIONS 4
#define GRID_SIZE 5

#define PHI_SIZE 25 //5*5

//Gaussian parameters
#define DISTANCE 10 //10 cm la case
#define ECTYPE 10   //30
#define	VAR 0

//Reward's value
#define REWARD_VALUE 10
#define REWARD_VALUE2 6

//Rewards' position
#define RWX 0
#define RWY 1
#define RW2X 0
#define RW2Y 3

//When you get a reward you get back to...
#define RX 4
#define RY 1
#define R2X 2
#define R2Y 3

//Actions
#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

//All constants
#define ALPHA 	0.1
#define GAMMA 	0.9

#define EPSILON 0.5

//Q initial value
#define Q_INITIAL  0.0


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Choose the best action from a given state (i, j)
int bestAction(int i, int j, double b[NB_ACTIONS][PHI_SIZE]);

//Select an action with a e-greedy policy
int e_greedy(int x, int y, float e,double b[NB_ACTIONS][PHI_SIZE]);

//Dyna-MG function 
void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int* step_to_converge);

double generateGaussian(int var, int ectype, double d);

double multVectorOneValue(double mat1[PHI_SIZE], double mat2[PHI_SIZE]);

double multVectorOneValue2(double mat1[NB_ACTIONS][PHI_SIZE], double mat2[PHI_SIZE], int action);

void multiplicationVectorScalar(double result[PHI_SIZE], double mat[PHI_SIZE], double lambda);

void multMatrixCarreCol(double result[PHI_SIZE], double mat1[NB_ACTIONS][PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE], int action);

void multMatrixLCarre(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int action);

void multMatrixColL(double result[PHI_SIZE][PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]);

void additionVector(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]);

void additionVector2(double mat1[NB_ACTIONS][PHI_SIZE], double mat2[PHI_SIZE], int action);

void soustractionVector(double result[PHI_SIZE], double mat1[PHI_SIZE], double mat2[PHI_SIZE]);

void additionMatrix(double mat1[NB_ACTIONS][PHI_SIZE][PHI_SIZE], double mat2[PHI_SIZE][PHI_SIZE], int action);

void generateVect(double phi[PHI_SIZE], int X, int Y);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Display
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Display the grid configuration (rewards, our position etc..)
void displayConfig(int stateX, int stateY, int grid[GRID_SIZE][GRID_SIZE]);

//Display the result grid 
void displayGridDirections(double b[NB_ACTIONS][PHI_SIZE]);


