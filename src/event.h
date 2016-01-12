/*
    ezEvent.h
    based on 'Emitter' from clib.
    v.01 David Bergeron 2016
 */

#ifndef _EZ_EVENT_H
#define _EZ_EVENT_H

#include "dataStructures.h"

/*
 * Callback Type
 */

//typedef void (event_cb)(void *data);

typedef int (*event_cmp)(const char *a, const char *b);

/*
 * Emitter type:
 *  Listeners
 *  Comparison Function
 */

typedef struct event_s
{
    List *listeners;
    event_cmp cmp;
} Event_s;

/*
 * Create event system.
 */

Event_s *newEvent_s(void);

/*
 * freeEventSystem
 */

void freeEvent_s( Event_s *self );

/*
 * Transmit an 'Event' from system, passing 'data' to listeners.
 */

int sendEvent( Event_s *self, const char *event, void *data);

/*
 * Register event callback for event
 * event_on registers event for future events.
 * event_once registers event for one time use.
 */

int event_on( Event_s *self, const char *event, void *cb);
int event_once( Event_s *self, const char *event, void *cb);

/*
 * Remove listener from event.
 */

int event_off( Event_s *self, const char *event, void *cb);

#endif
