#ifndef _MAP_H
#define _MAP_H
/*
 *  Wrecker
 *  map.h
 */

typedef struct Tile 
{
    enum tileType[3];
    unsigned long tileFlags;
    short volume;       //For tracking oxygen levels
} Tile;

#endif
