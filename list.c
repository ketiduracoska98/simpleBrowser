#include "utils.h"
// allocate memory for list element and add the info
List *allocList(void *ae, int size) {
	List *list = malloc(sizeof(List));
	if (list == NULL)
	 	return NULL;
	list->info = malloc(size);
	list->next = NULL;
	memcpy(list->info, ae, size);
	return list;
}
// list with downloaded resources
void Finished(List **head_ref, void *ae, int size) {
	List *head = *head_ref;
	List *aux = allocList(ae, size);
	if(aux == NULL)
		return;
	if(*head_ref == NULL) {
		// first element of the list
		*head_ref = aux;
		return;
	}
	while (head->next != NULL) {
		head = head->next;
	}
	head->next = aux;
}
// print the downloaded resources
void print_finished(List *head, FILE *fw) {
	while (head != NULL) {
		fprintf(fw, "[\"%s\" : completed]\n", ((Resource *)head->info)->name);
		head = head->next;
	}
}
