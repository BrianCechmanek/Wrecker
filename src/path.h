#pragma once

#include <stdbool.h>

#include "map.h"
#include "utarray.h"

// TODO: Add support for custom map type.
typedef struct PathData
{
    int x0, y0;
    int dx, dy;
    Map_t *map;

    UT_array *path;
    float *distance;
    float *aStar;
    int *prev;
    UT_array *heap; // min-heap for pathfinding
} PathData;

/*
 *  path_new: Generates required information to start pathfinding.
 *
 *      TODO: Need to add support for multiple maps, currently 
 *      everything is limited to the map an entity is in.
 *      Will probably need a custom pathfinding function for that in order
 *      to combine maps.
 */
PathData *path_new( Map_t *map );
void path_delete( PathData *data ); 

/*
 *  path_compute:
 *      Computes the fastest path between (x0,y0) and (x1, y1).
 *      Uses A*, current heuristic is kind of slow.
 */
bool path_compute( PathData *data, int x0, int y0, int dx, int dy );

/*
 *  path_step:
 *      Walks a single step down our path.
 *      gives the option recalculate path if it is suddenly obstructed.
 */
bool path_step( PathData *data, int *x, int *y, bool recalc );

/* helper functions */
int path_length( PathData *data );
bool path_isEmpty( PathData *data);
void path_atIndex( PathData *data, int index, int *x, int *y);

/*
 *  TODO: Add dijkstra maps?
 */
