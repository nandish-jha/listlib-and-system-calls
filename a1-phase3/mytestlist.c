/*
AAKASH JANA AAJ284 11297343
NANDISH JHA NAJ474 11282001
*/

#include <list.h>


/* We are going to declare a fixed pool of memory for the LISTs and NODEs */
NODE pool_nodes[MAX_NODES];
LIST pool_lists[MAX_LISTS];
int free_node_head = 0;  /* head of free-node of NODEs */
int free_list_head = 0;  /* head of free-list of LISTs */
int initialized = 0;      /* 0 if uninitialized else 1 */

/* Comparator function for integers */
int intListItemComparator(void *item, void *comparisonArg)
{
    if (*(int *)item == *(int *)comparisonArg) 
    {
        return 1;
    } 
    else 
    {
        return 0;
    }
}

/* Function to free integer pointer */
void free_int(void *p) 
{ 
    printf("   Freeing item: %d\n", *(int *)p);
    free(p);
}

int *heap_int(int v) 
{ 
    int *p = malloc(sizeof *p);
    if (p) *p = v; 
    return p;
}

int main(int argc, char const *argv[])
{
    LIST *testList, *list2;
    int count = 0, result = 0, i;
    int item1 = 100, item2 = 200, item3 = 300, item4, item5;
    int item6 = 350, item7 = 400, item8 = 450, item10 = 0;
    int tests_passed = 0;
    void *current, *removed, *found;
    int *new_val;
    printf("=== Part C List Implementation Test Program ===\n");

    /* Test 1: Create a list */
    printf("\n1. Testing ListCreate():\n");
    testList = ListCreate();
    list2 = ListCreate(); /* To check multiple list creation*/
    if (testList != NULL) 
    {
        tests_passed++;
        printf("   SUCCESS: List created\n");
    } 
    else 
    {
        printf("   FAILED: List creation failed\n");
        return 1;
    }
    if (list2 != NULL) 
    {
        tests_passed++;
        printf("   SUCCESS: List created\n");
    } 
    else 
    {
        printf("   FAILED: List creation failed\n");
        return 1;
    }

    /* Test 2: Check initial count */
    printf("\n2. Testing ListCount() on empty list:\n");
    count = ListCount(testList);
    if (count == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: Initial count is 0\n");
    } 
    else 
    {
        printf("   FAILED: Initial count is %d, expected 0\n", count);
    }

    /* Test 3: Test adding items */
    printf("\n3. Testing ListAppend():\n");
    result = ListAppend(testList, heap_int(item1));
    if (result == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: Item appended\n");
    } 
    else 
    {
        printf("   FAILED: Item append failed\n");
    }

    result = ListAppend(testList, heap_int(item2));
    if (result == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: 2nd Item appended\n");
    } 
    else 
    {
        printf("   FAILED: 2nd Item append failed\n");
    }

    result = ListAppend(testList, heap_int(item3));
    if (result == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: 3rd Item appended\n");
    } 
    else 
    {
        printf("   FAILED: 3rd Item append failed\n");
    }

    /* Test 4: Check count after additions */
    printf("\n4. Testing ListCount() after additions:\n");
    count = ListCount(testList);
    if (count == 3) 
    {
        tests_passed++;
        printf("   SUCCESS: Count after 3 appends is 3\n");
    } 
    else 
    {
        printf("   FAILED: Count after 3 appends is %d, expected 3\n", count);
    }

    /* Test 5: Test navigation */
    printf("\n5. Testing navigation functions:\n");
    current = ListFirst(testList);
    if (current != NULL) 
    {
        tests_passed++;
        printf("   ListFirst successful\n");
        printf("   current: %d\n", *(int*)current);
    }

    current = ListLast(testList);
    if (current == NULL) 
    {
        printf("   Error: ListLast returned NULL\n");
    }
    else 
    {
        tests_passed++;
        printf("   SUCCESS: ListLast successful\n");
        printf("   current: %d\n", *(int*)current);
    }

    current = ListNext(testList);
    if (current != NULL) 
    {
        printf("   ListNext: %d\n", *(int*)current);
    } 
    else
    {
        tests_passed++;
        printf("   ListNext: NULL (at end)\n");
    }

    /* Test 6: Test insertion */
    printf("\n6. Testing ListInsert():\n");
    item4 = 150;
    ListFirst(testList); /*Position at first item*/
    current = ListNext(testList);  /*Position at second item*/
    printf("   ListNext: %d\n", *(int*)current);
    result = ListInsert(testList, heap_int(item4)); /*Insert before current*/
    if (result != 0)
    {
        printf("   Error: ListInsert failed\n");
    }
    else 
    {
        tests_passed++;
        printf("   SUCCESS: ListInsert result: %d\n", result);
    }
   
    /* Test 7: Test insertion */
    printf("\n6. Testing ListPrepend():\n");
    item5 = 95;
    result = ListPrepend(testList, heap_int(item5)); /*Insert at front*/
    current = ListFirst(testList); /*Position at first item*/
    if (*(int*)current != 95)
    {
        printf("   Error: ListPrepend did not add item at front\n");
    }
    else
    {
        tests_passed++;
        printf("   SUCCESS: ListPrepend result: %d\n", result);
    }
    
    /* Test 8: Test insertion in 2nd list */
    printf("\n8. Testing ListAppend on 2nd List :\n");
    result = ListAppend(list2, heap_int(item6));
    result = ListAppend(list2, heap_int(item7));
    result = ListAppend(list2, heap_int(item8));
    if (list2->count != 3)
    {
        printf("   Error: ListAppend on 2nd List failed\n");
    }
    else 
    {
        tests_passed++;
        printf("   SUCCESS: ListAppend on 2nd List count: %d\n", list2->count);
    }
    printf("   ListAppend on 2nd List result: %d\n", result);
    
    /* Test 9: Test Concatenation of two lists */
    printf("\n9. Testing ListConcat w/testList and list2 :\n");
    ListConcat(testList, list2);
    result = ListCount(testList);
    printf("   ListConcat w/testList and list2, count result: %d\n", result);
    if (result != 8 || list2->count != 0)
    {
        printf("   Error: ListConcat did not concatenate properly\n");
    }
    else 
    {
        tests_passed++;
        printf("   SUCCESS: ListConcat concatenated properly\n");
    }

    /* Test 10: Test moving backward in list */
    printf("\n10. Testing ListPrev :\n");
    ListLast(testList); /*Position at last item*/
    for (i = 0; i < 3; i++) 
    {
        current = ListPrev(testList);
        if (current != NULL) 
        {
            printf("   ListPrev: %d\n", *(int*)current);
        } 
        else 
        {
            printf("   ListPrev: NULL \n");
        }
    }
    
    /* Test 11: Test insertion in 2nd list */
    printf("\n11. Testing ListAdd :\n");
    for (i = 1; i <= 3; i++) 
    {   
        new_val = heap_int(i*1000);
        result = ListAdd(testList, new_val);
        if (result != -1) 
        {
            printf("   ListAdd: %d\n", *(int *)new_val);
        } 
        else 
        {
            printf("   ListAdd: Failed\n");
        }
    }
    if (testList->count != 10)
    {
        printf("   Error: ListAdd did not add items properly\n");
    }
    else 
    {
        tests_passed++;
        printf("   SUCCESS: ListAdd added items properly (upto MAX_LIMIT), \
count: %d\n", testList->count);
    }
    printf("  Current count of items in list: %d\n", testList->count);
    if (testList->count > 10)
    {
        printf("   Error: List count increased over MAX_LIMIT (10) \n");
    }

    /* Test 12: Test search */
    printf("\n12. Testing search functionality:\n");
    current = ListFirst(testList);
    printf("\t a. Testing search for item present in list: %d\n", item3);
    if (current != NULL) 
    {   
        found = ListSearch(testList, intListItemComparator, &item3);
        if (found != NULL && *(int*)found == item3) 
        {
            tests_passed++;
            printf("   ListSearch successful for item %d\n", item3);
            printf("   found: %d\n", *(int*)found);
        } 
        else 
        {
            printf("   Error: ListSearch did not find item %d\n", item3);
        }
    }

    printf("\t 12b. Testing if cursor is left at the found item after search :\
%d\n", item3);
    current = ListCurr(testList);
    if (current != NULL && *(int*)current == item3)
    {
        tests_passed++;
        printf("   As Expected : Cursor is at found position after search\n");
        printf("   current: %d\n", *(int*)current);
    } 
    else 
    {
        printf("   Error: Cursor not at last position after search \n");
    }

    printf("\t 12c. Testing search for item not present in list: 750\n");
    current = ListFirst(testList);
    if (current != NULL) 
    {   
        item10 = 750;
        found = ListSearch(testList, intListItemComparator, &item10);
        if (found == NULL) 
        {
            tests_passed++;
            printf("   As Expected : ListSearch unsuccessful for item %d\n",\
 item10);
        } 
        else 
        {
            printf("   Error: ListSearch did not return NULL \n");
        }
    }

    printf("\t 12d. Testing if cursor is left at last after search for absent\
 item : 750\n");
    current = ListCurr(testList);
    if (current == NULL && testList->after == 1) 
    {
        tests_passed++;
        printf("   As Expected : Cursor is at last position after search\n");
    } 
    else 
    {
        printf("   Error: Cursor not at last position after search \n");
    }

    /* Test 13: Test removal */
    printf("\n13. Testing ListRemove():\n");
    current = ListFirst(testList); /*Position at first item*/
    current = ListNext(testList);
    current = ListNext(testList);
    current = ListNext(testList);
    count = ListCount(testList);
    removed = ListRemove(testList);
    if (removed != NULL)
    {
        printf("   Removed item: %d\n", *(int*)removed);
    } 
    else 
    {
        printf("   Nothing removed\n");
    }
    current = ListFirst(testList);
    if ((count-1 ) == ListCount(testList) && \
    ListSearch(testList, intListItemComparator, removed) == NULL)
    {
        tests_passed++;
        printf("   SUCCESS: ListRemove removed item properly,\
 new count: %d\n",ListCount(testList));
    }
    else 
    {
        printf("   Error: ListRemove did not remove item properly,\
prev count : %d, current count: %d\n",\
         count, ListCount(testList));
    }

    /* Test 14: Test trimming */
    printf("\n14. Testing ListTrim():\n");
    count = ListCount(testList);
    printf("   Current count before trim: %d\n", count);
    removed = ListTrim(testList);
    current = ListFirst(testList);
    if (removed != NULL && (count-1) == ListCount(testList) && \
    ListSearch(testList, intListItemComparator, removed) == NULL)
    {
        tests_passed++;
        printf("   SUCCESS: ListTrim trimmed item properly, new count: %d\n",\
         ListCount(testList));
        printf("   Trimmed item: %d\n", *(int*)removed);
    } 
    else 
    {
        printf("   Error : Trim operation failed!\n");
    }


    /* Test 15: Final cleanup */
    printf("\n15. Testing ListFree():\n");
    ListFree(testList, free_int);
    printf("   List freed successfully\n");
    if (free_list_head == 0 )
    {
        tests_passed++;
        printf("   SUCCESS: ListFree added list back to free list\n");
    } 
    else 
    {
        printf("   Error: ListFree did not add list back to free list\n");
    }

    if (tests_passed == 22) 
    {
        printf("\nAll %d tests passed successfully!\n",tests_passed);
    } 
    else 
    {
        printf("\nSome tests failed. Tests passed: %d/22\n", tests_passed);
    }

    printf("\n=== Test Program Complete ===\n");
    return 0;
}
