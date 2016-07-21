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

PQueue createPQueue();
int emptyP(PQueue P);
int equalsPQE(PQueueE e1, PQueueE e2);

PQueue addHeadP(PQueue P, PQueueE x);
PQueue addQueueP(PQueue P, PQueueE x);
PQueue addElement(PQueue P, PQueueE x);

PQueueE headP(PQueue P);
PQueue deleteHead(PQueue P);

void displayPQueue(PQueue P);

