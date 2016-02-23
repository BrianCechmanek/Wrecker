/*
 * Movement.c
 * Initital movement system, mostly to be used as template.
 *
 */

#include "systems.h"

#include "ECS.h"
#include "wrecker.h"
#include "components.h"
#include "map.h"

systemID movementSystemId;

void _process(struct diana *diana, void *user_data, unsigned int entity, float delta)
{
	Position_c *position;
	Velocity_c *velocity;

	diana_getComponent(diana, entity, PositionID, (void **)&position);
	diana_getComponent(diana, entity, VelocityID, (void **)&velocity);

	position->x += velocity->dx * (delta / 1000.f);
	position->y += velocity->dy * (delta / 1000.f);

	// just testing the render stuff.
	if (position->y > 50) position->y = 0;
	if (position->x > 50) position->x = 0;

	/*
	if New Position is outside bounds of current map
	Move to Sector map.
	Are we inside the location of another map?
	Move to that map.
	*/

	/*
    if (position->x < 0 || position->x >= position->map->width ||
	position->y < 0 || position->y >=  position->map->height) {

	Position_c *parentPos;
	entID parent = position->map->parent;

	WRECK(getComponent, parent, PositionID, (void **)&parentPos);
	position->x += parentPos->x;
	position->y += parentPos->y;
	position->map = parentPos->map;

	// Check Sector to see if new Position falls into another Map.
	// checkForMap( position, parent )
	}*/
}

int movement_createSystem(struct diana *diana)
{
    int error = DL_ERROR_NONE;

    struct systemInfo info = ECS_SYSTEM_INFO_DEFAULT;
    info.name = "movement";
    info.process = _process;

    error = ecs_createSystem(diana, &info, &movementSystemId);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

    componentID components[2] = { PositionID, VelocityID };
    error = ecs_watchComponents(diana, movementSystemId, ARRAY_SIZE(components), components);
    if (error != DL_ERROR_NONE)
    {
        return error;
    }

    return error;
}


/*
 * Check live sector to see if new Position falls within the bounds of another active map.
 * Example: An alien moves out of the map for ship A, which puts him in the global map.
 *          Then we run this function and determine he is now within the bounds of ship B.
 */ 

void checkForMap( Position_c *position, entID sector )
{
    /* Poll list of entities with maps in sector.
     * Is new Position within bounds of a map?
     * calculate offset for new map coordinates
     * Update map info.
     */
     Position_c     *parentPos;
     eType_c        *type; 
     ent_list_c     *eList;


     WRECK(getComponent, sector, ent_listID, (void **)&parentPos);
     check(parentPos, "Unable to load entity list for entity: %d", sector);

     for (int i = 0; i < eList->num; i++){

        WRECK(getComponent, eList->ents[i], eTypeID, (void **)&type);
        if (type->has_map == true){

            WRECK(getComponent, eList->ents[i], PositionID, (void **)&parentPos);
            int xMax = parentPos->x + parentPos->map->width;
            int yMax = parentPos->y + parentPos->map->height;

            if (position->x >= parentPos->x && position->x < xMax
                && position->y >= parentPos->y && position->y < yMax){

                position->map = parentPos->map;
                position->x -= parentPos->x;
                position->y -= parentPos->y;
                break;
            }
        }
    }

    return;
error:
    log_info("Error updating Entity position from Map to Map.");
    exit(1);
}

