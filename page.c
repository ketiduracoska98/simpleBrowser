#include "utils.h"
/* Allocate memory for the page element 
 and get its resources */
Page* allocPage(char url[20]) {
	Page *page = malloc(sizeof(Page));
	if (page == NULL)
		return NULL;
	strcpy(page->url, url);
	page->resources = get_page_resources(url, &page->num_res);
	return page;
}

Page* go_to(Tab *tab, int cur_tab_index, char url[20]) {
	// go to the  current tab 
	while (cur_tab_index > 1) {
		cur_tab_index--;
		tab = tab->next;
	}
	// if there is no page open 
	if (tab->current_page == NULL) {
		// allocate a page 
		tab->current_page = allocPage(url);
		return tab->current_page;
	} else {
		// push currently opened page to the back stack 
		push(tab->back, tab->current_page, sizeof(Page));
		// there is no forward pages
		DeleteStack(tab->forward, sizeof(Page));
		// open the  new url 
		tab->current_page = allocPage(url);
		return tab->current_page;
	}
}

