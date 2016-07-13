#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "list.h"

List createList(void){ return NULL;}

List addHead(List L, Sasr x){
	List newList;
	newList=(List)malloc(sizeof(*newList));
	newList->value=x;
	newList->next=L;
return newList;
}

List addQueue(List L, Sasr x){
	if(L==NULL){
		L=(List)malloc(sizeof(*L));
		L->value=x;
		L->next=NULL;
	}
	else
		L->next=addQueue(L->next, x);
return L;
}

Sasr head(List L){
	return (L->value);
}

int empty(List L){ return (L==NULL);}

int alreadyExist(List L, Sasr x){

	List newList = L;
    	if(!empty(L)){
		
		if( (newList->value.firstState.X == x.firstState.X) && (newList->value.firstState.Y == x.firstState.Y)
		&& (newList->value.secondState.X == x.secondState.X) && (newList->value.secondState.Y == x.secondState.Y)
		&& (newList->value.action == x.action)
		&& (newList->value.reward == x.reward) ){
			
           		return 1;
        	}
		else{
		    while(newList->next != NULL){
		        if( (newList->next->value.firstState.X == x.firstState.X) && (newList->next->value.firstState.Y == x.firstState.Y)
			&& (newList->next->value.secondState.X == x.secondState.X) && (newList->next->value.secondState.Y == x.secondState.Y)
			&& (newList->next->value.action == x.action)
			&& (newList->next->value.reward == x.reward) ){
				
		        	return 1; 
		          
		        }
		        newList = newList->next;
		    }
		}
    	}

return 0;
}

List deleteElement(List L, Sasr x){
	List newList = L;
    	if(!empty(L)){
		if( (newList->value.firstState.X == x.firstState.X) && (newList->value.firstState.Y == x.firstState.Y)
		&& (newList->value.secondState.X == x.secondState.X) && (newList->value.secondState.Y == x.secondState.Y)
		&& (newList->value.action == x.action)
		&& (newList->value.reward == x.reward) ){
	
		    return(newList->next);
		}
		else{
		    while(newList->next != NULL){
		        if( (newList->next->value.firstState.X == x.firstState.X) && (newList->next->value.firstState.Y == x.firstState.Y)
			&& (newList->next->value.secondState.X == x.secondState.X) && (newList->next->value.secondState.Y == x.secondState.Y)
			&& (newList->next->value.action == x.action)
			&& (newList->next->value.reward == x.reward) ){
		            List a = newList->next;
		            newList->next = newList->next->next;
		            free(a);
		        }
		        newList = newList->next;
		    }
		}
    	}
    return(L);
}

Sasr findSrWithSa(List L, State s, int action){

	List newList = L;
    	if(!empty(L)){
		
		if( (newList->value.firstState.X == s.X) && (newList->value.firstState.Y == s.Y)
		&& (newList->value.action == action) ){
           		return newList->value;
        	}
		else{
		    while(newList->next != NULL){
		        if( (newList->next->value.firstState.X == s.X) && (newList->next->value.firstState.Y == s.Y)
			&& (newList->next->value.action == action) ){
				
		        	return newList->next->value; 
		          
		        }
		        newList = newList->next;
		    }
		}
    	}
}

List findAllStateLeadsToS(List L, State s){
	List result = createList();
	List newList = L;
    	if(!empty(L)){
		
		if( (newList->value.secondState.X == s.X) && (newList->value.secondState.Y == s.Y) ){
           		result = addQueue(result, newList->value);
        	}

		else{
		    while(newList->next != NULL){
		        if( (newList->next->value.secondState.X == s.X) && (newList->next->value.secondState.Y == s.Y) ){
		        	result = addQueue(result, newList->next->value); 
		        }
		        newList = newList->next;
		    }
		}
    	}

return result;
}

void displayList(List L){
    if(empty(L))
        printf("List is empty");
    else{
        while(L->next != NULL){
            printf("((%d, %d), %d, (%d, %d), %d)\n",L->value.firstState.X, L->value.firstState.Y, L->value.action, L->value.secondState.X, L->value.secondState.Y, L->value.reward);
            L = L->next;
        }
    }

    printf("\n");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PQueue
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PQueue createPQueue(){ return NULL; }
int emptyP(PQueue P){ return (P == NULL); }

PQueue addHeadP(PQueue P, PQueueE x){
	PQueue newQueue;
	newQueue=(PQueue)malloc(sizeof(*newQueue));
	newQueue->sa=x;
	newQueue->next=P;

return newQueue;
}

PQueue addQueueP(PQueue P, PQueueE x){

	if(P==NULL){
		P=(PQueue)malloc(sizeof(*P));
		P->sa=x;
		P->next=NULL;
	}
	else
		P->next=addQueueP(P->next, x);
return P;
}

int equalsPQE(PQueueE e1, PQueueE e2){

	if(e1.state.X == e2.state.X && e1.state.Y == e2.state.Y && e1.action == e2.action && e1.priority == e2.priority) return 1;
	else return 0;
}

PQueue addElement(PQueue P, PQueueE x){
	if(P != NULL){
		if(equalsPQE(P->sa, x)){
			if(P->sa.priority < x.priority) P->sa.priority = x.priority;
		}
		else if(P->sa.priority < x.priority)
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
	return (P->sa);
}

PQueue deleteHead(PQueue P){

	if (P == NULL) return NULL;
	else{
		PQueue r = P;
		P = P->next;
		free(r);
		
	}
}
