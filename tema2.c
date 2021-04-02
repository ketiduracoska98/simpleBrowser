#include "utils.h"

/* KETI DURACOSKA 322CB*/

// pop top of back stack and make it current page
void back(Tab *tab, int cur_tab_index, int size, FILE *fw) {
	void *page_stack, *current_page;
	// go to the current tab
	while (cur_tab_index > 1) {
		cur_tab_index--;
		tab = tab->next;
	}
	if (((Stiva *)(tab->back))->top == NULL) {
		fprintf(fw, "\"Can't go back, no pages in stack\"\n");
		return;
	}
	page_stack = malloc(size);
	pop(tab->back, page_stack, size);
	current_page = tab->current_page;
	tab->current_page = (Page *)page_stack;
	/* push the previously current page 
	 to the forward stack. */
	push(tab->forward, current_page, size);	
}
// pop top of forward stack and make it current page
void forward(Tab *tab, int cur_tab_index, int size, FILE *fw) {
	void *page_stack, *current_page;
	// go to the current tab
	while (cur_tab_index > 1) {
		cur_tab_index--;
		tab = tab->next;
	}
	if (((Stiva *)(tab->forward))->top == NULL) {
		fprintf(fw, "\"Can't go forward, no pages in stack\"\n");
		return;
	}
	page_stack = malloc(size);
	pop(tab->forward, page_stack, size);
	current_page = tab->current_page;
	tab->current_page = (Page *)page_stack;
	/* push the previously current page 
	 to the back stack. */
	push(tab->back, current_page, size);	
}
/* print the resources of the opened page at the 
current tab */
void list_dl(Tab *tab, int cur_tab_index, FILE *fw) {
	int i;
	while(cur_tab_index > 1) {
		cur_tab_index--;
		tab = tab->next;
	}
	// tab is empty
	if(tab->current_page == NULL)
		return;
	// print the vector of resources
	for (i = 0; i < ((Page *)tab->current_page)->num_res; i++) {
		fprintf(fw, "[%d - \"%s\" : %ld]\n",
			i, tab->current_page->resources[i].name,
			tab->current_page->resources[i].dimension);
	}
}

void download(void *c, void *ae, int size) {
	/* first element of queue 
	 no need for sorting */ 
	if(((Coada *)c)->front == NULL) {
		Enqueue(c, ae, size);
		return;
	}
	// remaining size for downloading of the new resource
	unsigned long rem_size_ae = ((Resource *)ae)->dimension 
	- ((Resource *)ae)->currently_downloaded;

	void *aux = InitQ(size);
	if(aux == NULL) 
		return;
	void *ae_1 = malloc(size);
	if(ae_1 == NULL) {
		free(aux);
		return;
	}
	unsigned long rem_size_ae_1;
	int flag = 1;
	while(((Coada *)c)->front != NULL) {
		Dequeue(c, ae_1, size);
		// remaining size of the front element 
		rem_size_ae_1 = ((Resource *)ae_1)->dimension 
		- ((Resource *)ae_1)->currently_downloaded;
		if(flag == 1) {
			if(rem_size_ae_1 >= rem_size_ae) {
				// add both resources to the aux queue sorted
				Enqueue(aux, ae, size);
				Enqueue(aux, ae_1, size);
				flag = 0;
			} else if (rem_size_ae_1 < rem_size_ae) {
				Enqueue(aux, ae_1, size);
				if(((Coada *)c)->rear == NULL) {
					Enqueue(aux, ae, size);
					flag = 0;
				}
			}
		} else {
			Enqueue(aux, ae_1, size);
		}
	}
	while(((Coada *)aux)->front != NULL) {
		// add the resources to the original queue
		Dequeue(aux, ae_1, size);
		Enqueue(c, ae_1, size);
	}
	free(aux);
	free(ae_1);	
}

void wait(void *c, int bandwidth, int sec, List **finished, int size) {
	// whole dimension for downloading
	int dim = bandwidth * sec; 
	void *ae = malloc(size);
	if(ae ==  NULL)
		return;
	while(((Coada *)c)->front != NULL) {
		// the whole dim is downloaded
		if(dim == 0)
			return;
		// get the first element of the priority queue
		Dequeue(c, ae, size);
		int rem_size = ((Resource *)ae)->dimension 
		- ((Resource *)ae)->currently_downloaded;
		/* the whole dim is downloaded, but the 
		resources still has remaining size for downlading */
		if(rem_size > dim) {
			((Resource *)ae)->currently_downloaded += dim;
			dim = 0;
			// the resource is added back to the queue
			download(c, ae, size);
			// the resource is downloaded completely
		} else if(rem_size <= dim) {
			dim -= rem_size;
			((Resource *)ae)->currently_downloaded 
			= ((Resource *)ae)->dimension;
			// it is added 
			Finished(finished, ae, size);
		}
	}
}
// print downloaded resources 
void downloads(void *c, int size, List *head, FILE *fw) {
	DisplayPQueue(c, size, fw);
	print_finished(head, fw);
}

int main(int argvc, char* argv[]) {
	// no input or output file 
	if(argvc < 3) 
		return -1;

	FILE *fr = fopen(argv[1], "r");
	FILE *fw = fopen(argv[2], "w");

	char *command = malloc(20), url[21];
	int bandwidth = 1024, cur_tab_index, flag = 0, flag_down = 0;
	// empty tab without page 
	Tab *tab = allocTab();
	Tab *current_tab = change_tab(tab, size(tab));
	// index of the current tab
	cur_tab_index = size(tab);
	void *history = InitQ(sizeof(Page));
	void *pri_down = InitQ(sizeof(Resource));
	List *finished = NULL;
	while (fscanf(fr,"%s", command) == 1) {
		// scan the command and call the functions 
		if (!strcmp(command, "set_band")) {
			fscanf(fr,"%d", &bandwidth);
		} else if (!strcmp(command, "newtab")) {
			newtab(&tab);
			current_tab = change_tab(tab, size(tab));
			cur_tab_index = size(tab);
		} else if (!strcmp(command, "deltab")) {
			// current element is the last element 
			if (cur_tab_index == size(tab)) {
				flag = 1;
			}
			deltab(tab);
			/* index of the current element is the 
			new size of the tab list*/ 
			if (flag == 1) {
				current_tab = change_tab(tab, size(tab));
				cur_tab_index = size(tab);
				flag = 0;
			}
			/*if flag == 0 the current tab is not the last element, 
			the last element is deleted, but the current element is 
			still the same*/
		} else if (!strcmp(command, "change_tab")) {
			fscanf(fr,"%d", &cur_tab_index);
			current_tab = change_tab(tab, cur_tab_index);
			cur_tab_index++;
		} else if (!strcmp(command, "print_open_tabs")) {
			printTabs(tab, fw);
		} else if (!strcmp(command, "goto")) {
			fscanf(fr,"%s", url);
			Page *page = go_to(tab, cur_tab_index, url);
			Enqueue(history, page, sizeof(Page));
			if (flag_down == 1) {
				/* resource is added for downloading
				 time while the page is loaded is 1 second */
				wait(pri_down, bandwidth, 1, &finished, sizeof(Resource));
			}
		} else if (!strcmp(command, "back")) {
			back(tab, cur_tab_index, sizeof(Page), fw);
		} else if (!strcmp(command, "forward")) {
			forward(tab, cur_tab_index, sizeof(Page), fw);
		} else if (!strcmp(command, "history")) {
			DisplayQueue(history, sizeof(Page), fw);
		} else if (!strcmp(command, "del_history")) {
			int nr_entries;
			fscanf(fr, "%d", &nr_entries);
			DeleteQueue(history, nr_entries, sizeof(Page));
		} else if (!strcmp(command, "list_dl")) {
			list_dl(tab, cur_tab_index, fw);
		} else if (!strcmp(command, "downloads")) {
			downloads(pri_down, sizeof(Resource), finished, fw);
		} else if (!strcmp(command, "download")) {
			Tab *temp = tab;
			int res_index;
			fscanf(fr, "%d", &res_index);
			int temp_index = cur_tab_index;
			// go to the current tab
			while (temp_index > 1) {
				temp_index--;
				temp = temp->next;
			}
			// no loaded paage
			if(temp->current_page == NULL) {
				continue;
			}
			// flag_down = 1 means resources is added for downloading 
			flag_down = 1;
			void *ae = &((Page *)temp->current_page)->resources[res_index];
			// add the resources to the priority queue
			download(pri_down, ae, sizeof(Resource));
		} else if (strcmp(command, "wait") == 0) {
			int seconds;
			fscanf(fr, "%d", &seconds);
			wait(pri_down, bandwidth, seconds, &finished, sizeof(Resource));
		}
	}

	DeleteQueue(pri_down, 0, sizeof(Resource));
	free(pri_down);
	DeleteQueue(history, 0, sizeof(Page));
	free(history);
	free(current_tab);
	free(command);
return 0;
}
