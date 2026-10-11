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



#ifndef LIST_H
#define LIST_H


#define MIN_LISTS 10
#define MIN_NODES 100


typedef struct Node {
	void *item;
	struct Node *next;
	struct Node *prev;
	int in_use;
} NODE;

typedef struct List {
	NODE *head;
	NODE *tail;
	NODE *curr;
	int count;
	int in_use;
} LIST;

typedef void(*ItemFree)(void* );
typedef int(*Comparator)(void* , void* );


/* external declarataions for the memory pools for extra portion*/
extern LIST list_pool[MIN_LISTS];
extern NODE node_pool[MIN_NODES];
extern NODE *free_node;
extern int node_pool_initialized;

/* PROTOTYPES */

LIST *ListCreate(void);
int ListCount(LIST *list);
void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);
int ListAdd(LIST *list, void *item);
int ListInsert(LIST *list, void *item);
int ListAppend(LIST *list, void *item);
int ListPrepend(LIST *list, void *item);
void *ListRemove(LIST *list);
void ListConcat(LIST *list1, LIST *list2);
void ListFree(LIST *list, ItemFree itemFree);
void *ListTrim(LIST *list);
void *ListSearch(LIST *list, 
	Comparator comparator,
	void *comparisonArg);

#endif
