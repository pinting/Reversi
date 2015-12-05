#include "list.h"

List *list_new(int key, int value)
{
	List *first;

	first = malloc(sizeof(List));
	first->key = key;
	first->value = value;
	first->next = NULL;
	first->prev = NULL;

	return first;
}

List *list_get(List *list, int key)
{
	list_seek_start(&list);

	for (; list != NULL; list = list->next)
	{
		if (list->key == key)
		{
			return list;
		}
	}

	return NULL;
}

int list_value(List *list, int key)
{
	List *element;

	element = list_get(list, key);

	if (element != NULL)
	{
		return element->value;
	}

	return 0;
}

void list_add(List *list, int key, int value)
{
	List *next;

	list_seek_end(&list);

	next = list_new(key, value);
	next->prev = list;
	list->next = next;
}

void list_seek_start(List **list)
{
	for (; (*list)->prev != NULL; *list = (*list)->prev);
}

void list_seek_end(List **list)
{
	for (; (*list)->next != NULL; *list = (*list)->next);
}

void list_free(List *list)
{
	list_seek_start(&list);

	while (list != NULL)
	{
		free(list->prev);
		list = list->next;
	}
}