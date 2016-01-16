/*
 * Movement.c 
 * Initital movement system, mostly to be used as template. 
 *  
 */

#include "systems.h"

#include "wrecker.h"
#include "components.h"

#define NORMAL_SYSTEM 0
#define PASSIVE_SYSTEM 1

unsigned int movementSystem;

int initMovementSystem(void)
{
    WRECK(createSystem, "movement", NULL, p_Movement, NULL, NULL, NULL, NULL,
                         NORMAL_SYSTEM, &movementSystem);
    WRECK(watch, movementSystem, Position);
    WRECK(watch, movementSystem, Velocity);
    return 0;
}
                                          
void p_Movement( struct diana *diana, void *ud, unsigned int e, float d)
{
    Position_c     *p;
    Velocity_c    *v;
    int x0, y0;

    WRECK( getComponent, e, Position, (void **)&p);
    WRECK( getComponent, e, Velocity, (void **)&v);

    x0 = p->x; y0 = p->y;
    p->x += v->dx;
    p->y += v->dy;

    v->dx = 0;
    v->dy = 0;
    //sendEvent( eSys, "actorMoved", {x0, y0, p->x, p->y} ); 
    
}