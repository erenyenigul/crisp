#include "list.h"
#include <stdio.h>
#include <stdlib.h>

array_list *create_list(size_t initial_capacity, size_t element_size)
{
    array_list *list = malloc(sizeof(array_list));
    list->capacity = initial_capacity;
    list->size = 0;
    list->items = malloc(element_size * list->capacity);
    list->element_size = element_size;

    return list;
}

void destroy_list(array_list *list)
{
    free(list->items);
    free(list);
}

void add_list(array_list *list, void *item)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        list->items =
            realloc(list->items, list->element_size * list->capacity);
    }

    memcpy(list->items + list->size * list->element_size, item,
           list->element_size);
    list->size++;
}

void *calloc_list(array_list *list)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        list->items =
            realloc(list->items, list->element_size * list->capacity);
    }

    void *item = list->items + list->size * list->element_size;
    memset(item, 0, list->element_size);
    list->size++;
    
    return item;
}

void *get_list(array_list *list, int index)
{
    if (index < 0 || index >= list->size)
    {
        return NULL;
    }

    return ((char *)list->items) + (index * list->element_size);
}

void remove_list(array_list *list, int index)
{
    if (index < 0 || index >= list->size)
    {
        return;
    }

    memset(list->items + index * list->element_size, 0, list->element_size);
    memcpy(list->items + index * list->element_size,
           list->items + (index + 1) * list->element_size,
           (list->size - index - 1) * list->element_size);

    list->size--;
}

void pop_list(array_list *list, int index, void *item)
{
    if (index < 0 || index >= list->size)
    {
        return;
    }

    memcpy(item, list->items + index * list->element_size,
           list->element_size);
    memset(list->items + index * list->element_size, 0, list->element_size);
    memcpy(list->items + index * list->element_size,
           list->items + (index + 1) * list->element_size,
           (list->size - index - 1) * list->element_size);

    list->size--;
}

int get_size_list(array_list *list) { return list->size; }

/*
int main(void)
{

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    array_list *list = create_list(100, sizeof(expression_value));

    for(int i=0; i < 100; i++){
    expression_value v;
    v.type = V_INT;
    v.val.i = i;
    add_list(list, &v);
    }

    remove_list(list, 2);

    for(int i=0; i<99; i++){
     ((expression_value *) get_list(list, i))->val.i);
    }

     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("%d\n",)
    return 0;
}*/