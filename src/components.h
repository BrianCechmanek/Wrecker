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

typedef struct Map_c
{
    EID parent;
    char *title;
    int height;
    int width;
    Tile *map;
} Map_c;

typedef struct position_c 
{
	float x, y;
    Map_c *map;
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
