#include <stdlib.h>
#include <util.h>

LList *init_llist() {
    LList *ret = malloc(sizeof(LList));
    ret->head = NULL;
    ret->tail = NULL;
    ret->current = NULL;
    return ret;
}

void append_llist(LList *list, void *data) {
    LNode *node = malloc(sizeof(LNode));
    node->data = data;
    node->next = NULL;
    if (!list->head) {
        /* First item case */
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
}

/* N.B. The data removed, must be the exact data, we only compare the pointer */
int remove_llist(LList *list, void *data) {
    LNode *cur = list->head, *prev = NULL;
    while (cur) {
        if (cur->data == data) {
            if (prev) {
                /* If previous exists, then point to next */
                prev->next = cur->next;
            } else {
                /* Must be the list head, update head */
                list->head = cur->next;
            }
            if (cur == list->tail) {
                /* If this is the tail, then point to prev */
                list->tail = prev;
            }
            free(cur);
            return 1;
        }
        prev = cur;
        cur = cur->next;
    }
    return 0;
}

/* Start iterating a list, returns the first element */
LNode *iter_start_llist(LList *list) {
    list->current = list->head;
    return list->current;
}

/* Get the next in the list, returns the next element and updates interal state */
LNode *iter_next_llist(LList *list) {
    list->current = list->current->next;
    return list->current;
}
