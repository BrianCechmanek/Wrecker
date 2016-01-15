/*
 * I spent too long thinking of a title for this file;
 * Didn't want to move systems into differnet files until
 * I have a better way of organizing them.
 */

#include "systems.h"

#include "wrecker.h"
#include "components.h"
#include "diana.h"

#define NORMAL_SYSTEM 0
#define PASSIVE_SYSTEM 1

extern unsigned int movementSystem;

int buildMovementSystem(void)
{
    WRECK(createSystem, "movement", NULL, processMovementSystem, NULL, NULL, NULL, NULL,
                         NORMAL_SYSTEM, &movementSystem);
    WRECK(watch, movementSystem, Position_c);
    WRECK(watch, movementSystem, Velocity_c);
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
