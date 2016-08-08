#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>


typedef struct PQueueE{
	int i;
	double priority;
}PQueueE;

typedef struct cellP{
	PQueueE element;
	struct cellP *next;
}*PQueue;

typedef struct cellA{
	int action;
	double value;
	struct cellA *next;
}*ListMaxAction;

typedef struct cellAP{
	int action;
	double prob;
	struct cellAP *next;
}*ListIndAction;

PQueue createPQueue();
int emptyP(PQueue P);
int equalsPQE(PQueueE e1, PQueueE e2);

PQueue addHeadP(PQueue P, PQueueE x);
PQueue addQueueP(PQueue P, PQueueE x);
PQueue addElement(PQueue P, PQueueE x);

ListMaxAction addElementListQueue(ListMaxAction list, int a, double value);
ListMaxAction addElementListMaxAction(ListMaxAction list, int a, double value, int *size);
int listMaxActionRandom(ListMaxAction list, int size);

ListIndAction addHeadLIA(ListIndAction L, int action, double prob);
ListIndAction addQueueLIA(ListIndAction L, int action, double prob);
ListIndAction addElementLIA(ListIndAction L, int action, double prob);

PQueueE headP(PQueue P);
PQueue deleteHead(PQueue P);

void displayPQueue(PQueue P);

