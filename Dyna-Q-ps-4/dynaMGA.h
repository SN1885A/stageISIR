#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "listMGA.h"

//Experimentation definition
#define NB_STEPS 5 // 10
#define NB_EPISODES 100000 // 1 0000
#define REPLAY 1
//#define THETA_CONV_VERIF
#define POLICY_VERIF
#define DEBUG 1

//Universe definition
#define NB_ACTIONS 4
#define GRID_SIZE 10 //5

//State vector
#define PHI_SIZE 100//25 //5*5

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
int bestAction(double* phi, double* theta, double** b, double*** F);

//Select an action with a e-greedy policy
int e_greedy(int x, int y, double* phi, double* theta, float e, double** b, double*** F);

//Dyna-MG function 
//void dyna_MG(double theta[PHI_SIZE], double b[NB_ACTIONS][PHI_SIZE], double F[NB_ACTIONS][PHI_SIZE][PHI_SIZE], int* episode_to_converge, int* step_to_converge);
void dyna_MG(double* theta, double** b, double*** F, int* episode_to_converge, int* step_to_converge);

double generateGaussian(int var, int ectype, double d);

void generateVect(double* phi, int X, int Y);

void normalize(double phi[PHI_SIZE]);

int verifPolicy(double* theta, double** b, double*** F);

ListMaxAction bestActionForVerifPolicy(double* phi, double *theta, double** b, double*** F);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Matrix operations
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double multVectorOneValue(double* mat1, double* mat2);

double multVectorOneValue2(double** mat1, double* mat2, int action);

void multiplicationVectorScalar(double* result, double* mat, double lambda);

void multMatrixCarreCol(double* result, double*** mat1, double* mat2, int action);

void multMatrixLCarre(double* result, double* mat1, double*** mat2, int action);

void multMatrixColL(double** result, double* mat1, double* mat2);

void additionVector(double* result, double* mat1, double* mat2);

void additionVector2(double** mat1, double* mat2, int action);

void soustractionVector(double* result, double* mat1, double* mat2);

void additionMatrix(double*** mat1, double** mat2, int action);

double** initMatrix(int dim1 , int dim2);

void freeMatrix(double** mat, int dim1 , int dim2);

double*** initCube(int dim1 , int dim2 ,int  dim3);

void freeCube(double*** mat, int dim1 , int dim2 , int dim3);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Display
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Display the result grid 
void displayGridDirections(double* theta, double** b, double*** F);


