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
	//diana_watch(wreckerD, movementSystem, PositionID);
	//diana_watch(wreckerD, movementSystem, VelocityID);

    WRECK(createSystem, "movement", NULL, p_Movement, NULL, NULL, NULL, NULL,
		DL_SYSTEM_FLAG_NORMAL, &movementSystem);
    s_watchComponents( movementSystem, 2, PositionID, 
                                          VelocityID );

    return 0;
}
                                          
void p_Movement(struct diana *diana, void *user_data, unsigned int entity, float delta)
{
	Position_c *position;
	Velocity_c *velocity;

	diana_getComponent(diana, entity, PositionID, (void **)&position);
	diana_getComponent(diana, entity, VelocityID, (void **)&velocity);

	position->x += velocity->dx * delta;
	position->y += velocity->dy * delta;

	//printf("%i move to (%f,%f)\n", entity, position->x, position->y);
}

