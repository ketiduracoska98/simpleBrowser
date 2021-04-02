#include "utils.h"
// allocate memory for the tab element
Tab* allocTab() {
	Tab *tab = malloc(sizeof(Tab));
	if (tab == NULL)
		return NULL;
	tab->current_page = NULL;
	// create the stacks
	tab->back = InitS(sizeof(Page));
	tab->forward = InitS(sizeof(Page));
	tab->next = NULL;
	return tab;
}
// add tab element at the end of the list
void newtab(Tab **head_ref) {
	Tab *tab = allocTab();
	if (tab == NULL)
		return;
	Tab *temp = *head_ref;
	// go to the last element
	while (temp->next != NULL) {
		temp = temp->next;
	}
	// add new element at the edn
	temp->next = tab;
	return;
}
/* return the elemet at the position index
 the change the current tab */ 
Tab* change_tab(Tab *temp, int index) {
	while (index > 0) {
		index--;
		temp = temp->next;
	}
	return temp;
}
// returns the size of the linked list
int size(Tab *temp) {
	int size = 0;
	while (temp != NULL) {
		size++;
		temp = temp->next;
	}
	return size;
}
// delete the last element of the linked list
void deltab(Tab *tab) {
	Tab *temp = tab, *prev;
	while (temp->next != NULL) {
		prev = temp;
		temp = temp->next;
	}
	prev->next = NULL;
	free(temp);
}
// print the linked list
void printTabs(Tab *tab, FILE *fw) {  
	int i = 0;
    while (tab != NULL) {  
    	if (tab->current_page == NULL) {
    		// no page opened at this tab
    		fprintf(fw, "(%d: empty)\n", i);
    	} else {
    		// print the url of the opened page
			fprintf(fw, "(%d: %s)\n", i, tab->current_page->url);
    	}
    	i++;
        tab = tab->next;  
    }  
}
