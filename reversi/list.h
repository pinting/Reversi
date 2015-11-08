#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct List
{
	int key;
	int value;
	struct List *next;
	struct List *prev;
} List;

List *list_new(int key, int value);
List *list_get(List *list, int key);
int list_value(List *list, int key);
void list_add(List *list, int key, int value);
void list_seek_start(List **list);
void list_seek_end(List **list);
void list_free(List *list);

#endif
