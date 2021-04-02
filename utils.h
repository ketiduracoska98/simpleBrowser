#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include<stdio.h>

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_

typedef struct{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

Resource* get_page_resources(const char *URL_Name, int *n);

typedef struct Page {
	char url[21];
	int num_res;
	Resource *resources;
} Page;

typedef struct Tab {
	Page *current_page;
	void *back;
	void *forward; 
	struct Tab *next;
} Tab;

Tab* allocTab();
void newtab(Tab **head_ref);
Tab* change_tab(Tab *temp, int index);
int size(Tab *temp);
void deltab(Tab *tab);
void printTabs(Tab *tab, FILE *fw);

Page* allocPage(char url[20]);
Page* go_to(Tab *tab, int cur_tab_index, char url[20]);

typedef struct List {
	void *info;
	struct List *next;
} List;

List *allocList(void *ae, int size);
void Finished(List **head_ref, void *ae, int size);
void print_finished(List *head, FILE *fw);

typedef struct {
	size_t dim;
	void *top;
} Stiva;

void* InitS(size_t d);
void push(void *s, void *ae, int size);
void pop(void *s, void *ae, int size);
void DeleteStack(void *s, int size);

typedef struct {
	size_t dim;
	void *front, *rear;
} Coada;

void* InitQ(size_t d);
void Enqueue(void *c, void *ae, int size);
void Dequeue(void *c, void *ae, int size);
void DisplayQueue(void *c, int size, FILE *fw);
void DisplayPQueue(void *c, int size, FILE *fw);
void DeleteQueue(void *c, int nr_entries, int size);

#endif 
