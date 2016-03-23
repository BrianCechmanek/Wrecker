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

PathData *path_new( Map_t *map );
bool path_compute( PathData *data, int x0, int y0, int dx, int dy );
bool path_step( PathData *data, int *x, int *y, bool recalc );
int path_length( PathData *data );
bool path_isEmpty( PathData *data);
void path_atIndex( PathData *data, int index, int *x, int *y);
void path_delete( PathData *data ); 

/*
 *  TODO: Add dijkstra maps?
 */
