/* Structs */

typedef struct _LNode {
    void *data;
    struct _LNode *next;
} LNode;

typedef struct _LList {
    LNode *head;
    LNode *tail;
    /* For iterating only */
    LNode *current;
} LList;

/* Prototypes */
LList *init_llist();
void append_llist(LList *, void *);
/* N.B. The data removed, must be the exact data, we only compare the pointer */
int remove_llist(LList *, void *);

/* Start iterating a list, returns the first element */
LNode *iter_start_llist(LList *);
/* Get the next in the list, returns the next element and updates interal state */
LNode *iter_next_llist(LList *);

/* Intended Usage, for(LNode *cur = iter_start_llist(mylist); cur; cur = iter_next_llist(mylist)) */
