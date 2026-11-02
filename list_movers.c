/* Emilio Montelongo Luna
 * 11317059
 * rjq760
 * CMPT 332-Fall-2026
 */


/*
 Ahmet Guldadov
 Student Number: 11369772
 NSID: six402
 CMPT 332-Fall-2026
**/

#include <stdio.h>
#include <list.h>

/* Actual allocation of the memory blocks for Phase 1 and extra creidt*/
LIST list_pool[MIN_LISTS];
NODE node_pool[MIN_NODES];
NODE *free_node = NULL;
int node_pool_initialized = 0;

/*
 * Purpose:     Makes a new, empty list.
 * Parameters:  None
 * Returns:     Reference to the new LIST on success, NULL pointer on failure
 */
LIST *ListCreate(void) {
	

	int i;


	if (node_pool_initialized == 0) {
        
	for (i = 0; i < MIN_NODES - 1; i++) {
            	node_pool[i].next = &node_pool[i + 1];
           	node_pool[i].in_use = 0; 
	}

        node_pool[MIN_NODES - 1].next = NULL;
        node_pool[MIN_NODES - 1].in_use = 0;
        free_node = &node_pool[0];
        node_pool_initialized = 1;


    }
	for (i=0; i < MIN_LISTS; i++) {
	if (list_pool[i].in_use == 0) {
		list_pool[i].head = NULL;
		list_pool[i].tail = NULL;
		list_pool[i].curr = NULL;
		list_pool[i].count = 0;
		list_pool[i].in_use = 1;
		return &list_pool[i]; 
		}
	
	}
	return NULL;
}

/*
 * Purpose:     Returns the number of items currently in the list.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Integer count of items, or -1 on failure
 */
int ListCount(LIST *list) {
    	if (list == NULL) {
        	printf("Error in procedure ListCount: invalid parameter\n");
        	return -1;
    }
    	

    	return list->count;
}

/*
 * Purpose:     Returns a pointer to the first item in the list
 *              and makes it the current item.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the item, or NULL if empty/failure
 */
void *ListFirst(LIST *list) {
    	if (list == NULL || list->head == NULL) {
        	printf("Error in procedure ListFirst: invalid parameter\n");
        	return NULL;
    }


    	list->curr = list->head;
	return list->curr->item;
}

/*
 * Purpose:     Returns a pointer to the last item in the list
 *              and makes it the current item.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the item, or NULL if empty/failure
 */
void *ListLast(LIST *list) {
    	if (list == NULL) {
        	printf("Error in procedure ListLast: invalid parameter\n");
        	return NULL;
    }
    	


    	list->curr = list->tail;
	return list->curr->item;
}

/*
 * Purpose:     Advances the list's current node by one.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the new current item, or NULL if out of bounds
 */
void *ListNext(LIST *list) {

    	if (list == NULL) {
        	printf("Error in procedure ListNext: invalid parameter\n");
        	return NULL;
    }
   	 


    	list->curr = list->curr->next;
        
        if (list->curr != NULL) {
                return list->curr->item;
        }
        return NULL;
}

/*
 * Purpose:     Backs up the list's current node by one.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the new current item, or NULL if out of bounds
 */
void *ListPrev(LIST *list) {

    	if (list == NULL) {
        	printf("Error in procedure ListPrev: invalid parameter\n");
        	return NULL;
    }
    	


    	if (list->curr == NULL) {
                if (list->tail != NULL) {
                        list->curr = list->tail;
                        return list->curr->item;
                }
                return NULL; /* List is actually empty */
        }

        list->curr = list->curr->prev;
        
        if (list->curr != NULL) {
                return list->curr->item;
        }
        return NULL;
}

/*
 * Purpose:     Returns a pointer to the current item in the list.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the current item, or NULL on failure
 */


void *ListCurr(LIST *list) {
    	if (list == NULL) {
        	printf("Error in procedure ListCurr: invalid parameter\n");
        	return NULL;
    }
    	


    	return list->curr->item;
}



/*
 * Purpose:     Returns the last item and takes it out of the list.
 *              The new last item becomes the current item.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the trimmed item, or NULL on failure
 */
/*
void *ListTrim(LIST *list) {
	
    	if (list == NULL) {
        	printf("Error in procedure ListTrim: invalid parameter\n");
        	return NULL;
    }
    	printf("Got to procedure ListTrim\n");
*/
    /* TODO: Implement logic to remove tail node and update pointers */
  /*  
    	return NULL;
}
*/


/*
 * Purpose:     Searches the list starting at the current item 
 * using a comparator.
 * Parameters:  list          - pointer to the target LIST
 *              comparator    - pointer to a matching routine function
 *              comparisonArg - argument passed into the comparator
 * Returns:     Pointer to the matched item, or NULL if not found
 */
void *ListSearch(LIST *list, Comparator comparator, void *compArg){

	(void)compArg;

    	if (list == NULL || comparator == NULL || compArg == NULL) {

        	printf("Error in procedure ListSearch: invalid parameter\n");
        	return NULL;
    }
    	


    	while (list->curr != NULL) {
                if (comparator(list->curr->item, compArg) == 1) {
                        return list->curr->item;
                }
                list->curr = list->curr->next;
        }

        /* 
         * THOUGHT PROCESS & FIX: 
         * The A1 specification said that if no match is found, 
         * the current pointer must be left at the end of the list. The old
         * code left it as NULL. 
         */
        list->curr = list->tail;
        return NULL;
}

/*
 * Purpose:     Adds list2 to the end of list1. List2 ceases to exist.
 * Parameters:  list1 - pointer to the destination LIST
 *              list2 - pointer to the source LIST to be appended
 * Returns:     None
 */
void ListConcat(LIST *list1, LIST *list2) {

	if (list1 == NULL || list2 == NULL) {
                return;
        }

        if (list2->head != NULL) {
                if (list1->head == NULL) {
                        list1->head = list2->head;
                        list1->tail = list2->tail;
                } else {
                        list1->tail->next = list2->head;
                        list2->head->prev = list1->tail;
                        list1->tail = list2->tail;
                }
                list1->count += list2->count;
        }

        /* Destroy list2 making it available in the list_pool again */
        list2->head = NULL;
        list2->tail = NULL;
        list2->curr = NULL;
        list2->count = 0;
        list2->in_use = 0;


}
