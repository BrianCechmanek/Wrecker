#include "ships.h"

Stack newStack(void){
    Stack theStack;
    theStack.size = 0;
    return theStack;
}

bool stackPush( void *ent, int type, Stack *theStack)
{
    if ( theStack->size == 500  ) {
        printf("Stack Full!");
        return false;
    }

    int size = theStack->size;
    theStack->contents[size].object = ent;
    theStack->contents[size].type = type;
    theStack->size += 1;
    return true;
}

Entity *stackPop( Stack *theStack )
{
        if (!theStack->size){
            return NULL;
        }

        int size = theStack->size - 1;
        Entity *new = &theStack->contents[size];
        theStack->size -= 1;
        
        return new;
}

