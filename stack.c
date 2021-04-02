#include "utils.h"
// Allocate memory for the stack
void* InitS(size_t d) {
	Stiva *s = malloc(sizeof(Stiva));
	if (s == NULL)
		return NULL;
	s->dim = d;
	s->top = NULL;
	return (void *)s;
}
// add element to the top of the stack
void push(void *s, void *ae, int size) {
	List *aux = allocList(ae, size);
	if (aux == NULL)
		return;
	if (((Stiva *)s)->top == NULL) {
		// first element to the stack
		((Stiva *)s)->top = aux;
		return;
	}
	// change the top pointer to the new element
	aux->next = ((Stiva *)s)->top;
	((Stiva *)s)->top = aux;
}
// remove top element from the stack
void pop(void *s, void *ae, int size) {
	if (((Stiva *)s)->top == NULL)
		return;
	// capy the information to ae 
	memcpy(ae, ((List *)((Stiva *)s)->top)->info, size);
	// change the links and free the memory
	List *temp = (List *)((Stiva *)s)->top;
	((Stiva *)s)->top = ((List *)(((Stiva *)s)->top))->next;
	temp->next = NULL; 
	free(temp);
}

void DeleteStack(void *s, int size) {
	void *ae = malloc(size);
	if (ae == NULL)
		return;

	while (((Stiva *)s)->top != NULL) {
		// pop/remove all the elements
		pop(s, ae, size);
	}
	// free the memory allocated for elements
	free(ae);
}