/*
    ezEvent.c
    v.02 David Bergeron 2016
 */

#include "event.h"
#include "dbg.h"

#include <stdlib.h>
#include <string.h>

eventBase *eventCenter;
// Helper function Protos
static int addListener( eventBase *self, const char *event, void *cb, listenDuration type);
static int removeListener( eventBase *self, listener_t *listener);
/*
 * Create Listener
 */
static listener_t *newListener(const char *event, void (*fn)(), listenDuration type)
{
    listener_t *new = malloc(sizeof(listener_t));
    if (new){
        struct callback *cb = malloc(sizeof(struct callback));
        check_mem(cb);
        cb->fn = fn;
        cb->type = type;
        cb->next = NULL;

        new->event = event;
        new->cb = cb;

        return new;
    }
error:
    return NULL;
}

void initEventCenter(void)
{
    eventCenter = newEventBase();
}

/*
 * default Event name Comparator
 */
static int nameCmp( const char *a, const char *b)
{
    return strcmp(a, b);
}

/*
 * Create new Event System
 * Destroy Event System
 */
eventBase *newEventBase(void)
{
    eventBase *new = malloc(sizeof(eventBase));
    if ( new ){
        new->listeners = NULL;
        new->cmp = nameCmp;
    }
    return new;
}

void freeEventBase( eventBase *self )
{
    if (self->listeners){
        listener_t *cur, *tmp;

        HASH_ITER(hh, self->listeners, cur, tmp){
            HASH_DEL(self->listeners, cur);
            free(cur);
        }
    }
    free(self);
}

/*
 * send Event to listeners
 *
 */

int sendEvent( eventBase *self, const char *event, void *data )
{
    listener_t *listener = NULL;

    HASH_FIND_STR(self->listeners, event, listener);
    check(listener != NULL, "Failed to find event %s for event send.", event);

    struct callback *tmp, *prev;
    tmp = listener->cb;
    prev = NULL;
    
    while(tmp){
        tmp->fn(data);
        if(tmp->type == LISTEN_ONCE){
            if (prev == NULL){
                listener->cb = listener->cb->next;
                free(tmp); 
                tmp = listener->cb;
                continue;
            } else {
                prev->next = tmp->next;
                free(tmp);
                tmp = prev->next;
                continue;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    } 
    if (listener->cb == NULL){
        HASH_DEL(self->listeners, listener);
        free(listener);
    }
    
    return 0;

error:
   return 1; 
}

static int freeCallbackList( struct callback *cb)
{
    struct callback *tmp, *kill;
    tmp = cb;
    while(tmp){
        kill = tmp;
        tmp = tmp->next;
        free(kill);
    }
    return 0;
}

/*
 * Remove Listener from Event System
 */
static int removeListener(eventBase *self, listener_t *listener)
{
    freeCallbackList( listener->cb );
    HASH_DEL(self->listeners, listener);
    free(listener);
    return 0;
}

static int addListener( eventBase *self, const char *event, void *cb, listenDuration t )
{
    listener_t *listener = NULL;
    HASH_FIND_STR( self->listeners, event, listener);
    if( listener == NULL ){
        listener = newListener(event, cb, t);
        check_mem(listener);
        HASH_ADD_KEYPTR(hh, self->listeners, listener->event, strlen(event), listener);
    } else {
        struct callback *tmp = listener->cb;

        while( tmp->next != NULL ){
            tmp = tmp->next;
        }

        struct callback *newCb = malloc(sizeof(*newCb));
        check_mem(newCb);
        newCb->fn = cb;
        newCb->type = t;
        newCb->next = NULL;

        tmp->next = newCb;
    }
    //Check for duplicate key value first.
    return 0;

error:
    return 1;
}

int event_on( eventBase *self, const char *event, void *cb)
{
    return addListener(self, event, cb, LISTEN_ON);
}

int event_once( eventBase *self, const char *event, void *cb)
{
    return addListener(self, event, cb, LISTEN_ONCE);
}

int event_off( eventBase *self, const char *event, void *cb)
{
    listener_t *listener;
    HASH_FIND_STR(self->listeners, event, listener);

    if (listener){
        struct callback *cur, *prev;
        cur = listener->cb;
        prev = NULL;
        while(cur){
            if( cur->fn == cb ){
                if( prev == NULL ){
                    listener->cb = cur->next;
                    free(cur);
                    cur = listener->cb;
                    continue;
                } else {
                    prev->next = cur->next;
                }
                free(cur);
                if (prev){
                    cur = prev->next;
                }
                continue;
            }
            prev = cur;
            cur = cur->next;
        }
        if( listener->cb == NULL)
            removeListener( self, listener );
    }
    return 0;
}

