#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/// The List struct which holds key-value pairs.
typedef struct List
{
	/// Key of the element
	int key;
	/// Value of the element
	int value;
	/// Next element.
	struct List *next;
	/// Prev element.
	struct List *prev;
} List;

/// Create a new list.
/// @param key First key.
/// @param value First value.
List *list_new(int key, int value);

/// Get an item from the list by its key.
/// @param list List struct.
/// @param key Key of the element.
List *list_get(List *list, int key);

/// Get a value from the list by its key.
/// @param list List struct.
/// @param key Key of the element.
int list_value(List *list, int key);

/// Add a new element to the list.
/// @param list List struct.
/// @param key New key value.
/// @param value Value of the element.
void list_add(List *list, int key, int value);

/// Seek to the begining of the list.
/// @param list List struct.
void list_seek_start(List **list);

/// Seek to the end of the list.
/// @param list List struct.
void list_seek_end(List **list);

/// Free the list.
/// @param list List struct.
void list_free(List *list);

#endif
