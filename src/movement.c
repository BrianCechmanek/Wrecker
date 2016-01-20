/*
 * Movement.c 
 * Initital movement system, mostly to be used as template. 
 *  
 */

#include "systems.h"

#include "ECS.h"
#include "wrecker.h"
#include "components.h"

sysID movementSystem;

int initMovementSystem(void)
{
    WRECK(createSystem, "movement", NULL, p_Movement, NULL, NULL, NULL, NULL,
                         NORMAL_SYSTEM, &movementSystem);
    s_watchComponents( movementSystem, 2, Position, 
                                          Velocity );

    return 0;
}
                                          
void p_Movement( struct diana *diana, void *ud, entID e, float d)
{
    Position_c     *p;
    Velocity_c    *v;
//    int x0, y0;

    p = getComponent( e, Position);
    v = getComponent( e, Velocity);

//    x0 = p->x; y0 = p->y;
    p->x += v->dx;
    p->y += v->dy;

    v->dx = 0;
    v->dy = 0;
    //sendEvent( eSys, "actorMoved", {x0, y0, p->x, p->y} ); 
}

