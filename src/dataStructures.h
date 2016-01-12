#ifndef _DATASTRUCTURES_H
#define _DATASTRUCTURES_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    void *data;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List{
    int count;
    Node *first;
    Node *last;
} List;

List *createList(void);
void listAppend( List *theList, void *data);
void listPush( List *theList, void *data);
void *removeNode( List *theList, Node *theNode);
void *listPop( List *theList );
int listEmpty( List theList);
void freeList( List *theList );

#endif
