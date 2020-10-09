#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q != NULL) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
        return q;
    }
    return NULL;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (q == NULL)
        return;
    list_ele_t *tmp = q->head;
    while (tmp) {
        list_ele_t *tmp2 = tmp->next;
        free(tmp->value);
        free(tmp);
        tmp = tmp2;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (q == NULL || s == NULL)
        return false;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    int s_len = strlen(s);
    char *c = (char *) malloc(sizeof(char) * (s_len + 1));
    if (c == NULL) {
        free(newh);
        return false;
    }
    strncpy(c, s, s_len);
    c[s_len] = 0;
    newh->next = q->head;
    newh->value = c;
    q->head = newh;
    if (q->size == 0) {
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL || s == NULL)
        return false;
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (newh == NULL)
        return false;
    newh->next = NULL;
    int s_len = strlen(s);
    char *c = (char *) malloc(sizeof(char) * (s_len + 1));
    if (c == NULL) {
        free(newh);
        return false;
    }
    strncpy(c, s, s_len);
    c[s_len] = 0;
    newh->value = c;
    if (q->size == 0) {
        q->head = newh;
        q->tail = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->size == 0 || sp == NULL)
        return false;
    list_ele_t *item = q->head;
    q->head = q->head->next;
    if (item->value != NULL) {
        int size =
            strlen(item->value) > bufsize ? bufsize - 1 : strlen(item->value);
        memset(sp, 0, bufsize);
        strncpy(sp, item->value, size);
        free(item->value);
    }
    free(item);
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size == 0)
        return;
    list_ele_t **head = &(q->head);
    list_ele_t *cursor = NULL;
    q->tail = q->head;
    while ((*head) != NULL) {
        list_ele_t *next = (*head)->next;
        (*head)->next = cursor;
        cursor = *head;
        *head = next;
    }
    *head = cursor;
}

void find_mid_and_split(list_ele_t *q,
                        list_ele_t **start,
                        list_ele_t **after_mid)
{
    list_ele_t *slow, *fast;
    if (q == NULL) {
        return;
    }
    slow = q;
    fast = q->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *start = q;
    *after_mid = slow->next;
    slow->next = NULL;
}

void merge(queue_t **q, list_ele_t **a, list_ele_t **q1, list_ele_t **q2)
{
    list_ele_t *tmp;
    list_ele_t *i = *q1;
    list_ele_t *j = *q2;
    list_ele_t *k = NULL;

    if (strcmp(i->value, j->value) < 0) {
        tmp = i;
        i = i->next;
    } else {
        tmp = j;
        j = j->next;
    }
    k = tmp;
    while (i != NULL && j != NULL) {
        if (strcmp(i->value, j->value) < 0) {
            k->next = i;
            i = i->next;
        } else {
            k->next = j;
            j = j->next;
        }
        k = k->next;
    }
    while (i != NULL) {
        k->next = i;
        i = i->next;
        k = k->next;
    }
    while (j != NULL) {
        k->next = j;
        j = j->next;
        k = k->next;
    }
    (*a) = tmp;
    (*q)->head = tmp;
    (*q)->tail = k;
}

void merge_sort(queue_t **q, list_ele_t **a)
{
    if (*a == NULL || (*a)->next == NULL) {
        return;
    }
    list_ele_t *s, *m;
    find_mid_and_split(*a, &s, &m);
    merge_sort(q, &s);
    merge_sort(q, &m);
    merge(q, a, &s, &m);
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q == NULL || q->size == 0 || q->head == NULL || q->head->next == NULL)
        return;
    merge_sort(&q, &(q->head));
}
