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
    if (list == NULL) {
        printf("Error in procedure ListRemove: invalid parameter\n");
        return NULL;
    }
    printf("Got to procedure ListRemove\n");
    return NULL;
}

/*
 * Purpose:     Deletes a list entirely. Invokes itemFree on every item inside.
 * Parameters:  list     - pointer to the target LIST
 *              itemFree - pointer to a routine that frees an individual item
 * Returns:     None
 */
void ListFree(LIST *list, ItemFree itemFree) {
    if (list == NULL || itemFree == NULL) {
        printf("Error in procedure ListFree: invalid parameter\n");
        return;
    }
    printf("Got to procedure ListFree\n");
}

/*                                                                              
 * Purpose:     Returns the last item and takes it out of the list.             
 *              The new last item becomes the current item.                     
 * Parameters:  list - pointer to the target LIST                               
 * Returns:     Pointer to the trimmed item, or NULL on failure                 
 */                                                                             
void *ListTrim(LIST *list) {                                                    
                                                                                
        if (list == NULL) {                                                     
                printf("Error in procedure ListTrim: invalid parameter\n");     
                return NULL;                                                    
    }                                                                           
        printf("Got to procedure ListTrim\n");                                  
                                                                                
    /* TODO: Implement logic to remove tail node and update pointers */         
                                                                                
        return NULL;                                                            
}  


