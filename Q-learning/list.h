#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//List of actions with the same value
typedef struct cellA{
	int action;
	double value;
	struct cellA *next;
}*ListMaxAction;


ListMaxAction addElementListQueue(ListMaxAction list, int a, double value);
ListMaxAction addElementListMaxAction(ListMaxAction list, int a, double value, int *size);
int listMaxActionRandom(ListMaxAction list, int size);
void suppListMaxAction(ListMaxAction L);
ListMaxAction deleteHeadListMaxAction(ListMaxAction P);
