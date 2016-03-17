#ifndef _COMPONENTS_H
#define _COMPONENTS_H
/*
 *  Wrecker
 *  components.h
 *  Components are Data; should be tucked away into a neat little struct.
 */

#include "wrecker.h"
#include "BearLibTerminal.h"

#define E_LIST_MAX 100 // TEMPORARY: TODO:REWORK TO BE EXMAPDNING BUFFER

typedef struct ModelCell ModelCell;
    
typedef struct componentEntry
{
    componentID component;
    void *data;
} componentEntry;

typedef struct ent_list_c
{
    entID ents[E_LIST_MAX];
    short num;
}ent_list_c;

// Not sure why I added this.
typedef struct raceComponents
{
    componentEntry *components;
    int num;
}raceComponents;

typedef struct eType_c
{
    char *name;
    short type;
    bool has_map;
    bool active;
}eType_c;

typedef struct Map_c
{
    entID parent;
    char *title;
    int height;
    int width;
    Cell *map;
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
	ModelCell *Cells;
	unsigned numberOfCells;
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

extern componentID eTypeID;
extern componentID PositionID;
extern componentID VelocityID;
extern componentID RenderID;
extern componentID ModelID;
extern componentID MapID;
extern componentID RNGID;
extern componentID ent_listID;

extern const Position_c  POSITION_DEFAULT;
extern const Velocity_c  VELOCITY_DEFAULT;
extern const Render_c    RENDER_DEFAULT;

#endif
