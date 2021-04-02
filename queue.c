#include "utils.h"
// Allocate memory for the queue
void* InitQ(size_t d) {
	Coada *c = malloc(sizeof(Coada));
	if (c == NULL)
		return NULL;
	c->dim = d;
	c->front = c->rear = NULL;
	return (void *)c;
}
// add element to the queue
void Enqueue(void *c, void *ae, int size) {
	List *aux = allocList(ae, size);
	if (aux == NULL)
		return;
	if (((Coada *)c)->rear == NULL) {
		// frist element to the queue
		((Coada *)c)->rear = ((Coada *)c)->front = aux;
		return;
	}
	((List *)((Coada *)c)->rear)->next = aux;
	((Coada *)c)->rear = aux;
}
// remove element from the front of the queue
void Dequeue(void *c, void *ae, int size) {
	if (((Coada *)c)->front == NULL)
		return;
	// copy the information to ae
	memcpy(ae, ((List *)((Coada *)c)->front)->info, size);
	// change the links the free the memory
	List *temp = (List *)((Coada *)c)->front;
	((Coada *)c)->front = ((List *)(((Coada *)c)->front))->next; 
	if(((Coada *)c)->front == NULL)
		((Coada *)c)->rear = NULL;
	temp->next = NULL; 
	free(temp);
}

void DisplayQueue(void *c, int size, FILE *fw) {
	if (((Coada *)c)->front == NULL)
		// the queue is empty
		return;
	void *aux = InitQ(size);
	if (aux == NULL) 
		return;
	void *ae = malloc(size);
	if (ae == NULL) {
		free(aux);
		return;
	}
	while (((Coada *)c)->front != NULL) {
		// move elements to aux
		Dequeue(c, ae, size);
		Enqueue(aux, ae, size);
	}
	while (((Coada *)aux)->front != NULL) {
		/* print the elements url from the aux and add
		 them back to original queue */
		Dequeue(aux, ae, size);
		fprintf(fw, "%s\n", ((Page *)ae)->url);
		Enqueue(c, ae, size);
	}
	free(aux);
	free(ae);
}
/* print the resources remaining size and 
 dimension  */
void DisplayPQueue(void *c, int size, FILE *fw) {
	if (((Coada *)c)->front == NULL)
		return;
	void *aux = InitQ(size);
	if (aux == NULL)
		return;
	void *ae = malloc(size);
	if (ae == NULL) {
		free(aux);
		return;
	}
	while (((Coada *)c)->front != NULL) {
		Dequeue(c, ae, size);
		Enqueue(aux, ae, size);
	}
	while (((Coada *)aux)->front != NULL) {
		Dequeue(aux, ae, size);
		unsigned long rem_size = ((Resource *)ae)->dimension 
		- ((Resource *)ae)->currently_downloaded;
		fprintf(fw, "[\"%s\" : %ld/%ld]\n", ((Resource *)ae)->name, 
		rem_size, ((Resource *)ae)->dimension);
		Enqueue(c, ae, size);
	}
	free(aux);
	free(ae);
}

void DeleteQueue(void *c, int nr_entries, int size) {
	void *ae = malloc(size);
	if (ae == NULL)
		return;
	if (((Coada *)c)->front != NULL) {
		if (nr_entries != 0) {
			while (nr_entries > 0) {
				Dequeue(c, ae, size);
				nr_entries--;
			}
			// 0 elements - delete whole queue
		} else {
			while (((Coada *)c)->front != NULL) {
				Dequeue(c, ae, size);
			}
		}
	}
	free(ae);
}
