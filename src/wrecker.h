#ifndef _WRECKER_H
#define _WRECKER_H

#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#if __WINDOWS__
	#include "wunistd.h"
#else
	#include <unistd.h>
#endif
#include "BearLibTerminal.h"
#include "dbg.h"

#include "map.h"
#include "event.h"
#include "diana.h"

#define DBUFF_MEM_WIDTH 200
#define DBUFF_MEM_HEIGHT 60

#define SCREEN_W 80
#define SCREEN_H 25
#define MS_PER_UPDATE 14
#define UPDATE_PER_FRAME 10

#define WRECK(F, ...) \
do \
{ \
    int _err_ = diana_ ## F (wreckerD, ## __VA_ARGS__); \
    if (_err_ != DL_ERROR_NONE && \
        _err_ != DL_ERROR_FULL_COMPONENT) \
    { \
        char *_fmt_ = "%s:%i diana_" #F "(wreckerD, " #__VA_ARGS__ ") -> %i\n"; \
        printf(_fmt_, __FILE__, __LINE__, _err_); \
        break; \
    } \
} while (0)

#define FLOOR_CHAR      '.'
#define SPACE_CHAR      ' '
#define WALL_CHAR       '#'
#define DOOR_CHAR       '+'
#define PLAYER_CHAR     '@'
#define TABLET_CHAR     '?'
#define SWITCH_CHAR     '/'

//  Typedefs to simplify common types
typedef unsigned int systemID;
typedef unsigned int componentID;
typedef unsigned int entID;

// Structs, probably will move these elsewhere
typedef struct cellDisplayBuffer
{
    short code;
    color_t foreColor;
    color_t backColor;
    bool needsUpdate;
	int layer;
    // char opacity;
} cellDisplayBuffer;

typedef struct gameState
{
    int currentState;
    uint64_t lastActual;
    uint64_t clock;
    uint64_t nextUpdate;
    uint64_t timeCurrent;
    int screenHeight;
    int screenWidth;
} gameState;

enum ENTITY_TYPES
{
    E_OJBECT = 0,
    E_ACTOR,
    E_SHIP,
    E_ENVIRONMENT,
        NUM_OF_ENTITY_TYPES
};

/*
 *  Globals
 */

// State Globals
extern gameState    *Wrecker;
extern struct diana *wreckerD;
extern cellDisplayBuffer **displayBuffer;

#endif
