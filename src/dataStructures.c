/*
     Doubly Linked List 
     listAppend();
     listPush();
     listPop();
     listRemove();
     listshift();
*/

#include "dataStructures.h"
#include <stdio.h>
#include <stdlib.h>

List *createList(void)
{
    List *newList;
    newList =  malloc(sizeof(List));
    if( newList == NULL) exit(1);
    newList->count = 0;
    newList->first = NULL;
    newList->last = NULL;
    return newList;
}

void listAppend( List *theList, void *data)
{
    Node *newNode = calloc(1, sizeof(Node));
    newNode->data = data;

    if ( theList->last == NULL ){
        theList->last = newNode;
        theList->first = newNode;
    } else {
        newNode->next = NULL;
        newNode->prev = theList->last;
        theList->last->next = newNode;
        theList->last = newNode;
    }
    theList->count++;
}

void listPush( List *theList, void *data)
{
    Node *newNode = calloc(1, sizeof(Node));
    newNode->data = data;

    if (theList->last == NULL ) {
        theList->last = newNode;
        theList->first = newNode;
    } else {
        theList->first->prev = newNode;
        newNode->next = theList->first;
        theList->first = newNode;
    }
    theList->count++;
}

void *removeNode( List *theList, Node *theNode)
{
    if( theList->count == 0 || theNode == NULL){
        return NULL;
    }
    if( theList->count == 1 ){
        theList->first == NULL;
        theList->last == NULL;
    }
    else if ( theNode == theList->first ){
        theList->first = theNode->next;
        theList->first->prev = NULL;
    }
    else if (theNode == theList->last){
        theList->last = theNode->prev;
        theList->last->next = NULL;
    } else {
        theNode->prev->next = theNode->next;
        theNode->next->prev = theNode->prev;
    }
    theList->count--;
    return theNode;
}
    
void *listPop( List *theList )
{
    Node *theNode = theList->last;
    return (theNode != NULL? removeNode( theList, theNode ): NULL);
}

int inList( List *theList, Node *theNode )
{
    Node *tmp;
    tmp = theList->last;
    while(tmp){
        if( tmp == theNode)
            return 1;
        tmp = tmp->prev;
    }
    return 0;
}

int listEmpty( List theList)
{
    if( theList.count == 0 )
        return 1;
    return 0;
}

void copyToList( Node *oldNode, List *theList )
{
    Node *newNode = malloc(sizeof(Node));
    *newNode = *oldNode;
    listAppend( theList, newNode );
}

// DOES NOT CLEAN UP NODES, assumes they've been handed off!
void clearList( List *theList )
{
    theList->count = 0;
    theList->first = NULL;
    theList->last = NULL;
}

void freeList( List *theList )
{
    Node *tmp;
    void *data;
    while(theList->first){
        tmp = theList->first;
        theList->first = tmp->next;
        data = tmp->data;
        free(data);
        free(tmp);
    }
    free(theList);
}
