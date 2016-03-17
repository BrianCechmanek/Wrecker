#ifndef EVENT_H
#define EVENT_H
/*
    ezEvent.h
    based on 'Emitter' from clib.
    v.01 David Bergeron 2016
 */
#include "uthash.h"

typedef enum {
    LISTEN_ON,
    LISTEN_ONCE
} listenDuration;

struct callback{
    void (*fn)();
    listenDuration type;
    struct callback *next;
};

typedef struct listener_t
{
    const char *event;  // Also Key for hash lookup.
    struct callback *cb;       // Listen callback list
    UT_hash_handle hh;
} listener_t;

/*
 * Callback Type
 */

typedef int (*event_cmp)(const char *a, const char *b);

/*
 * Emitter type:
 *  Listeners
 *  Comparison Function
 */
typedef struct eventBase 
{
    listener_t *listeners;
    event_cmp cmp;
} eventBase;

/*
 * Create event system.
 */
eventBase *newEventBase(void);

/*
 * freeEventSystem
 */
void freeEventBase( eventBase *self );

/*
 * Transmit an 'Event' from system, passing 'data' to listeners.
 */
int sendEvent( eventBase *self, const char *event, void *data);

/*
 * Register event callback for event
 * event_on registers event for future events.
 * event_once registers event for one time use.
 */
int event_on( eventBase *self, const char *event, void *cb);
int event_once( eventBase *self, const char *event, void *cb);

/*
 * Remove listener from event.
 */
int event_off( eventBase *self, const char *event, void *cb);

//Custom function just to keep all Event files tucked away here and out of main.
void initEventCenter(void);
/*
 * Global EventBase.
 */
extern eventBase *eventCenter;
#endif
