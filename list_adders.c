/*
 * Emilio Montelongo Luna
 * 11317059
 * rjq760
 * CMPT 332 Fall-2026
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
 * Purpose:     Adds a new item to the list directly after the current item, 
 *              and makes the new item the current item.
 * Parameters:  list - pointer to the target LIST
 *              item - pointer to the item being added
 * Returns:     0 on success, -1 on failure
 */
int ListAdd(LIST *list, void *item) {
	NODE *new_node;
	NODE *next_node;


    	if (list == NULL || item == NULL) {
        	return -1;
    }
    
       	if (free_node == NULL) {
		return -1;
	}
	new_node = free_node;
	free_node = free_node->next;
	
	new_node->item = item;
	new_node->in_use = 1;
	
	if (list->head == NULL) {
		new_node->prev = NULL;
		new_node->next = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		next_node = list->curr->next;
		new_node->prev = list->curr;
		new_node->next = next_node;
		list->curr->next = new_node;
		
		if (next_node != NULL) {
			next_node->prev = new_node;
		} else {
			list->tail = new_node;
		}
	}
	list->curr = new_node;
	list->count++;

    return 0;
}

/*
 * Purpose:     Inserts a new item to the list directly before the current item
 *              and makes the new item the current item.
 * Parameters:  list - pointer to the target LIST
 *              item - pointer to the item being inserted
 * Returns:     0 on success, -1 on failure
 */
int ListInsert(LIST *list, void *item) {

	NODE *new_node;
	NODE *prev_node;



   	if (list == NULL || item == NULL) {
        	printf("Error in procedure ListInsert: invalid parameter\n");
        	return -1;
    }
   
    
    
    	if (free_node == NULL) {
		return -1;
	}
	new_node = free_node;
	free_node = free_node->next;

	new_node->item = item;
	new_node->in_use = 1;
	if (list->head == NULL) {
		new_node->prev = NULL;
		new_node->next = NULL;
		list->head = new_node;
		list->tail = new_node;
	} else {
		prev_node = list->curr->prev;
		new_node->next = list->curr;
		new_node->prev = prev_node;
		list->curr->prev = new_node;
		
		if (prev_node != NULL) {
			prev_node->next = new_node;
	} else {
		list->head = new_node;
	}
	}
	list->curr = new_node;
	list->count++;
	
	
    return 0;
}

/*
 * Purpose:     Adds a new item to the end of the list,
 *              and makes the new item the current item.
 * Parameters:  list - pointer to the target LIST
 *              item - pointer to the item being appended
 * Returns:     0 on success, -1 on failure
 */
int ListAppend(LIST *list, void *item) {
	
	NODE *new_node;

	if (list == NULL || item == NULL) {
        
        	return -1;
    }
  	

	if (free_node == NULL) {
		return -1;
	}
	new_node = free_node;
	free_node = free_node->next; 
	
	new_node->item = item;
	new_node->next = NULL;
	new_node->prev = list->tail;
	new_node->in_use = 1;
	

	if (list->tail == NULL) {
		list->head = new_node;
	} else {
		list->tail->next = new_node;
	}
	list->tail = new_node;
	list->curr = new_node;
	list->count++;
		
    
    return 0;
}

/*
 *              and makes the new item the current item.
 * Parameters:  list - pointer to the target LIST
 *              item - pointer to the item being prepended
 * Returns:     0 on success, -1 on failure
 */
int ListPrepend(LIST *list, void *item) {
	NODE *new_node;
	
    	if (list == NULL || item == NULL) {
        	return -1;
    }
  	
    	if (free_node == NULL) {
		return -1;
	}
	new_node = free_node;
	free_node = free_node->next;
    
   	                                                                       
                                                                                
        new_node->item = item;                                                  
        new_node->prev = NULL;                                                  
        new_node->next = list->head;                                            
        new_node->in_use = 1;                                                   
                                                                                
                                                                                
        if (list->head == NULL) {                                               
                list->tail = new_node;                                          
        } else {                                                                
                list->head->prev = new_node;                                    
        }                                                                       
        list->head = new_node;                                                  
        list->curr = new_node;                                                  
        list->count++;                                                          
                                                                                
                                                                                  
    return 0;
}
