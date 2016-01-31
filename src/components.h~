#ifndef _COMPONENTS_H
#define _COMPONENTS_H
/*
 *  Wrecker
 *  components.h
 *  Components are Data; should be tucked away into a neat little struct.
 */

#include "wrecker.h"
#include "BearLibTerminal.h"

typedef struct Position_c
{
    int x;
    int y;
} Position_c;

typedef struct Velocity_c
{
    int dx;
    int dy;
    int max;
} Velocity_c;

typedef struct Render_c
{
    int code;
    color_t *foreColor;
    color_t *backColor;
} Render_c;

typedef struct Model_c
{
    char *name;
    int height;
    int width;

    char **map;
}

extern componentID Position;
extern componentID Velocity;
extern componentID Render;
extern componentID Model;

#endif
