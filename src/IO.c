/*
 *  Wrecker
 *  IO.c 
 *  Input/Output functions; controls; interacting with system.
 */

#include "IO.h"

#include "wrecker.h"
#include "event.h"
#include "diana.h"
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

void registerComponents(void)
{
    WRECK(createComponent, "position", sizeof(Position_c), DL_COMPONENT_FLAG_INLINE, &Position);
    WRECK(createComponent, "render", sizeof(Render_c), DL_COMPONENT_FLAG_INLINE, &Render);
    WRECK(createComponent, "velocity", sizeof(Velocity_c), DL_COMPONENT_FLAG_INLINE, &Velocity);
}

void initSystems(void)
{
    initMovementSystem();
    initRenderSystem();
}

void initECS(void)
{
    allocate_diana(emalloc, free, &wreckerD);
    registerComponents();
    initSystems();
    WRECK(initialize);
}

/*
 * Inits game state and allocates memory for DIANA
 */

void initWrecker(){
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
