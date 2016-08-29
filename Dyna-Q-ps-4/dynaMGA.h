#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include "listMGA.h"

//Experimentation definition
#define NB_STEPS 5 // 10
#define NB_EPISODES 100000000 // 100 000 000
#define REPLAY 1
//#define WFILE 1
//#define WFILE2 1
//#define THETA_CONV_VERIF
#define POLICY_VERIF
#define SOFTMAX 1
//#define EGREEDY 1
//#define RAND 1

//#define DEBUG 1

//Universe definition
#define NB_ACTIONS 4
#define GRID_SIZE 5

//State vector
#define PHI_SIZE 25

//Gaussian parameters
#define DISTANCE 10 //10 cm la case
#define ECTYPE 30   //30
#define	VAR 0

//Reward's value
#define REWARD_VALUE 0.2

//Rewards' position
#define RWX 2
#define RWY 2

//When you get a reward you get back to...
#define RX 0
#define RY 0

//Actions
#define NORTH   0
#define EAST    1
#define SOUTH   2
#define WEST    3

//All constants
#define ALPHA 	0.3
#define GAMMA 	0.7
#define BETA 	30 //100
#define THETA_CONV  0.0001
#define EPSILON 0.06 //0.006
#define HAVE_REALLY_CONV 20

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int e_greedy(int x, int y, double* phi, double* theta, float e, double** b, double*** F);

int softmax(int x, int y, double* phi, double* theta, double** b, double*** F);

void dyna_MG(double* theta, double** b, double*** F, int* episode_to_converge, int* step_to_converge, int testSeed);

double generateGaussian(int var, int ectype, double d);

void generateVect(double* phi, int X, int Y);

//Check if the policy is correct
int verifPolicy(double* theta, double** b, double*** F);

//Return a list with the best actions
ListMaxAction bestActionForVerifPolicy(double* phi, double *theta, double** b, double*** F);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Matrix operations
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

double multVectorOneValue(double* mat1, double* mat2);

double multVectorOneValue2(double** mat1, double* mat2, int action);

void multiplicationVectorScalar(double* result, double* mat, double lambda);

void multMatrixCarreCol(double* result, double*** mat1, double* mat2,
		int action);

void multMatrixLCarre(double* result, double* mat1, double*** mat2, int action);

void multMatrixColL(double** result, double* mat1, double* mat2);

void additionVector(double* mat1, double* mat2);

void additionVector2(double** mat1, double* mat2, int action);

void soustractionVector(double* result, double* mat1, double* mat2);

void additionMatrix(double*** mat1, double** mat2, int action);

double** initMatrix(int dim1, int dim2);

void freeMatrix(double** mat, int dim1, int dim2);

double*** initCube(int dim1, int dim2, int dim3);

void freeCube(double*** mat, int dim1, int dim2, int dim3);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Display
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Display the result grid 
void displayGridDirections(double* theta, double** b, double*** F);

