/*
    ezEvent.c
    v.01 David Bergeron 2016
 */

#include "event.h"

#include <stdlib.h>
#include <string.h>

typedef enum {
    LISTEN_ON,
    LISTEN_ONCE
} listenTypes;

typedef struct listener_t
{
    void (*fn)();       // Listen callback
    const char *event;  // Event to listen for
    listenTypes type;   // Listen Type
} listener_t;

// Helper function Protos
static int addListener( Event_s *self, const char *event, void *cb, listenTypes type);
static int removeListener( Event_s *self, listener_t *listener);

/*
 * Create Listener
 */
static listener_t *newListener(const char *event, void *fn)
{
    listener_t *new = malloc(sizeof(listener_t));
    if (new){
        new->fn = fn;
        new->event = event;
        new->type = LISTEN_ON;
        return new;
    }
    return NULL;
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
Event_s *newEvent_s(void)
{
    Event_s *new = malloc(sizeof(Event_s));
    if ( new ){
        new->cmp = nameCmp;
        new->listeners = createList();
    }
    return new;
}

void freeEvent_s( Event_s *self )
{
    if(self->listeners){
        freeList(self->listeners);
    }
    free(self);
}

/*
 * send Event to listeners
 */
int sendEvent( Event_s *self, const char *event, void *evData)
{
    Node *cur = self->listeners->first;
    if (cur == NULL){
        return -1;
    }

    while( cur ) {
        listener_t *listener = (listener_t *) cur->data;
        if ( self->cmp(listener->event, event) == 0 ) {
            listener->fn(evData);
            if ( listener->type == LISTEN_ONCE ) {
                cur = cur->next;
                removeListener(self, listener);
                continue;
            }
        }
        cur = cur->next;
    }
    return 0;
}

/*
 * Remove Listener from Event System
 */
static int removeListener( Event_s *self, listener_t *listener)
{
    Node *cur = self->listeners->first;
    listener_t *tmp;
    if (cur == NULL){
        return -1;
    }

    while (cur){
        if ( (listener_t *) cur->data == listener ){
            removeNode(self->listeners, cur);
            //tmp = (listener_t*)cur->data;
            free(cur->data);
            free(cur);
            break;
        }

        cur = cur->next;
    }
    return 0;
}

static int addListener( Event_s *self, const char *event, void *cb, listenTypes t)
{
    listener_t *listener = newListener(event, cb);
    if (!listener) return -1;
    listener->type = t;
    listAppend(self->listeners, listener);
    return 0;
}

int event_on( Event_s *self, const char *event, void *cb)
{
    return addListener(self, event, cb, LISTEN_ON);
}

int event_once( Event_s *self, const char *event, void *cb)
{
    return addListener(self, event, cb, LISTEN_ONCE);
}

int event_off( Event_s *self, const char *event, void *cb)
{
    listener_t *listener;
    Node *cur = self->listeners->first;
    if (cur == NULL){
        return -1;
    }

    while (cur){
        listener = (listener_t *) cur->data;
        if (strcmp(event, listener->event) == 0){

            if(cb){
                if (listener->fn == cb){
                    (void)removeNode(self->listeners, cur);
                    free(cur->data);
                    free(cur);
                    break;
                }
            } else {
                (void)removeNode(self->listeners, cur);
                free(cur->data);
                free(cur);
            }
        }
        cur = cur->next;
    }
    return 0;
}
