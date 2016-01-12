#include "IO.h"
#include "wrecker.h"
#include "dbg.h"
#include "event.h"
#include "diana.h"

/*
 * Current function for pulling game time.
 * It's probably bad for some reason.
 */
uint64_t getTicks(void)
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000 + t.tv_usec/1000;
}

void initWreckState(void)
{
    wreckState = emalloc(sizeof gameState);
    wreckState->currentState = 1;
    wreckState->lastActual = getTicks();
    wreckState->clock = 0;
    wreckState->nextUpdate = 0;
    wreckState->timeCurrent = 0;
}

void initEventSys(){
    eventSys = newEvent_s();
    //registerEvents();
}

void dianaBuild(void)
{
    allocate_diana(emalloc, free, &diana);
    // Initialize components;
    // Add systems.
}

/*
 * Inits game state and allocates memory for DIANA
 */


void initWrecker(){
    initWreckState();
    initEventSys();
    dianaBuild(void);
    log_info("Wrecker systems initialized successfully");
}
