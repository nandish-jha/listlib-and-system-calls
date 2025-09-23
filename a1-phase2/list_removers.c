/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <list.h>


/* Creates a new empty list and returns pointer to it */
LIST *ListCreate(void)
{
    int i, freeListIdx;
    printf("Reached function: ListCreate - Creating new empty list\n");
    /* Runs only once to clean the memory pre-reserved by the pool variables */
    if (initialized == 0) 
    {
        for (i = 0; i < MAX_NODES; i++)
        {
            pool_nodes[i].next = -1;
            pool_nodes[i].prev = -1;
            pool_nodes[i].item = NULL;
            pool_nodes[i].link = i + 1;
        }
        pool_nodes[MAX_NODES - 1].link = -1; /* end of free nodes */
        for (i = 0; i < MAX_LISTS; i++)
        {
            pool_lists[i].head = -1;
            pool_lists[i].tail = -1;
            pool_lists[i].curr = -1;
            pool_lists[i].count = 0;
            pool_lists[i].before = 1;
            pool_lists[i].after = 0;
            pool_lists[i].link = i + 1;
        }
        pool_lists[MAX_LISTS - 1].link = -1; /* end of free list */
        initialized = 1;
    }
    /* Allocate a list from the pool */
    freeListIdx = free_list_head;
    if (freeListIdx == -1) 
    {
        printf("   Error: No more lists can be created. Max limit reached.\n");
        return NULL;
    }
    free_list_head = pool_lists[freeListIdx].link; /* update free list head */
    pool_lists[freeListIdx].link = -1;
    printf("   ListCreate: List created successfully\n");
    return &pool_lists[freeListIdx];
}

/*  Return current item and take it out of list */
void *ListRemove(LIST *list)
{
    printf("Reached function: ListRemove - Removing current item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListRemove\n");
        return NULL;
    }
    printf("   ListRemove: Item removed (stub implementation)\n");
    return NULL;
}

/* adds list2 to the end of list1 */
void ListConcat(LIST *list1, LIST *list2)
{
    printf("Reached function: ListConcat - Concatenating two lists\n");
    if (list1 == NULL || list2 == NULL) 
    {
        printf("   Error: NULL list passed to ListConcat\n");
        return;
    }
    if (list2->count == 0) 
    {
        /* nothing to concatenate */
        return;
    }
    pool_nodes[list1->tail].next = list2->head;
    pool_nodes[list2->head].prev = list1->tail;
    list1->tail = list2->tail;
    list1->count = list1->count + list2->count;
    list2->head = -1, list2->tail = -1, list2->curr = -1;
    list2->count = 0, list2->before = 1, list2->after = 0;

    printf("   ListConcat: Lists concatenated (stub implementation)\n");
}

/* delete list. itemFree is a pointer to a routine that frees an item */
void ListFree(LIST *list, listItemFreeFn itemFree)
{
    printf("Reached function: ListFree - Freeing list and all items\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListFree\n");
        return;
    }
    printf("   ListFree: List freed successfully (stub implementation)\n");
    return;
}

/* Return last item and take it out of list */
void *ListTrim(LIST *list)
{
    printf("Reached function: ListTrim - Removing and returning last item\n");
    if (list == NULL) 
    {
        printf("   Error: NULL list passed to ListTrim\n");
        return NULL;
    }
    printf("   ListTrim: Last item trimmed (stub implementation)\n");
    return NULL;
}
