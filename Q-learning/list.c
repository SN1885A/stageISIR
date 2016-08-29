#include "list.h"

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
			suppListMaxAction(list);
			list=NULL;
			list = addElementListQueue(list, a, value);
			(*size) = 1;
		}

	}

	return list;
}

ListMaxAction deleteHeadListMaxAction(ListMaxAction P){

	if (P == NULL) return NULL;
	else{
		ListMaxAction r = P;
		P = P->next;
		free(r);

	}
return P;
}

void suppListMaxAction(ListMaxAction L){
	while(L != NULL){
		ListMaxAction tmp = L->next;
		free(L);
		L = tmp;
	}
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
