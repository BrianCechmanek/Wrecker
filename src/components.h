#ifndef _COMPONENTS_H
#define _COMPONENTS_H
/*
 *  Wrecker
 *  components.h
 *  Components are Data; should be tucked away into a neat little struct.
 */

#include "wrecker.h"
#include "BearLibTerminal.h"

typedef struct position_c 
{
	float x, y;
} Position_c;

typedef struct velocity_c
{
    int dx, dy, max;
} Velocity_c;

typedef struct Render_c
{
    int code;
} Render_c;

typedef struct Model_c
{
    char *name;
    int height;
    int width;

    char **map;
} Model_c;

extern componentID PositionID;
extern componentID VelocityID;
extern componentID RenderID;
extern componentID ModelID;

#endif
