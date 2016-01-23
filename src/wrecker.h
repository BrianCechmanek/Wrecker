#ifndef _WRECKER_H
#define _WRECKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#include "BearLibTerminal.h"
#include "dbg.h"

#include "map.h"
#include "event.h"

#define DBUFF_MEM_WIDTH 200
#define DBUFF_MEM_HEIGHT 60

#define SCREEN_W 80
#define SCREEN_H 25
#define MS_PER_UPDATE 14
#define UPDATE_PER_FRAME 10

#define WRECK(F, ...) do { int ___err = diana_ ## F (wreckerD, ## __VA_ARGS__); if(___err != DL_ERROR_NONE && ___err != DL_ERROR_FULL_COMPONENT) { printf("%s:%i diana_" #F "(wreckerD, " #__VA_ARGS__ ") -> %i\n", __FILE__, __LINE__, ___err); break; } } while(0)

//  Typedefs to simplify common types
typedef unsigned int sysID;
typedef unsigned int componentID;
typedef unsigned int entID;

// Structs, probably will move these elsewhere
typedef struct cellDisplayBuffer
{
    short code;
    color_t foreColor;
    color_t backColor;
    bool needsUpdate;
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

/*
 *  Globals
 */

// State Globals
extern gameState    *Wrecker;
extern Event_s      *eventSys;
extern struct diana *wreckerD;
extern cellDisplayBuffer **displayBuffer;

#endif
