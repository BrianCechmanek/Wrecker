#ifndef _MAP_H
#define _MAP_H
/*
 *  Wrecker
 *  map.h
 */

#define Fl(N)   (1 << (N))

enum tile_type{
    NONE,
    WALL, 
    FLOOR,
};

//TODO: Build system that loads tile Types into an indexed array.
typedef struct Tile 
{
    unsigned short tileLayer[3]; // Made up of tileTypes.
    unsigned long tileFlags;
    short volume;       //For tracking oxygen levels

    bool walkable;
    bool transparent;
} Tile;

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

#endif
