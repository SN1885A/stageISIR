#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "listMGA.h"


PQueue createPQueue(){ return NULL; }
int emptyP(PQueue P){ return (P == NULL); }

PQueue addHeadP(PQueue P, PQueueE x){
	PQueue newQueue;
	newQueue=(PQueue)malloc(sizeof(*newQueue));
	newQueue->element=x;
	newQueue->next=P;

return newQueue;
}

PQueue addQueueP(PQueue P, PQueueE x){

	if(P==NULL){
		P=(PQueue)malloc(sizeof(*P));
		P->element=x;
		P->next=NULL;
	}
	else
		P->next=addQueueP(P->next, x);
return P;
}


int equalsPQE(PQueueE e1, PQueueE e2){

	if(e1.i == e2.i) return 1;
	else return 0;
}

PQueue addElement(PQueue P, PQueueE x){
	if(P != NULL){
		if(equalsPQE(P->element, x)){
			if(P->element.priority < x.priority) P->element.priority = x.priority;
		}
		else if(P->element.priority < x.priority)
		    P = addHeadP(P, x);
		else{
		        P->next = addElement(P->next, x);
		}
    }
	else
        	P = addQueueP(P, x);
return P;
}

PQueueE headP(PQueue P){
	return (P->element);
}

PQueue deleteHead(PQueue P){

	if (P == NULL) return NULL;
	else{
		PQueue r = P;
		P = P->next;
		free(r);
		
	}
return P;
}

void displayPQueue(PQueue P){
	PQueue tmp = P;
	if(emptyP(P)) printf("PQueue is empty");
	else{
		
		while(tmp != NULL){
			printf("indice = %d priority = %f\n", tmp->element.i, tmp->element.priority);
			if(tmp->next != NULL) tmp = tmp->next;
			else break;
		}
	}
	printf("\n");
}


ListMaxAction addElementListQueue(ListMaxAction list, int a, double value){

	if(list == NULL){
		list=(ListMaxAction)malloc(sizeof(*list));
		list->action=a;
		list->value = value;
		list->next=NULL;
	}
	else
		list->next=addElementListQueue(list->next, a, value);

return list;
}

ListMaxAction addElementListMaxAction(ListMaxAction list, int a, double value, int *size){

	if(list == NULL){
		list = addElementListQueue(list, a, value);
		(*size)++;
	}
	else{
		if(list->value == value){
			list = addElementListQueue(list, a, value);
			(*size)++;
		}
		else{
			list = NULL;
			list = addElementListQueue(list, a, value);
			(*size) = 1;
		}

	}

	return list;
}

int listMaxActionRandom(ListMaxAction list, int size){

	int a;
	ListMaxAction tmp = list;

	int r = rand()%size;
	int cpt = 0;

	while(r != cpt){
		tmp = tmp->next;
		cpt++;
	}
	a = tmp->action;

return a;
}

ListIndAction addHeadLIA(ListIndAction L, int action, double prob){

	ListIndAction newList;
	newList=(ListIndAction)malloc(sizeof(*newList));
	newList->prob = prob;
	newList->action = action;
	newList->next = L;

return L;
}

ListIndAction addQueueLIA(ListIndAction L, int action, double prob){

	if(L==NULL){
			L=(ListIndAction)malloc(sizeof(*L));
			L->prob = prob;
			L->action = action;
			L->next = NULL;
		}
		else
			L->next = addQueueLIA(L->next, action, prob);
return L;

}

ListIndAction addElementLIA(ListIndAction L, int action, double prob){

	if(L != NULL){

		if(L->prob < prob)
			L = addHeadLIA(L, action, prob);
		else
				L->next = addElementLIA(L->next, action, prob);
	}
	else
		L = addQueueLIA(L, action, prob);

return L;
}

void displayLIA(PQueue P){
	PQueue tmp = P;
	if(emptyP(P)) printf("PQueue is empty");
	else{

		while(tmp != NULL){
			printf("indice = %d priority = %f\n", tmp->element.i, tmp->element.priority);
			if(tmp->next != NULL) tmp = tmp->next;
			else break;
		}
	}
	printf("\n");
}



