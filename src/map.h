#ifndef _MAP_H
#define _MAP_H
/*
 *  Wrecker
 *  map.h
 */
#include "wrecker.h"

#define Fl(N)   (1 << (N))

extern const int dirList[8][2];

typedef struct TileLayer
{
    int type;
    int health;
} TileLayer;

typedef struct Cell 
{
    TileLayer layers[3]; // Made up of tileTypes.
    unsigned long tileFlags;
    int volume;       //For tracking oxygen levels

    bool walkable;
    bool transparent;
} Cell;

typedef struct Map_t
{
    int height;
    int width;
    int numCells;
    Cell **grid;
} Map_t;

typedef struct Map_c
{
    entID parent;
    char *title;
    Map_t *map;
} Map_c;

//TODO: Build system that loads tile Types into an indexed array.

enum directions
{
    DIR_NORTH = 0,
    DIR_N_EAST,
    DIR_EAST,
    DIR_S_EAST,
    DIR_SOUTH,
    DIR_S_WEST,
    DIR_WEST,
    DIR_N_WEST,

    NUM_DIRECTIONS
};

enum tile_type{
    NONE,
    WALL, 
    FLOOR,
};

enum tile_flagList
{
    DISCOVERED              = Fl(0),  // UNUSED
    HAS_PLAYER              = Fl(1),
    HAS_ALIEN               = Fl(2),
    HAS_SHIP                = Fl(3),
    OBSTRUCTS_MOVEMENT      = Fl(4),
    OBSTRUCTS_VISION        = Fl(5),
    IS_VACUUM               = Fl(6) // UNUSED
};

bool map_pathableCell( Map_t *map, int x, int y );

#endif
