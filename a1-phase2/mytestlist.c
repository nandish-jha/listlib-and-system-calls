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

int main(int argc, char const *argv[])
{
    LIST *testList, *list2;
    int count = 0, result = 0, i;
    int item1 = 100, item2 = 200, item3 = 300, item4, item5;
    int item6 = 350, item7 = 400, item8 = 450;
    int tests_passed = 0;
    int *new_val;
    void *current, *removed;
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
    result = ListAppend(testList, &item1);
    if (result == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: Item appended\n");
    } 
    else 
    {
        printf("   FAILED: Item append failed\n");
    }

    result = ListAppend(testList, &item2);
    if (result == 0) 
    {
        tests_passed++;
        printf("   SUCCESS: 2nd Item appended\n");
    } 
    else 
    {
        printf("   FAILED: 2nd Item append failed\n");
    }

    result = ListAppend(testList, &item3);
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
    result = ListInsert(testList, &item4);
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
    result = ListPrepend(testList, &item5);
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
    result = ListAppend(list2, &item6);
    result = ListAppend(list2, &item7);
    result = ListAppend(list2, &item8);
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
        new_val = malloc(sizeof(int));
        *new_val = 400 + i*10;
        result = ListAdd(testList, new_val);
        if (result != -1) 
        {
            printf("   ListAdd: %d\n", *(int*)new_val);
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

    /* Test 12: Test removal */
    printf("\n12. Testing ListRemove():\n");
    removed = ListRemove(testList);
    if (removed != NULL)
    {
        printf("   Removed item: %d\n", *(int*)removed);
    } 
    else 
    {
        printf("   Nothing removed\n");
    }

    /* Test 13: Final cleanup */
    printf("\n13. Testing ListFree():\n");
    /* No custom free function needed for simple integers*/
    ListFree(testList, NULL);
    printf("   List freed successfully\n");

    if (tests_passed == 15) 
    {
        printf("\nAll %d tests passed successfully!\n",tests_passed);
    } 
    else 
    {
        printf("\nSome tests failed. Tests passed: %d/14\n", tests_passed);
    }

    printf("\n=== Test Program Complete ===\n");
    return 0;
}
