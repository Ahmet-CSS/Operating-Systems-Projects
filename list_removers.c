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

/*
 * Purpose:     Returns the current item and takes it out of the list.
 *              Makes the next item the current one.
 * Parameters:  list - pointer to the target LIST
 * Returns:     Pointer to the removed item, or NULL on failure
 */
void *ListRemove(LIST *list) {
	NODE *node_to_remove;
        void *removed_item;

        if (list == NULL || list->curr == NULL) {
		printf("Error: nothing to remove in lsit\n");
                return NULL;
		
        }

        node_to_remove = list->curr;
        removed_item = node_to_remove->item;

        if (node_to_remove->prev != NULL) {
                node_to_remove->prev->next = node_to_remove->next;
        } else {
                list->head = node_to_remove->next;
        }

        if (node_to_remove->next != NULL) {
                node_to_remove->next->prev = node_to_remove->prev;
        } else {
                list->tail = node_to_remove->prev;
        }

        /* 
         * the next item becomes the current one. If we removed
         * the tail, this naturally sets curr to NULL.
         */
        list->curr = node_to_remove->next;
        list->count--;

        /* Return the node to the free pool */
        node_to_remove->in_use = 0;
        node_to_remove->item = NULL;
        node_to_remove->next = free_node;
        free_node = node_to_remove;

        return removed_item;
}

/*
 * Purpose:     Deletes a list entirely. Invokes itemFree on every item inside.
 * Parameters:  list     - pointer to the target LIST
 *              itemFree - pointer to a routine that frees an individual item
 * Returns:     None
 */
void ListFree(LIST *list, ItemFree itemFree) {
    
    NODE *curr_node;
    NODE *next_node;
    if (list == NULL || itemFree == NULL) {
        printf("Error in procedure ListFree: invalid parameter\n");
        return;
    }
    
    printf("Got to procedure ListFree\n");
    curr_node = list->head;
        while (curr_node != NULL) {
                if (itemFree != NULL && curr_node->item != NULL) {
                        (*itemFree)(curr_node->item);
                }
                
                next_node = curr_node->next;
                
                curr_node->in_use = 0;
                curr_node->item = NULL;
                curr_node->next = free_node;
                free_node = curr_node;
                
                curr_node = next_node;
        }

        list->head = NULL;
        list->tail = NULL;
        list->curr = NULL;
        list->count = 0;
        list->in_use = 0;
}

/*                                                                              
 * Purpose:     Returns the last item and takes it out of the list.             
 *              The new last item becomes the current item.                     
 * Parameters:  list - pointer to the target LIST                               
 * Returns:     Pointer to the trimmed item, or NULL on failure                 
 */                                                                             
void *ListTrim(LIST *list) {
	NODE *tail_node;
        void *trimmed_item;
       /* 
         * THOUGHT PROCESS & FIX: 
         * The A1 specs says we cant call API functions from inside the API. 
         * The previous code called ListRemove(list) inside ListTrim.
         *  I fixed this by implementing the manual pointer manipulation 
         *  to trim the tail directly.
         */
        if (list == NULL || list->tail == NULL) {                                                     
                return NULL;                                                    
        }                                                                           
        
        tail_node = list->tail;
        trimmed_item = tail_node->item;

        if (tail_node->prev != NULL) {
                tail_node->prev->next = NULL;
                list->tail = tail_node->prev;
        } else {
                list->head = NULL;
                list->tail = NULL;
        }

        list->curr = list->tail;
        list->count--;

        tail_node->in_use = 0;
        tail_node->item = NULL;
        tail_node->next = free_node;
        free_node = tail_node;
                                                                                
        return trimmed_item;                                                                                                                        
}  


