#ifndef _MAP_H
#define _MAP_H
/*
 *  Wrecker
 *  map.h
 */

#define Fl(N)   (1 << (N))

enum tileType{
    NONE,
    WALL, 
};

typedef struct Tile 
{
    enum tileType tileLayer[3];
    unsigned long tileFlags;
    short volume;       //For tracking oxygen levels
} Tile;

enum tileFlagTypes
{
    DISCOVERED              = Fl(0),  // UNUSED
    HAS_PLAYER              = Fl(1),
    HAS_ALIEN               = Fl(2),
    HAS_SHIP                = Fl(3),
    OBSTRUCTS_MOVEMENT      = Fl(4),
    OBSTRUCTS_VISION        = Fl(5),
    IS_VACUUM               = Fl(6) // UNUSED
};

#endif
