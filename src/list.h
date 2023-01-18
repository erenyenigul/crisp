#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void *items;
    size_t element_size;
    size_t capacity;
    size_t size;
} array_list;

array_list *create_list(size_t initial_capacity, size_t element_size);
void destroy_list(array_list *list);
void add_list(array_list *list, void *item);
void insert_list(array_list *list, void *item, int index);
void *get_list(array_list *list, int index);
void remove_list(array_list *list, int index);
int get_size_list(array_list *list);
void pop_list(array_list *list, int index, void *item);
void *calloc_list(array_list *list);
#endif