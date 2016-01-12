#ifndef _WRECKER_H
#define _WRECKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

#include "BearLibTerminal.h"
#include "dbg.h"

#define MS_PER_UPDATE 14
#define UPDATE_PER_FRAME 10

typedef struct gameState
{
    int currentState;
    uint64_t lastActual;
    uint64_t clock;
    uint64_t nextUpdate;
    uint64_t timeCurrent;
} gameState;

/*
 *  Globals
 */
extern gameState    *wreckState;
extern Event_s      *eventSys;
extern struct diana *diana;

#endif