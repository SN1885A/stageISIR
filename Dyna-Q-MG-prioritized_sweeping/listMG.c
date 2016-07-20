#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "listMG.h"

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

	if(e1.i == e2.i && e1.priority == e2.priority) return 1;
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
}

void displayPQueue(PQueue P){

	if(emptyP(P)) printf("PQueue is empty");
	else{
		while(P->next != NULL){
    			printf("indice = %d priority = %f)\n",P->element.i, P->element.priority);
   			P = P->next;
		}
	}
	printf("\n");
}


