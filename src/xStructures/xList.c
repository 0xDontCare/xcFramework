#include "xStructures/xList.h"
#include <stdlib.h>  // standard library (for malloc, free)
#include "xBase/xTypes.h"

// TODO: remove dependency on stdlib.h (custom memory allocation functions)

typedef struct xListNode_s {
    void *data;
    struct xListNode_s *next;
    struct xListNode_s *prev;
} xListNode;

struct xList_s {
    xListNode *head;
    xListNode *tail;
    xSize elemSize;
    xSize listSize;
};

xList *xList_new(xSize elemSize)
{
    // validate arguments
    if (elemSize == 0) {
        return NULL;
    }

    // allocate memory for the list
    xList *list = (xList *)malloc(sizeof(xList));
    if (!list) {
        return NULL;
    }

    // initialize list attributes
    list->elemSize = elemSize;
    list->listSize = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void xList_free(xList *list)
{
    // validate arguments
    if (!list) {
        return;
    }

    // free all nodes in the list
    xListNode *current = list->head;
    while (current) {
        xListNode *next = current->next;
        free(current);
        current = next;
    }

    // make nodes inaccessible and free list descriptor
    list->head = NULL;
    list->tail = NULL;

    free(list);
}

inline xSize xList_getSize(const xList *list) { return (list) ? list->listSize : 0; }

inline xSize xList_getElemSize(const xList *list) { return (list) ? list->elemSize : 0; }

inline xBool xList_isValid(const xList *list) { return (list && list->elemSize > 0) ? true : false; }

void xList_insert(xList *list, const void *data, xSize index)
{
    // validate arguments
    if (!list || !data || index > list->listSize) {
        return;
    }

    // special cases for start and end of the list
    if (index == 0) {
        // start of the list
        xList_pushFront(list, data);
        return;
    } else if (index == list->listSize) {
        // end of the list
        xList_pushBack(list, data);
        return;
    }

    // allocate memory for new node
    xListNode *newNode = (xListNode *)malloc(sizeof(xListNode));
    if (!newNode) {
        return;
    }

    // assign data to the new node
    newNode->data = (void *)data;

    // somewhere in the middle of the list
    xListNode *current = list->head;
    for (xSize i = 0; i < index; i++) {
        current = current->next;
    }
    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    list->listSize++;
}

void *xList_get(const xList *list, xSize index)
{
    // validate arguments
    if (!list || index >= list->listSize) {
        return NULL;
    }

    // find node at specified index
    xListNode *current = list->head;
    for (xSize i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

void *xList_remove(xList *list, xSize index)
{
    // validate arguments
    if (!list || index >= list->listSize) {
        return NULL;
    }

    // special cases for start and end of the list
    if (index == 0) {
        // start of the list
        return xList_popFront(list);
    } else if (index == list->listSize - 1) {
        // end of the list
        return xList_popBack(list);
    }

    // find node at specified index
    xListNode *current = list->head;
    for (xSize i = 0; i < index; i++) {
        current = current->next;
    }

    // remove node from the list
    if (current->prev) {
        current->prev->next = current->next;
    } else {
        list->head = current->next;
    }
    if (current->next) {
        current->next->prev = current->prev;
    } else {
        list->tail = current->prev;
    }

    // save data and free memory
    void *data = current->data;
    free(current);
    list->listSize--;

    return data;
}

void xList_pushFront(xList *list, const void *data)
{
    // validate arguments
    if (!list || !data) {
        return;
    }

    // allocate memory for new node
    xListNode *newNode = (xListNode *)malloc(sizeof(xListNode));
    if (!newNode) {
        return;
    }

    // assign data to the new node
    newNode->data = (void *)data;

    // insert node into the list
    newNode->next = list->head;
    if (list->head) {
        list->head->prev = newNode;
    } else {
        list->tail = newNode;
    }
    newNode->prev = NULL;
    list->head = newNode;
    list->listSize++;
}

void xList_pushBack(xList *list, const void *data)
{
    // validate arguments
    if (!list || !data) {
        return;
    }

    // allocate memory for new node
    xListNode *newNode = (xListNode *)malloc(sizeof(xListNode));
    if (!newNode) {
        return;
    }

    // assign data to the new node
    newNode->data = (void *)data;

    // insert node into the list
    newNode->next = NULL;
    newNode->prev = list->tail;
    if (list->tail) {
        list->tail->next = newNode;
    } else {
        list->head = newNode;
    }
    list->tail = newNode;
    list->listSize++;
}

void *xList_popFront(xList *list)
{
    // validate arguments
    if (!list || !list->head) {
        return NULL;
    }

    // remove node from the list
    xListNode *current = list->head;
    list->head = current->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    // save data and free memory
    void *data = current->data;
    free(current);
    list->listSize--;

    return data;
}

void *xList_popBack(xList *list)
{
    // validate arguments
    if (!list || !list->tail) {
        return NULL;
    }

    // remove node from the list
    xListNode *current = list->tail;
    list->tail = current->prev;
    if (list->tail) {
        list->tail->next = NULL;
    } else {
        list->head = NULL;
    }

    // save data and free memory
    void *data = current->data;
    free(current);
    list->listSize--;

    return data;
}

void *xList_peekFront(const xList *list)
{
    // validate arguments
    if (!list || !list->head) {
        return NULL;
    }

    return list->head->data;
}

void *xList_peekBack(const xList *list)
{
    // validate arguments
    if (!list || !list->tail) {
        return NULL;
    }

    return list->tail->data;
}

void xList_clear(xList *list)
{
    if (!list) {
        return;
    }

    xListNode *current = list->head;
    while (current) {
        xListNode *next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->listSize = 0;
}
