/*
 *  Wrecker
 *  IO.c 
 *  Input/Output functions; controls; interacting with system.
 */

#include "IO.h"

#include "wrecker.h"
#include "event.h"
#include "diana.h"
#include "ECS.h"
#include "components.h"
#include "systems.h"
#include "dbg.h"

#include <sys/time.h>

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
    wreckState = emalloc(sizeof(gameState));
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

/*
 * Inits game state and allocates memory for DIANA
 */

void initWrecker()
{
    initWreckState();
    initEventSys();
    initECS();
}

void updateGame( void )
{
    WRECK( process, 0 );
}

void render(void)
{
    WRECK(processSystem, renderSystem, 0);
    terminal_refresh();
}

void handleInput( int code )
{
    return;
}
