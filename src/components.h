#ifndef _COMPONENTS_H
#define _COMPONENTS_H
/*
 *  Wrecker
 *  components.h
 *  Components are Data; should be tucked away into a neat little struct.
 */

#include "wrecker.h"
#include "BearLibTerminal.h"
#include "random.h"

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
	unsigned foreColor, backColor;
} Render_c;

typedef struct Model_c
{
    char *name;
    int height;
    int width;

    char **map;
} Model_c;

typedef struct Map_c
{
    entID owner;
    int height;
    int width;
    Tile *map;
} Map_c;

typedef void *RNG;
typedef struct RNG_c
{
    RNG instance;
} RNG_c;

extern componentID PositionID;
extern componentID VelocityID;
extern componentID RenderID;
extern componentID ModelID;
extern componentID MapID;
extern componentID RNGID;

#endif
