#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "listMGA.h"

//Experimentation definition
#define NB_STEPS 5 // 10
#define NB_EPISODES 100000 // 1 0000
#define REPLAY 1
//#define THETA_CONV_VERIF
#define POLICY_VERIF
//#define DEBUG 1

//Universe definition
#define NB_ACTIONS 4
#define GRID_SIZE 5 //5

//State vector
#define PHI_SIZE 25//25 //5*5

//Gaussian parameters
#define DISTANCE 10 //10 cm la case
#define ECTYPE 30   //30
#define	VAR 0

//Reward's value
#define REWARD_VALUE 0.2

//Rewards' position
#define RWX 3
#define RWY 3

//When you get a reward you get back to...
#define RX 4
#define RY 0

//Actions
#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

//All constants
#define ALPHA 	0.3
#define GAMMA 	0.7
#define TAU 	1
#define THETA_CONV  0.00002
#define EPSILON 0.06
#define HAVE_REALLY_CONV 10

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Choose the best action from a given state
int bestAction(double phi[PHI_SIZE], double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]);

//Select an action with a e-greedy policy
int e_greedy(int x, int y, double phi[PHI_SIZE], double theta[PHI_SIZE], float e, double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]);

//Dyna-MG function 
void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int* episode_to_converge, int* step_to_converge);

double generateGaussian(int var, int ectype, double d);

void generateVect(double phi[PHI_SIZE], int X, int Y);

void normalize(double phi[PHI_SIZE]);

int verifPolicy(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]);

ListMaxAction bestActionForVerifPolicy(double phi[PHI_SIZE], double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Matrix operations
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Display
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Display the result grid 
void displayGridDirections(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE]);


