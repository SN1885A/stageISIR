#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef struct state{
	int X;
	int Y;
}State;

typedef struct sasr{
	State firstState, secondState;
	int action, reward;
}Sasr;

typedef struct PQueueE{
	State state;
	int action;
	float priority;
}PQueueE;

typedef struct cellP{
	PQueueE sa;
	struct cellP *next;
}*PQueue;

typedef struct cell{
	Sasr value;
	struct cell *next;
}*List;

typedef struct model{
	List list;
	int size;
}Model;

typedef struct dynaQReturn{
	Model model;
	PQueue pQueue;
	float delta;
}DynaQReturn;

//List for the model
List createList();
Sasr head(List L);
int empty(List L);

List addHead(List L, Sasr x);
List addQueue(List L, Sasr x);
List deleteElement(List L, Sasr x);

int alreadyExist(List L, Sasr x);
Sasr findSrWithSa(List L, State s, int action);
List findAllStateLeadsToS(List L, State s);
List deleteHeadL(List L);

void displayList(List L);

//PQueue list
PQueue createPQueue();
int emptyP(PQueue P);
int equalsPQE(PQueueE e1, PQueueE e2);

PQueue addHeadP(PQueue P, PQueueE x);
PQueue addQueueP(PQueue P, PQueueE x);
PQueue addElement(PQueue P, PQueueE x);

PQueueE headP(PQueue P);
PQueue deleteHead(PQueue P);

void displayPQueue(PQueue P);

