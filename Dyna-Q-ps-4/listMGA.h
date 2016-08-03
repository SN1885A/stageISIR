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
	struct cellA *next;
}*ListMaxAction;

PQueue createPQueue();
int emptyP(PQueue P);
int equalsPQE(PQueueE e1, PQueueE e2);

PQueue addHeadP(PQueue P, PQueueE x);
PQueue addQueueP(PQueue P, PQueueE x);
PQueue addElement(PQueue P, PQueueE x);

ListMaxAction addElementListMaxAction(ListMaxAction list, int a);
int listMaxActionRandom(ListMaxAction list, int size);

PQueueE headP(PQueue P);
PQueue deleteHead(PQueue P);

void displayPQueue(PQueue P);

