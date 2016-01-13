#ifndef _COMPONENTS_H
#define _COMPONENTS_H
/*
 *  Wrecker
 *  components.h
 *  Components are Data; should be tucked away into a neat little struct.
 */

#include "BearLibTerminal.h"

typedef struct Position
{
    int x;
    int y;
} Position_c;

typedef struct Velocity
{
    int dx;
    int dy;
    int max;
} Velocity_c;

typedef struct Renderer
{
    int code;
    color_t *foreColor;
    color_t *backColor;
} Renderer_c;

#endif
