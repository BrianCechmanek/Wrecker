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
#include "dbg.h"

#include <sys/time.h>

// Macro for simplifying DIANA calls.
#define WRECK(F, ...) do { int ___err = diana_ ## F (wreckerD, ## __VA_ARGS__); if(___err != DL_ERROR_NONE && ___err != DL_ERROR_FULL_COMPONENT) { printf("%s:%i diana_" #F "(wreckerD, " #__VA_ARGS__ ") -> %i\n", __FILE__, __LINE__, ___err); BRK(); } } while(0)

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
    buildComponents();
    // buildSystems();
}

void buildComponents(void)
{
    WRECK(createComponent, "position", sizeof(Position_c), DL_COMPONENT_FLAG_INLINE, &positionComponent);


/*
 * Inits game state and allocates memory for DIANA
 */


void initWrecker(){
    initWreckState();
    initEventSys();
    dianaBuild(void);
    log_info("Wrecker systems initialized successfully");
}
